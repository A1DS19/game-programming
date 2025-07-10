#pragma once

#include "SFML/Graphics.hpp"

namespace Game {
constexpr auto screenDims = sf::Vector2u{1920, 1080};
enum class Direction { Left, Right, None };
} // namespace Game
