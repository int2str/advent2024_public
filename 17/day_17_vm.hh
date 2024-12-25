#ifndef DAY_17_VM_HH
#define DAY_17_VM_HH

#include <fmt/core.h>
#include <sys/mman.h>

#include <array>
#include <cstdint>
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

void emit_ADV(uint8_t arg, auto&& emit) {
  if (arg < 4) {
    const auto dd = static_cast<uint8_t>(arg & 0x7);
    emit(std::array<uint8_t, 4>{
        0x48, 0xc1, 0xef, dd,  // shr    $0x3,%rdi
    });
  } else if (arg == 4) {
    // Special case for RA / 2^RA: This always result in 0
    emit(std::array{
        0x48, 0x31, 0xff  // xor    %rdi,%rdi
    });
  } else if (arg == 5) {
    emit(std::array{
        0x48, 0x89, 0xf1,  // mov    %rsi,%rcx
        0x48, 0xd3, 0xef   // shr    %cl,%rdi
    });
  } else if (arg == 6) {
    emit(std::array{
        0x48, 0x89, 0xd1,  // mov    %rdx,%rcx
        0x48, 0xd3, 0xef   // shr    %cl,%rdi
    });
  }
}

void emit_BXL(uint8_t arg, auto&& emit) {
  const auto dd = static_cast<uint8_t>(arg & 0x7);
  emit(std::array<uint8_t, 4>{
      0x48, 0x83, 0xf6, dd  // xor    $dd, %rsi
  });
}

void emit_BXC(uint8_t /*unused*/, auto&& emit) {
  emit(std::array{
      0x48, 0x31, 0xd6  // xor    %rdx,%rsi
  });
}

void emit_JNZ(uint8_t /*unused*/, auto&& emit) {
  emit(std::array<uint8_t, 5>{
      0x48, 0x85, 0xff,  // test   %rdi,%rdi
      0x75, 0x00         // jnz  (0 jump for now; back-patch!)
  });
}

void emit_OUT(uint8_t arg, auto&& emit) {
  if (arg >= 4) {
    const auto from_reg = std::array<uint8_t, 3>{
        0xf8,  // RDI
        0xf0,  // RSI
        0xd0   // RDX

    };
    const auto dd = from_reg[arg - 4];
    emit(std::array<uint8_t, 14>{
        0x49, 0xc1, 0xe1, 0x03,  // shl    $0x3,%r9
        0x49, 0x89, dd,          // mov    <dd>,%r8
        0x49, 0x83, 0xe0, 0x07,  // and    $0x7,%r8
        0x4d, 0x09, 0xc1,        // or     %r8,%r9
    });

  } else {
    emit(std::array<uint8_t, 8>{
        0x49, 0xc1, 0xe1, 0x03,  // shl    $0x3,%r9
        0x49, 0x83, 0xc9, arg    // or     $0x3,%r9
    });
  }
}

auto compile(auto&& program, auto* x86_it) -> bool {
  auto emit = [&](auto&& rng) mutable {
    x86_it = std::copy(std::begin(rng), std::end(rng), x86_it);
  };

  // Setting %R9 to 1 initially as there might be leading zeros in the output.
  // This 1 needs to be removed from the final output before use.
  emit(std::array{
      0x49, 0xc7, 0xc1, 0x01, 0x00, 0x00, 0x00  // mov $0x1,%r9
  });

  auto jump_label = std::vector<uint8_t*>{};
  auto jumps      = std::vector<std::pair<uint8_t*, uint8_t>>{};

  auto in_it = std::begin(program);
  while (in_it != std::end(program)) {
    if (jump_label.size() < 8) jump_label.emplace_back(x86_it);

    const auto op  = *in_it++;
    const auto arg = *in_it++;
    switch (op) {
        // clang-format off
      case OpCode::ADV: emit_ADV(arg, emit); break;
      case OpCode::BXL: emit_BXL(arg, emit); break;
      case OpCode::JNZ: emit_JNZ(arg, emit); jumps.emplace_back(x86_it, arg); break;
      case OpCode::BXC: emit_BXC(arg, emit); break;
      case OpCode::OUT: emit_OUT(arg, emit); break;
        // clang-format on

      case OpCode::CDV: {
        if (arg == 5) {
          emit(std::array{
              0x48, 0x89, 0xf1,  // mov    %rsi,%rcx
              0x48, 0x89, 0xfa,  // mov    %rdi,%rdx
              0x48, 0xd3, 0xea   // shr    %cl,%rdx
          });
        } else {
          fmt::print("Unhandled arg for op {} ({})\n", op, arg);
          return false;
        }
        break;
      }

      case OpCode::BST:
        if (arg == 4) {
          emit(std::array{
              0x48, 0x89, 0xfe,       // mov    %rdi,%rsi
              0x48, 0x83, 0xe6, 0x07  // and    $0x7,%rsi
          });
        } else {
          fmt::print("Unhandled arg for op {} ({})\n", op, arg);
          return false;
        }
        break;

      default:
        fmt::print("Unhandled command {} ({})\n", op, arg);
        return false;
    }
  }

  emit(std::array{
      0x4c, 0x89, 0xc8,  // mov    %r9,%rax
      0xc3               // ret
  });

  // Back-patch jump labels
  for (const auto [back_patch_at, jump_label_index] : jumps) {
    *(back_patch_at - 1) = static_cast<uint8_t>(
        -(back_patch_at - jump_label.at(jump_label_index)));
  }

  return true;
}

using chronospatial_computer = size_t (*)(size_t, size_t, size_t);

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
