#include "PlayerGraphics.h"
#include "PlayerUpdate.h"

void PlayerGraphics::Assemble(sf::VertexArray &canvas,
                              std::shared_ptr<Update> genericUpdate,
                              sf::IntRect &textCoords) {
  mPlayerUpdate = static_pointer_cast<PlayerUpdate>(genericUpdate);
  mPosition = mPlayerUpdate->GetPositionPointer();

  mVertexStartIndex = canvas.getVertexCount();
  canvas.resize(canvas.getVertexCount() + 4);

  canvas[mVertexStartIndex].texCoords.x = textCoords.position.x;
  canvas[mVertexStartIndex].texCoords.y = textCoords.position.y;
  canvas[mVertexStartIndex + 1].texCoords.x =
      textCoords.position.x + textCoords.position.x;
  canvas[mVertexStartIndex + 1].texCoords.y = textCoords.position.y;
  canvas[mVertexStartIndex + 2].texCoords.x =
      textCoords.position.x + textCoords.position.x;
  canvas[mVertexStartIndex + 2].texCoords.y =
      textCoords.position.y + textCoords.position.y;
  canvas[mVertexStartIndex + 3].texCoords.x = textCoords.position.x;
  canvas[mVertexStartIndex + 3].texCoords.y =
      textCoords.position.y + textCoords.position.y;
}
