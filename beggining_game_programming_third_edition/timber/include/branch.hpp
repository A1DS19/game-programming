#pragma once

#include "SFML/Graphics.hpp"
#include "game.hpp"
#include <array>

struct Branch {
  Branch();
  void UpdateBranch(int seed);

  Branch(Branch &&) noexcept = default;            // default move‐ctor
  Branch &operator=(Branch &&) noexcept = default; // default move‐assign

  // disable copying
  Branch(Branch const &) = delete;
  Branch &operator=(Branch const &) = delete;

  std::unique_ptr<sf::Texture> texture;
  std::unique_ptr<sf::Sprite> sprite;
  float height;
};

namespace Branches {
namespace Constants {
constexpr unsigned int MAX = 6;
};

void CreateBranches();
void UpdateBranches(int seed);
extern std::array<Game::Side, Branches::Constants::MAX> branchPositions;
extern std::array<Branch, Branches::Constants::MAX> branches;
}; // namespace Branches
