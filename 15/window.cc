#include "window.hh"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

#include "utils/coordinate.hh"
#include "utils/coordinate_directions.hh"
#include "utils/coordinate_formatter.hh"

namespace {

using Utils::Coordinate;
using Utils::Direction;

[[nodiscard]] auto lerp(float t, sf::Vector2f a,
                        sf::Vector2f b) -> sf::Vector2f {
  return (1 - t) * a + t * b;
}

[[nodiscard]] auto wallTileFor(const Utils::Grid<char>& map,
                               Coordinate position) -> sf::IntRect {
  const int max_x = static_cast<int>(map.width()) - 1;
  const int max_y = static_cast<int>(map.height()) - 1;
  if (position.y == max_y or map[position + Direction::down()] != '#') {
    if (position.x < max_x and position.y < max_y and
        map[position + Direction::downRight()] == '#')
      return {16, 0, 16, 16};
    return {0, 0, 16, 16};
  }
  return {32, 0, 16, 16};
}

[[nodiscard]] auto robotTileFor(Coordinate direction) -> sf::IntRect {
  if (direction == Direction::down()) return {16, 0, 16, 16};
  if (direction == Direction::left()) return {32, 0, 16, 16};
  if (direction == Direction::right()) return {48, 0, 16, 16};
  return {0, 0, 16, 16};
}

}  // namespace

namespace Day15 {

constexpr auto WINDOW_TITLE = "Advent of Code";

const auto SIZE = 800;

auto Window::tileAt(Utils::Coordinate coordinate) const -> sf::Vector2f {
  return sf::Vector2f{static_cast<float>(coordinate.x) * tile_.x,
                      static_cast<float>(coordinate.y) * tile_.y};
}

Window::Window(Utils::Coordinate max)
    : max_{max},
      tile_{static_cast<float>(static_cast<uint32_t>(SIZE / max.x)),
            static_cast<float>(static_cast<uint32_t>(SIZE / max.y))},
      window_{sf::VideoMode{static_cast<uint32_t>(SIZE / max.x * max.x),
                            static_cast<uint32_t>(SIZE / max.y * max.y)},
              WINDOW_TITLE} {
  const auto scale =
      sf::Vector2f{static_cast<float>(SIZE) / static_cast<float>(16 * max_.x),
                   static_cast<float>(SIZE) / static_cast<float>(16 * max_.y)};

  texture_blocks_.loadFromFile("assets/tileset/tileset.png");
  block_.setTexture(texture_blocks_);
  block_.setScale(scale);

  texture_robot_.loadFromFile("assets/tileset/robot.png");
  robot_.setTexture(texture_robot_);
  robot_.setScale(scale);

  clock_.restart();
}

void Window::fps(unsigned int limit) { window_.setFramerateLimit(limit); }

auto Window::isOpen() const -> bool { return window_.isOpen(); }

void Window::handleEvents() {
  auto event = sf::Event{};
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window_.close();

    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code) {
        case sf::Keyboard::Key::Escape:
          window_.close();
          break;
        default:
          break;
      }
    }
  }
}

auto Window::draw(const Utils::Grid<char>& map, Coordinate robot_position,
                  Coordinate robot_direction) -> bool {
  if (robot_at_.isZero()) robot_at_ = robot_position;

  const auto tiles_per_second = static_cast<float>(max_.x) / 2.0F;
  const auto dt =
      std::min(1.0F, clock_.getElapsedTime().asSeconds() * tiles_per_second);

  if (dt >= 1.0F) {
    robot_at_ = robot_position;
    clock_.restart();
  }

  window_.clear(sf::Color::Red);
  for (const auto coordinate : map.coordinates()) {
    block_.setPosition(tileAt(coordinate));
    if (map[coordinate] == '#') {
      block_.setTextureRect(wallTileFor(map, coordinate));
    } else {
      block_.setTextureRect(sf::IntRect{48, 64, 16, 16});
    }
    window_.draw(block_);

    if (map[coordinate] == 'O') {
      block_.setTextureRect(sf::IntRect{0, 64, 16, 16});
      window_.draw(block_);
    }
  }

  robot_.setTextureRect(robotTileFor(robot_direction));
  robot_.setPosition(lerp(dt, tileAt(robot_at_), tileAt(robot_position)));
  window_.draw(robot_);

  window_.display();
  return (robot_at_ == robot_position);
}

}  // namespace Day15
