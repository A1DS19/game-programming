#include "PlayerUpdate.h"
#include "LevelUpdate.h"

void PlayerUpdate::Assemble(std::shared_ptr<LevelUpdate> levelUpdate,
                            std::shared_ptr<PlayerUpdate> playerUpdate) {
  mPosition.size.x = PLAYER_WIDTH * 10;
  mPosition.size.y = PLAYER_HEIGHT * 10;
  mIsPaused = levelUpdate->GetIsGamePaused();
}

void PlayerUpdate::HandleInput() noexcept { mInputReciever.ClearEvents(); }

void PlayerUpdate::Updatefn(float timeTakenFrom) { HandleInput(); }
