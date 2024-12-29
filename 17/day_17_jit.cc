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
  const auto div_ac = std::vector<uint8_t>{0, 6, 5, 4};
  EXPECT_EQ(out({0}), jit(div_ac)(7U, 0U, 3U));
  EXPECT_EQ(out({3}), jit(div_ac)(7U, 0U, 1U));
  EXPECT_EQ(out({3}), jit(div_ac)(25U, 0U, 3U));
  EXPECT_EQ(out({4}), jit(div_ac)(4'096U, 0U, 10U));
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
// OPCODE 2 - BST
//

TEST(Day_17_Chronospatial_Computer_JIT_BST_Literal) {
  const auto bst_0 = std::vector<uint8_t>{2, 0, 5, 5};
  const auto bst_1 = std::vector<uint8_t>{2, 1, 5, 5};
  const auto bst_2 = std::vector<uint8_t>{2, 2, 5, 5};
  const auto bst_3 = std::vector<uint8_t>{2, 3, 5, 5};
  EXPECT_EQ(out({0}), jit(bst_0)(0U, 0U, 0U));
  EXPECT_EQ(out({1}), jit(bst_1)(0U, 0U, 0U));
  EXPECT_EQ(out({2}), jit(bst_2)(0U, 0U, 0U));
  EXPECT_EQ(out({3}), jit(bst_3)(0U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_BST_Register_A) {
  const auto bst_a = std::vector<uint8_t>{2, 4, 5, 5};
  EXPECT_EQ(out({0}), jit(bst_a)(0U, 0U, 0U));
  EXPECT_EQ(out({3}), jit(bst_a)(3U, 0U, 0U));
  EXPECT_EQ(out({7}), jit(bst_a)(7U, 0U, 0U));
  EXPECT_EQ(out({2}), jit(bst_a)(10U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_BST_Register_B) {
  const auto bst_b = std::vector<uint8_t>{2, 5, 5, 5};
  EXPECT_EQ(out({0}), jit(bst_b)(0U, 0U, 0U));
  EXPECT_EQ(out({3}), jit(bst_b)(0U, 3U, 0U));
  EXPECT_EQ(out({7}), jit(bst_b)(0U, 7U, 0U));
  EXPECT_EQ(out({2}), jit(bst_b)(0U, 10U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_BST_Register_C) {
  const auto bst_c = std::vector<uint8_t>{2, 6, 5, 5};
  EXPECT_EQ(out({0}), jit(bst_c)(0U, 0U, 0U));
  EXPECT_EQ(out({3}), jit(bst_c)(0U, 0U, 3U));
  EXPECT_EQ(out({7}), jit(bst_c)(0U, 0U, 7U));
  EXPECT_EQ(out({2}), jit(bst_c)(0U, 0U, 10U));
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
// OPCODE 4 - BXC
//

TEST(Day_17_Chronospatial_Computer_JIT_BXC) {
  const auto bxc = std::vector<uint8_t>{4, 0, 5, 5};
  EXPECT_EQ(out({0}), jit(bxc)(0U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(bxc)(0U, 7U, 7U));
  EXPECT_EQ(out({1}), jit(bxc)(0U, 1U, 0U));
  EXPECT_EQ(out({1}), jit(bxc)(0U, 0U, 1U));
  EXPECT_EQ(out({3}), jit(bxc)(0U, 2U, 1U));
  EXPECT_EQ(out({5}), jit(bxc)(0U, 1U, 4U));
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
// OPCODE 6 - BDV
//

TEST(Day_17_Chronospatial_Computer_JIT_BDV_Literal) {
  const auto div_1 = std::vector<uint8_t>{6, 0, 5, 5};
  EXPECT_EQ(out({7}), jit(div_1)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_1)(0U, 0U, 0U));

  const auto div_2 = std::vector<uint8_t>{6, 1, 5, 5};
  EXPECT_EQ(out({3}), jit(div_2)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_2)(0U, 0U, 0U));

  const auto div_4 = std::vector<uint8_t>{6, 2, 5, 5};
  EXPECT_EQ(out({1}), jit(div_4)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_4)(0U, 0U, 0U));

  const auto div_8 = std::vector<uint8_t>{6, 3, 5, 5};
  EXPECT_EQ(out({3}), jit(div_8)(24U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_8)(7U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_BDV_Register_A) {
  const auto div_aa = std::vector<uint8_t>{6, 4, 5, 5, 5, 4};
  EXPECT_EQ(out({0, 7}), jit(div_aa)(7U, 0U, 0U));
  EXPECT_EQ(out({0, 0}), jit(div_aa)(0U, 0U, 0U));
  EXPECT_EQ(out({0, 0}), jit(div_aa)(1'024U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_BDV_Register_B) {
  const auto div_ab = std::vector<uint8_t>{6, 5, 5, 5, 5, 4};
  EXPECT_EQ(out({0, 7}), jit(div_ab)(7U, 3U, 0U));
  EXPECT_EQ(out({3, 7}), jit(div_ab)(7U, 1U, 0U));
  EXPECT_EQ(out({3, 1}), jit(div_ab)(25U, 3U, 0U));
  EXPECT_EQ(out({4, 0}), jit(div_ab)(4'096U, 10U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_BDV_Register_C) {
  const auto div_ac = std::vector<uint8_t>{6, 6, 5, 5, 5, 4};
  EXPECT_EQ(out({0, 7}), jit(div_ac)(7U, 0U, 3U));
  EXPECT_EQ(out({3, 7}), jit(div_ac)(7U, 0U, 1U));
  EXPECT_EQ(out({3, 1}), jit(div_ac)(25U, 0U, 3U));
  EXPECT_EQ(out({4, 0}), jit(div_ac)(4'096U, 0U, 10U));
}

//
// OPCODE 7 - CDV
//

TEST(Day_17_Chronospatial_Computer_JIT_CDV_Literal) {
  const auto div_1 = std::vector<uint8_t>{7, 0, 5, 6};
  EXPECT_EQ(out({7}), jit(div_1)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_1)(0U, 0U, 0U));

  const auto div_2 = std::vector<uint8_t>{7, 1, 5, 6};
  EXPECT_EQ(out({3}), jit(div_2)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_2)(0U, 0U, 0U));

  const auto div_4 = std::vector<uint8_t>{7, 2, 5, 6};
  EXPECT_EQ(out({1}), jit(div_4)(7U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_4)(0U, 0U, 0U));

  const auto div_8 = std::vector<uint8_t>{7, 3, 5, 6};
  EXPECT_EQ(out({3}), jit(div_8)(24U, 0U, 0U));
  EXPECT_EQ(out({0}), jit(div_8)(7U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_CDV_Register_A) {
  const auto div_aa = std::vector<uint8_t>{7, 4, 5, 6, 5, 4};
  EXPECT_EQ(out({0, 7}), jit(div_aa)(7U, 0U, 0U));
  EXPECT_EQ(out({0, 0}), jit(div_aa)(0U, 0U, 0U));
  EXPECT_EQ(out({0, 0}), jit(div_aa)(1'024U, 0U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_CDV_Register_B) {
  const auto div_ab = std::vector<uint8_t>{7, 5, 5, 6, 5, 4};
  EXPECT_EQ(out({0, 7}), jit(div_ab)(7U, 3U, 0U));
  EXPECT_EQ(out({3, 7}), jit(div_ab)(7U, 1U, 0U));
  EXPECT_EQ(out({3, 1}), jit(div_ab)(25U, 3U, 0U));
  EXPECT_EQ(out({4, 0}), jit(div_ab)(4'096U, 10U, 0U));
}

TEST(Day_17_Chronospatial_Computer_JIT_CDV_Register_C) {
  const auto div_ac = std::vector<uint8_t>{7, 6, 5, 6, 5, 4};
  EXPECT_EQ(out({0, 7}), jit(div_ac)(7U, 0U, 3U));
  EXPECT_EQ(out({3, 7}), jit(div_ac)(7U, 0U, 1U));
  EXPECT_EQ(out({3, 1}), jit(div_ac)(25U, 0U, 3U));
  EXPECT_EQ(out({4, 0}), jit(div_ac)(4'096U, 0U, 10U));
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

TEST(Day_17_Chronospatial_Computer_JIT_Challenge) {
  const auto ra = 37283687U;
  const auto src =
      std::vector<uint8_t>{2, 4, 1, 3, 7, 5, 4, 1, 1, 3, 0, 3, 5, 5, 3, 0};
  const auto ret = std::vector<uint8_t>{1, 5, 3, 0, 2, 5, 2, 5, 3};
  EXPECT_EQ(encode3Bit(ret), jit(src)(ra, 0U, 0U));
}
