#pragma once

#include "SFML/Graphics.hpp"
#include "zombie.h"

using Horde = std::vector<std::unique_ptr<Zombie>>;

int createBackground(sf::VertexArray &va, sf::IntRect &arena) noexcept;
Horde createHorde(const int numZombies, sf::IntRect &arena) noexcept;
