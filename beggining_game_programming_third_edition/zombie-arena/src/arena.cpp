#include "arena.h"
#include <cstdlib>
#include <ctime>

int createBackground(sf::VertexArray &va, sf::IntRect &arena) {
  const int TILE_SIZE = 50;
  const int TILE_TYPES = 3;
  const int VERTS_PER_QUAD = 6; // Two triangles per quad

  // Compute world dimensions using arena.size
  int worldWidth =
      arena.size.x /
      TILE_SIZE; // .size.x is width :contentReference[oaicite:6]{index=6}
  int worldHeight =
      arena.size.y /
      TILE_SIZE; // .size.y is height :contentReference[oaicite:7]{index=7}

  va.setPrimitiveType(
      sf::PrimitiveType::Triangles); // Quads removed in SFML 3
                                     // :contentReference[oaicite:8]{index=8}
  va.resize(worldWidth * worldHeight * VERTS_PER_QUAD);

  std::srand(static_cast<unsigned>(std::time(nullptr))); // Seed once

  int currentVertex = 0;
  for (int w = 0; w < worldWidth; ++w) {
    for (int h = 0; h < worldHeight; ++h) {
      // Calculate positions of quad corners
      sf::Vector2f topLeft = {float(w * TILE_SIZE), float(h * TILE_SIZE)};

      sf::Vector2f topRight = {float((w + 1) * TILE_SIZE),
                               float(h * TILE_SIZE)};

      sf::Vector2f bottomRight = {float((w + 1) * TILE_SIZE),
                                  float((h + 1) * TILE_SIZE)};

      sf::Vector2f bottomLeft = {float(w * TILE_SIZE),
                                 float((h + 1) * TILE_SIZE)};

      // Triangle 1
      va[currentVertex + 0].position = topLeft;
      va[currentVertex + 1].position = topRight;
      va[currentVertex + 2].position = bottomRight;
      // Triangle 2
      va[currentVertex + 3].position = bottomRight;
      va[currentVertex + 4].position = bottomLeft;
      va[currentVertex + 5].position = topLeft;

      // Choose texture row: wall vs floor
      int tileRow =
          (h == 0 || h == worldHeight - 1 || w == 0 || w == worldWidth - 1)
              ? TILE_TYPES - 1
              : (std::rand() % TILE_TYPES);

      float texY = float(tileRow * TILE_SIZE);

      // Assign corresponding texture coordinates
      va[currentVertex + 0].texCoords = {0.f, texY};
      va[currentVertex + 1].texCoords = {float(TILE_SIZE), texY};
      va[currentVertex + 2].texCoords = {float(TILE_SIZE),
                                         float(TILE_SIZE) + texY};
      va[currentVertex + 3].texCoords = {float(TILE_SIZE),
                                         float(TILE_SIZE) + texY};
      va[currentVertex + 4].texCoords = {0.f, float(TILE_SIZE) + texY};
      va[currentVertex + 5].texCoords = {0.f, texY};

      currentVertex += VERTS_PER_QUAD;
    }
  }

  return TILE_SIZE;
}
