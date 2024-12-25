#ifndef DAY_6_WINDOW_HH
#define DAY_6_WINDOW_HH

#include <SFML/Graphics.hpp>

#include "state.hh"

namespace Day6 {

constexpr auto DEFAULT_FRAMERATE = 16U;

class Window {
  const State* state_;
  sf::RenderWindow window_;

  sf::CircleShape guard_;
  sf::CircleShape candidate_;

  sf::RectangleShape block_;
  sf::RectangleShape visited_;

  sf::Font font_{};
  sf::Text text_{};

  uint32_t frame_limit_{DEFAULT_FRAMERATE};

 public:
  explicit Window(const State* state);

  auto isOpen() const -> bool;

  void handleEvents();
  void draw();
};

}  // namespace Day6

#endif  // DAY_6_WINDOW_HH
