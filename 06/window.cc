#include "window.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "state.hh"
#include "utils/coordinate.hh"

namespace Day6 {

constexpr auto WINDOW_TITLE = "Advent of Code";

constexpr auto PIXEL_SCALE = 50U;

const auto COLOR_BACKGROUND = sf::Color{10, 20, 10};
const auto COLOR_VISITED    = sf::Color{200, 200, 0};
const auto COLOR_BLOCK      = sf::Color{0, 128, 0};
const auto COLOR_FONT       = sf::Color{255, 255, 255};
const auto COLOR_GUARD      = sf::Color{255, 0, 0};
const auto COLOR_CANDIDATE  = sf::Color{0, 0, 255};

const auto DEFAULT_TEXT_POSITION = 10U;

const auto HALF = 2.0F;

auto drawBlock(sf::RenderWindow& window, auto& item, const auto& position) {
  item.setPosition(sf::Vector2f{static_cast<float>(position.x) * PIXEL_SCALE,
                                static_cast<float>(position.y) * PIXEL_SCALE});
  window.draw(item);
}

auto drawBlocks(sf::RenderWindow& window, auto& item, const auto& positions) {
  for (const auto& position : positions) drawBlock(window, item, position);
}

Window::Window(const State* state)
    : state_{state},
      window_{
          sf::VideoMode{static_cast<uint32_t>(state->map.size.x) * PIXEL_SCALE,
                        static_cast<uint32_t>(state->map.size.y) * PIXEL_SCALE},
          WINDOW_TITLE},
      guard_{static_cast<float>(PIXEL_SCALE)},
      candidate_{static_cast<float>(PIXEL_SCALE)},
      block_{{PIXEL_SCALE, PIXEL_SCALE}},
      visited_{{PIXEL_SCALE, PIXEL_SCALE}} {
  window_.setFramerateLimit(frame_limit_);

  guard_.setFillColor(COLOR_GUARD);
  guard_.setOrigin(static_cast<float>(PIXEL_SCALE) / HALF,
                   static_cast<float>(PIXEL_SCALE) / HALF);

  candidate_.setFillColor(COLOR_CANDIDATE);
  candidate_.setOrigin(static_cast<float>(PIXEL_SCALE) / HALF,
                       static_cast<float>(PIXEL_SCALE) / HALF);

  block_.setFillColor(COLOR_BLOCK);
  visited_.setFillColor(COLOR_VISITED);

  font_.loadFromFile("assets/LiberationSans-Regular.ttf");

  text_.setFont(font_);
  text_.setFillColor(COLOR_FONT);
  text_.setStyle(sf::Text::Bold);
  text_.setPosition(DEFAULT_TEXT_POSITION, DEFAULT_TEXT_POSITION);
}

auto Window::isOpen() const -> bool { return window_.isOpen(); }

void Window::handleEvents() {
  auto event = sf::Event{};
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window_.close();

    if (event.type == sf::Event::KeyPressed) {
      switch (event.key.code) {
        case sf::Keyboard::Key::PageUp:
          frame_limit_ *= 2;
          break;
        case sf::Keyboard::Key::PageDown:
          frame_limit_ /= 2;
          break;
        case sf::Keyboard::Key::Space:
          frame_limit_ = 1;
          break;
        case sf::Keyboard::Key::Enter:
          frame_limit_ = frame_limit_ <= DEFAULT_FRAMERATE
                             ? static_cast<uint32_t>(-1)
                             : DEFAULT_FRAMERATE;
          break;
        case sf::Keyboard::Key::Escape:
          window_.close();
          break;
        default:
          break;
      }
      window_.setFramerateLimit(frame_limit_);
    }
  }
}

void Window::draw() {
  window_.clear(COLOR_BACKGROUND);

  for (const auto& pos : state_->map.blocked) drawBlock(window_, block_, pos);
  for (const auto& pos : state_->visited) drawBlock(window_, visited_, pos);

  if (state_->mode == Mode::Tracing) {
    drawBlock(window_, guard_, state_->guard.position);

    text_.setString(std::format("Steps: {}", state_->visited.count()));
  }

  if (state_->mode == Mode::Probing) {
    drawBlocks(window_, visited_, state_->candidates);
    drawBlock(window_, candidate_, state_->candidate_at);

    text_.setString(
        std::format("Obstructions: {}", state_->obstruction_positions));
  }

  if (state_->mode == Mode::Done) {
    text_.setString(std::format(
        "Advent of Code - Day 6\n\nGuard positions: {}\nPossible obstacles: {}",
        state_->candidates_attempted + 1, state_->obstruction_positions));
    const auto text_rect = text_.getLocalBounds();
    text_.setOrigin(text_rect.width / HALF, text_rect.height / HALF);
    text_.setPosition(window_.getView().getCenter());
  }

  window_.draw(text_);
  window_.display();
}

}  // namespace Day6
