#ifndef DAY_17_VM_HH
#define DAY_17_VM_HH

#include <fmt/core.h>
#include <sys/mman.h>

#include <array>
#include <cstdint>
#include <optional>
#include <vector>

namespace Day17::Detail {

template <size_t SIZE>
[[nodiscard]] auto allocateWritable() -> std::array<uint8_t, SIZE>* {
  void* mmaped = mmap(nullptr, SIZE, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (mmaped == reinterpret_cast<void*>(-1)) {  // NOLINT
    fmt::print("Unable to allocate writable memory\n");
    return nullptr;
  }
  return new (mmaped) std::array<uint8_t, SIZE>{};
}

template <size_t SIZE>
auto makeExecutable(std::array<uint8_t, SIZE>* program) -> bool {
  if (mprotect(program, program->size(), PROT_READ | PROT_EXEC) == -1) {
    fmt::print("Unable to make memory executable\n");
    return false;
  }
  return true;
}

}  // namespace Day17::Detail

namespace Day17 {

enum OpCode : uint8_t { ADV = 0x00, BXL, BST, JNZ, BXC, OUT, BDV, CDV };

enum Register : uint8_t { RA = 0x04, RB, RC };

[[nodiscard]] constexpr auto isLiteral(uint8_t operand) -> bool {
  return operand < 4U;
}

[[nodiscard]] constexpr auto literal(uint8_t operand) -> uint8_t {
  return static_cast<uint8_t>(operand & 0x7);
}

// The JIT compiler will create a single function
// with the following signature:
//
using chronospatial_computer = size_t (*)(size_t, size_t, size_t);

// On Linux, x86, the following registers are used for
// this function call
//   %rdi -> First function parameter
//           This will represent Register 'A' (short 'RA')
//   %rsi -> Second function paramter
//           This will represent Register 'B' (short 'RB')
//   %rdx -> Second function paramter
//           This will represent Register 'C' (short 'RC')
//
// The return value of this function is passed in this register:
//   %rax -> Function return value
//
// The output is presented as a single size_t (aka uint64_t) value,
// which represents a string of 3-bit values, with a leading '1' bit.
// See description of the 'OUT' opcode for details.

//
// OPCODE 0 -> ADV
//
// Divides register RA by 2^COMBO(RA), where COMBO() is either a
// 3-bit literal or the value of RA, RB or RC.
//
// The ADV, BDV and CDV functions all use an intermediate register:
//   %rcx -> Temprarily holds value to be used in shift-right
//
// This temporary register is used, since the 64-bit input registers
// cannot be used directly in the SHR instruction.
//
void emit_ADV(uint8_t operand, auto&& emit) {
  if (isLiteral(operand)) {
    emit(std::array<uint8_t, 4>{
        0x48, 0xc1, 0xef, literal(operand),  // shr    <op>,%rdi
    });
  } else if (operand == Register::RA) {
    // Special case for RA / 2^RA: This always result in 0
    emit(std::array{
        0x48, 0x31, 0xff  // xor    %rdi,%rdi
    });
  } else if (operand == Register::RB) {
    emit(std::array{
        0x48, 0x89, 0xf1,  // mov    %rsi,%rcx
        0x48, 0xd3, 0xef   // shr    %cl,%rdi
    });
  } else if (operand == Register::RC) {
    emit(std::array{
        0x48, 0x89, 0xd1,  // mov    %rdx,%rcx
        0x48, 0xd3, 0xef   // shr    %cl,%rdi
    });
  }
}

//
// OPCODE 6 -> BDV
//
// Same as ADV (see above).
// Result is stored in RB.
//
void emit_BDV(uint8_t operand, auto&& emit) {
  if (isLiteral(operand)) {
    emit(std::array<uint8_t, 7>{
        0x48, 0x89, 0xfe,                    // mov    %rdi,%rsi
        0x48, 0xc1, 0xee, literal(operand),  // shr    <op>,%rsi
    });
  } else if (operand == Register::RA) {
    // Special case for RA / 2^RA: This always result in 0
    // ... which in the BDV case means we clear out RB
    emit(std::array{
        0x48, 0x31, 0xf6  // xor    %rsi,%rsi
    });
  } else if (operand == Register::RB) {
    emit(std::array{
        0x48, 0x89, 0xf1,  // mov    %rsi,%rcx
        0x48, 0x89, 0xfe,  // mov    %rdi,%rsi
        0x48, 0xd3, 0xee   // shr    %cl,%rsi
    });
  } else if (operand == Register::RC) {
    emit(std::array{
        0x48, 0x89, 0xd1,  // mov    %rdx,%rcx
        0x48, 0x89, 0xfe,  // mov    %rdi,%rsi
        0x48, 0xd3, 0xee   // shr    %cl,%rsi
    });
  }
}

//
// OPCODE 7 -> CDV
//
// Same as ADV (see above).
// Result is stored in RC.
//
void emit_CDV(uint8_t operand, auto&& emit) {
  if (isLiteral(operand)) {
    emit(std::array<uint8_t, 7>{
        0x48, 0x89, 0xfa,                   // mov    %rdi,%rdx
        0x48, 0xc1, 0xea, literal(operand)  // shr    <op>,%rdx
    });
  } else if (operand == Register::RA) {
    // Special case for RA / 2^RA: This always result in 0
    // ... which in the CDV case means we clear out RC
    emit(std::array{
        0x48, 0x31, 0xd2  // xor    %rdx,%rdx
    });
  } else if (operand == Register::RB) {
    emit(std::array{
        0x48, 0x89, 0xf1,  // mov    %rsi,%rcx
        0x48, 0x89, 0xfa,  // mov    %rdi,%rdx
        0x48, 0xd3, 0xea   // shr    %cl,%rdx
    });
  } else if (operand == Register::RC) {
    emit(std::array{
        0x48, 0x89, 0xd1,  // mov    %rdx,%rcx
        0x48, 0x89, 0xfa,  // mov    %rdi,%rdx
        0x48, 0xd3, 0xea   // shr    %cl,%rdx
    });
  }
}

//
// OPCODE 1 -> BXL
//
// Bitwise XOR of RB and the literal operand.
//
void emit_BXL(uint8_t operand, auto&& emit) {
  emit(std::array<uint8_t, 4>{
      0x48, 0x83, 0xf6, literal(operand)  // xor    <op>, %rsi
  });
}

//
// OPCODE 2 -> BST
//
// Store the value of COMBO(operand) in RB.
//
void emit_BST(uint8_t operand, auto&& emit) {
  if (isLiteral(operand)) {
    emit(std::array<uint8_t, 7>{
        0x48, 0xc7, 0xc6, literal(operand), 0x00, 0x00, 0x00  // mov <dd>,%rsi
    });
  } else if (operand == Register::RA) {
    emit(std::array{
        0x48, 0x89, 0xfe  // mov    %rdi,%rsi
    });
  } else if (operand == Register::RB) {
    // Nothing to do. RB is already in RB; just mod8 below...

  } else if (operand == Register::RC) {
    emit(std::array{
        0x48, 0x89, 0xd6  // mov    %rdx,%rsi
    });
  }

  // Always mod 8 in the end...
  emit(std::array{
      0x48, 0x83, 0xe6, 0x07  // and    $0x7,%rsi
  });
}

//
// OPCODE 3 -> JNZ
//
// Jumps to the given instruction, identified by the
// absolute instruction counter given by the operand,
// if the value in RA is NOT zero.
//
// NOTE:
// This only emits the JNZ instruction code assembly.
// The actual (relative) x86 instruction address to jump
// to is back-patched later, once the instruction address
// is known.
// The x86_64 JNZ instruction is given a signed, 8-bit
// relative jump address.
void emit_JNZ(uint8_t /*unused*/, auto&& emit) {
  emit(std::array<uint8_t, 5>{
      0x48, 0x85, 0xff,  // test   %rdi,%rdi
      0x75, 0x00         // jnz  (0 jump for now; back-patch!)
  });
}

//
// OPCODE 4 -> BCX
//
// Bitwise XOR of RB and RC.
// The result is stored in RB
//
// The operand is unused.
//
void emit_BXC(uint8_t /*unused*/, auto&& emit) {
  emit(std::array{
      0x48, 0x31, 0xd6  // xor    %rdx,%rsi
  });
}

//
// OPCODE 5 -> OUT
//
// Adds the 3-bit value identified by COMBO(operand) to the
// return value of the function.
//
// The OUT instruction uses an additional intermediate
// register:
//     %r8 - Tempoarily stores the value to be output, in
//           order to perform a "mod 8" operation first,
//           since OUT outputs 3 bits at a time.
//
void emit_OUT(uint8_t operand, auto&& emit) {
  if (isLiteral(operand)) {
    emit(std::array<uint8_t, 8>{
        0x48, 0xc1, 0xe0, 0x03,             // shl    $0x3,%rax
        0x48, 0x83, 0xc8, literal(operand)  // or     <op>,%rax
    });

  } else {
    const auto ra_rb_rc = std::array<uint8_t, 3>{0xf8, 0xf0, 0xd0};
    const auto from     = ra_rb_rc[operand - 4];
    emit(std::array<uint8_t, 14>{
        0x48, 0xc1, 0xe0, 0x03,  // shl    $0x3,%rax
        0x49, 0x89, from,        // mov    <from>,%r8
        0x49, 0x83, 0xe0, 0x07,  // and    $0x7,%r8
        0x4c, 0x09, 0xc0,        // or     %r8,%rax
    });
  }
}

auto compile(auto&& program, auto* x86_it) -> std::optional<size_t> {
  const auto before = x86_it;

  auto emit = [&](auto&& rng) mutable {
    x86_it = std::copy(std::begin(rng), std::end(rng), x86_it);
  };

  // Setting %rax to 1 initially as there might be leading zeros in the output.
  // This 1 needs to be removed from the final output before use.
  emit(std::array{
      0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00  // mov $0x1,%rax
  });

  auto jump_label = std::vector<uint8_t*>{};
  auto jumps      = std::vector<std::pair<uint8_t*, uint8_t>>{};

  auto in_it = std::begin(program);
  while (in_it != std::end(program)) {
    // Remember jump-to address since it might be needed to back-patch
    // a JNZ instruction later...
    jump_label.emplace_back(x86_it);

    const auto opcode  = *in_it++;
    const auto operand = *in_it++;
    switch (opcode) {
        // clang-format off
      case OpCode::ADV: emit_ADV(operand, emit); break;
      case OpCode::BXL: emit_BXL(operand, emit); break;
      case OpCode::BST: emit_BST(operand, emit); break;
      case OpCode::JNZ: emit_JNZ(operand, emit); jumps.emplace_back(x86_it, operand); break;
      case OpCode::BXC: emit_BXC(operand, emit); break;
      case OpCode::OUT: emit_OUT(operand, emit); break;
      case OpCode::BDV: emit_BDV(operand, emit); break;
      case OpCode::CDV: emit_CDV(operand, emit); break;
        // clang-format on

      default:
        fmt::print("Unhandled command {} ({})\n", opcode, operand);
        return std::nullopt;
    }
  }

  emit(std::array{
      0xc3  // ret
  });

  // Back-patch jump labels
  for (const auto [back_patch_at, jump_label_index] : jumps) {
    *(back_patch_at - 1) = static_cast<uint8_t>(
        -(back_patch_at - jump_label.at(jump_label_index)));
  }

  return std::distance(before, x86_it);
}

template <size_t SIZE = 1'024>
[[nodiscard]] auto jit(auto&& program) -> chronospatial_computer {
  auto* x86 = Detail::allocateWritable<SIZE>();
  if (!compile(program, x86->begin())) return nullptr;

  Detail::makeExecutable(x86);

  return reinterpret_cast<chronospatial_computer>(x86);
}

[[nodiscard]] constexpr auto decode3Bit(size_t value) -> std::vector<uint8_t> {
  auto out = std::vector<uint8_t>{};
  do {
    out.insert(out.cbegin(), value & 0x7);
    value >>= 3;
  } while (value != 0);
  out.erase(out.begin());
  return out;
}

[[nodiscard]] constexpr auto encode3Bit(const std::vector<uint8_t>& bytes)
    -> size_t {
  auto value = size_t{1};
  for (const auto bte : bytes) {
    value <<= 3;
    value |= bte;
  }
  return value;
}

}  // namespace Day17
#endif  // DAY_17_VM_HH
