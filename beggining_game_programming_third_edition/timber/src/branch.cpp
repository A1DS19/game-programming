#include "branch.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

Branch::Branch() : texture(std::make_unique<sf::Texture>()) {
  if (!texture->loadFromFile("../assets/graphics/branch.png")) {
    std::cerr << "could not load branch texture" << std::endl;
  }

  sprite = std::make_unique<sf::Sprite>(*texture);
  sprite->setPosition({-2000, -2000});
  sprite->setPosition({20, 20});
}

namespace Branches {
std::array<Branch::Side, Constants::MAX> branchPositions{};
std::array<Branch, Constants::MAX> branches{};

void CreateBranches() {
  for (size_t i = 0; i < Constants::MAX; i++) {
    branches[i] = Branch{};
  }
}

void UpdateBranches(int seed) {
  for (int i = Branches::Constants::MAX - 1; i > 0; i--) {
    Branches::branchPositions[i] = Branches::branchPositions[i - 1];
  }

  // spawn a new branch at position 0
  srand((int)time(0) + seed);
  int r = (rand() % 5);

  switch (r) {
  case 0:
    branchPositions[0] = Branch::Side::LEFT;
    break;
  case 1:
    branchPositions[0] = Branch::Side::RIGHT;
    break;
  default:
    branchPositions[0] = Branch::Side::NONE;
    break;
  }
}
} // namespace Branches
