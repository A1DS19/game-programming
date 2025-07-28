#pragma once

#include "Graphics.h"
#include "SFML/Graphics.hpp"
#include "Update.h"

class PlayerUpdate;
class PlayerGraphics : Graphics {
private:
  sf::FloatRect *mPosition = nullptr;
  int mVertexStartIndex = -999;

  // We will come back to this soon
  // Animator* m_Animator;

  sf::IntRect *mSectionToDraw = new sf::IntRect;
  sf::IntRect *mStandingStillSectionToDraw = new sf::IntRect;

  std::shared_ptr<PlayerUpdate> mPlayerUpdate;

  const int BOOST_TEX_LEFT = 536;
  const int BOOST_TEX_TOP = 0;
  const int BOOST_TEX_WIDTH = 69;
  const int BOOST_TEX_HEIGHT = 100;

  bool mLastFacingRight = true;

public:
  // From Component : Graphics
  void Assemble(sf::VertexArray &canvas, std::shared_ptr<Update> genericUpdate,
                sf::IntRect &textCoords) override;

  void Draw(sf::VertexArray &canvas) override;
};
