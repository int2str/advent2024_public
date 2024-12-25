#include "window.hh"

#include <fmt/core.h>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "utils/coordinate.hh"

namespace Day14 {

constexpr auto WINDOW_TITLE = "Advent of Code";

const auto COLOR_BACKGROUND = sf::Color{10, 20, 10};
const auto COLOR_GRID       = sf::Color{45, 90, 45};
const auto COLOR_ROBOT      = sf::Color::Green;
const auto COLOR_FONT       = sf::Color::White;

const auto DEFAULT_TEXT_POSITION = 10U;

const auto SIZE = 768;

Window::Window(Utils::Coordinate max)
    : max_{max},
      tile_{static_cast<float>(static_cast<uint32_t>(SIZE / max.x)),
            static_cast<float>(static_cast<uint32_t>(SIZE / max.y))},
      window_{sf::VideoMode{static_cast<uint32_t>(SIZE / max.x * max.x),
                            static_cast<uint32_t>(SIZE / max.y * max.y)},
              WINDOW_TITLE} {
  auto width  = static_cast<float>(window_.getSize().x);
  auto height = static_cast<float>(window_.getSize().y);

  robot_.setSize(tile_);
  robot_.setSize(sf::Vector2f{tile_.x - 2.0F, tile_.y - 2.0F});
  robot_.setOrigin(sf::Vector2f{-1.0F, -1.0F});
  robot_.setFillColor(COLOR_ROBOT);

  font_.loadFromFile("assets/LiberationSans-Regular.ttf");

  text_.setFont(font_);
  text_.setCharacterSize(20);
  text_.setFillColor(COLOR_FONT);
  text_.setStyle(sf::Text::Bold);
  text_.setPosition(DEFAULT_TEXT_POSITION, DEFAULT_TEXT_POSITION);
  text_.setOutlineColor(COLOR_BACKGROUND);
  text_.setOutlineThickness(1);

  lines_.append({sf::Vector2f{0.0F, height / 2 - tile_.y / 2}, COLOR_GRID});
  lines_.append({sf::Vector2f{width, height / 2 - tile_.y / 2}, COLOR_GRID});
  lines_.append({sf::Vector2f{0.0F, height / 2 + tile_.y / 2}, COLOR_GRID});
  lines_.append({sf::Vector2f{width, height / 2 + tile_.y / 2}, COLOR_GRID});

  lines_.append({sf::Vector2f{width / 2 - tile_.x / 2, 0.0F}, COLOR_GRID});
  lines_.append({sf::Vector2f{width / 2 - tile_.x / 2, height}, COLOR_GRID});
  lines_.append({sf::Vector2f{width / 2 + tile_.x / 2, 0.0F}, COLOR_GRID});
  lines_.append({sf::Vector2f{width / 2 + tile_.x / 2, height}, COLOR_GRID});
}

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

auto Window::robotAt(const Robot& robot) const -> sf::Vector2f {
  return {static_cast<float>(robot.position.x) * tile_.x,
          static_cast<float>(robot.position.y) * tile_.y};
}

void Window::draw(const Robots& anomaly, size_t anomaly_at, double distance,
                  size_t steps) {
  window_.clear(COLOR_BACKGROUND);
  window_.draw(lines_);

  for (const auto robot : anomaly) {
    robot_.setPosition(robotAt(robot));
    window_.draw(robot_);
  }

  text_.setString(
      fmt::format("Anomaly detected at {}, magnitude {:.2F}\nCurrent step: {}",
                  anomaly_at, distance, steps));
  window_.draw(text_);
  window_.display();
}

}  // namespace Day14
