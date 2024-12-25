//
// int2str's Advent of Code 2024
// https://adventofcode.com/2024/day/17
//

#include <cstdint>
#include <vector>

#include "day_17_vm.hh"
#include "testrunner/testrunner.h"

using Day17::encode3Bit;
using Day17::jit;

namespace {

[[nodiscard]] constexpr auto out(const std::vector<uint8_t>& value) -> size_t {
  return Day17::encode3Bit(value);
}

}  // namespace

//
// OPCODE 0 - ADV
//

TEST(Day_17_Chronospatial_Computer_JIT_ADV_Literal) {
  const auto div_1 = std::vector<uint8_t>{0, 0, 5, 4};
  EXPECT_EQ(out({7}), jit(div_1)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_1)(0U, 0U, 0U));

  const auto div_2 = std::vector<uint8_t>{0, 1, 5, 4};
  EXPECT_EQ(out({3}), jit(div_2)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_2)(0U, 0U, 0U));

  const auto div_4 = std::vector<uint8_t>{0, 2, 5, 4};
  EXPECT_EQ(out({1}), jit(div_4)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_4)(0U, 0U, 0U));

  const auto div_8 = std::vector<uint8_t>{0, 3, 5, 4};
  EXPECT_EQ(out({3}), jit(div_8)(24U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_8)(7U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_ADV_Register_A) {
  const auto div_aa = std::vector<uint8_t>{0, 4, 5, 4};
  EXPECT_EQ(out({0}), jit(div_aa)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_aa)(0U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_aa)(1'024U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_ADV_Register_B) {
  const auto div_ab = std::vector<uint8_t>{0, 5, 5, 4};
  EXPECT_EQ(out({0}), jit(div_ab)(7U, 3U, 0U));
  EXPECT_EQ(out({3}), jit(div_ab)(7U, 1U, 0U));
  EXPECT_EQ(out({3}), jit(div_ab)(25U, 3U, 0U));
  EXPECT_EQ(out({4}), jit(div_ab)(4'096U, 10U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_ADV_Register_C) {
  const auto div_ac = std::vector<uint8_t>{0, 5, 5, 4};
  EXPECT_EQ(out({0}), jit(div_ac)(7U, 3U, 0U));
  EXPECT_EQ(out({3}), jit(div_ac)(7U, 1U, 0U));
  EXPECT_EQ(out({3}), jit(div_ac)(25U, 3U, 0U));
  EXPECT_EQ(out({4}), jit(div_ac)(4'096U, 10U, 0U));
}

//
// OPCODE 1 - BXL
//

TEST(Day_17_Chronospatial_Computer_JIT_BXL) {
  const auto xor_0 = std::vector<uint8_t>{1, 0, 5, 5};
  const auto xor_7 = std::vector<uint8_t>{1, 7, 5, 5};
  EXPECT_EQ(out({0}), jit(xor_0)(0U, 0U, 0U));
  EXPECT_EQ(out({5}), jit(xor_0)(0U, 5U, 0U));
  EXPECT_EQ(out({4}), jit(xor_7)(0U, 3U, 0U));
  EXPECT_EQ(out({0}), jit(xor_7)(0U, 7U, 0U));
}

//
// OPCODE 3 - JNZ
//

TEST(Day_17_Chronospatial_Computer_JIT_JNZ_Loop) {
  const auto src = std::vector<uint8_t>{0, 1, 5, 4, 3, 0};
  EXPECT_EQ(out({4, 2, 1, 0}), jit(src)(8U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_JNZ_Jump_Forward) {
  const auto src = std::vector<uint8_t>{3, 2, 5, 5, 5, 6};
  EXPECT_EQ(out({1, 2}), jit(src)(0U, 1U, 2U));
  EXPECT_EQ(out({2}), jit(src)(1U, 1U, 2U));
}

//
// OPCODE 5 - OUT
//

TEST(Day_17_Chronospatial_Computer_JIT_OUT_Literal) {
  const auto src = std::vector<uint8_t>{5, 0, 5, 1, 5, 2, 5, 3};
  EXPECT_EQ(out({0, 1, 2, 3}), jit(src)(0U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_OUT_Registers_Simple) {
  const auto src = std::vector<uint8_t>{5, 4, 5, 5, 5, 6};
  EXPECT_EQ(out({3, 2, 1}), jit(src)(3U, 2U, 1U));
}

TEST(Day_17_Chronospatial_Computer_JIT_OUT_Registers_Mod8) {
  const auto src = std::vector<uint8_t>{5, 4, 5, 5, 5, 6};
  EXPECT_EQ(out({0, 1, 2}), jit(src)(8U, 9U, 10U));
  EXPECT_EQ(out({3, 4, 5}), jit(src)(11U, 12U, 13U));
  EXPECT_EQ(out({6, 7, 0}), jit(src)(14, 15, 16));
}

//
// Complete sample programs
//

TEST(Day_17_Chronospatial_Computer_JIT_QuineIsQuine) {
  const auto ra  = 117'440U;
  const auto src = std::vector<uint8_t>{0, 3, 5, 4, 3, 0};
  EXPECT_EQ(encode3Bit(src), jit(src)(ra, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_Sample) {
  const auto ra  = 729U;
  const auto src = std::vector<uint8_t>{0, 1, 5, 4, 3, 0};
  const auto ret = std::vector<uint8_t>{4, 6, 3, 5, 6, 3, 5, 2, 1, 0};
  EXPECT_EQ(encode3Bit(ret), jit(src)(ra, 0U, 0U));
}
