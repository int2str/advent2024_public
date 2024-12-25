#ifndef DAY_14_WINDOW_HH
#define DAY_14_WINDOW_HH

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/System/Vector2.hpp>

#include "robots.hh"
#include "utils/coordinate.hh"

namespace Day14 {

class Window {
  Utils::Coordinate max_;

  sf::Vector2f tile_;
  sf::RenderWindow window_;

  sf::RectangleShape robot_;

  sf::Font font_{};
  sf::Text text_{};

  sf::VertexArray lines_{sf::Lines};

  [[nodiscard]] auto robotAt(const Robot& robot) const -> sf::Vector2f;

 public:
  explicit Window(Utils::Coordinate max);

  auto isOpen() const -> bool;

  void handleEvents();
  void draw(const Robots& anomaly, size_t anomaly_at, double distance,
            size_t steps);
};

}  // namespace Day14

#endif  // DAY_14_WINDOW_HH
