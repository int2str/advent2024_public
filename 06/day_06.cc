//
// int2str's Advent of CodeG 2024
// https://adventofcode.com/2024/day/6
//

#include "map.hh"
#include "state.hh"
#include "testrunner/testrunner.h"
#include "utils/coordinate.hh"
#include "utils/read_file.hh"

namespace Day6 {

[[nodiscard]] constexpr auto guardInBounds(const State& state) -> bool {
  return state.guard.position.x >= 0 and
         state.guard.position.x < state.map.size.x and  //
         state.guard.position.y >= 0 and
         state.guard.position.y < state.map.size.y;
}

void moveGuard(State& state) {
  state.visited.insert(state.guard.position);
  auto new_position = state.guard.position + state.guard.direction;
  while (state.map.blocked.contains(new_position)) {
    state.guard.direction.rotateClockwise();
    new_position = state.guard.position + state.guard.direction;
  }
  state.guard.position = new_position;
}

[[nodiscard]] auto hasLooped(State& state) -> bool {
  if (!state.visited.contains(state.guard.position)) state.travelled = 0;
  return ++state.travelled == state.max_travel;
}

void spyOnTheGuard(State& state) {
  state.resetGuard();

  // Part 1
  while (guardInBounds(state)) moveGuard(state);

  // Part 2
  state.switchToProbing();
  while (!state.noMoreCandidates()) {
    moveGuard(state);
    if (!guardInBounds(state)) {
      state.nextCandidate();

    } else if (hasLooped(state)) {
      ++state.obstruction_positions;
      state.nextCandidate();
    }
  }
}

}  // namespace Day6

TEST(Day_06_Guard_Gallivant_SAMPLE) {
  auto state =
      Day6::State{.map = Utils::readFileXY("06/sample.txt", Day6::Map{})};
  Day6::spyOnTheGuard(state);

  EXPECT_EQ(state.candidates_attempted + 1, 41);
  EXPECT_EQ(state.obstruction_positions, 6);
}
