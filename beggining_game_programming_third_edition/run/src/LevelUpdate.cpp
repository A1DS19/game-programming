#include "LevelUpdate.h"
#include "PlayerUpdate.h"
#include "SoundEngine.h"
#include <random>

void LevelUpdate::Assemble(std::shared_ptr<LevelUpdate> levelUpdate,
                           std::shared_ptr<PlayerUpdate> playerUpdate) {
  mPlayerPosition = playerUpdate->GetPositionPointer();

  // temp
  SoundEngine::StartMusic();
}

void LevelUpdate::ConnectToCameraTime(float *cameraTime) {
  mCameraTime = cameraTime;
}

void LevelUpdate::AddPlatformPosition(sf::FloatRect *newPosition) {
  mPlatformPositions.push_back(newPosition);
  mNumberOfPlatforms++;
}

bool *LevelUpdate::GetIsGamePaused() { return &mIsPaused; }

void LevelUpdate::PositionLevelAtStart() {
  float startOffset = mPlatformPositions[0]->position.x;

  for (int i = 0; i < mNumberOfPlatforms; i++) {
    // left
    mPlatformPositions[i]->position.x = 1 * 100 + startOffset;
    // top
    mPlatformPositions[i]->position.y = 0;
    // width
    mPlatformPositions[i]->size.x = 100;
    // heigth
    mPlatformPositions[i]->size.y = 20;
  }

  // left
  mPlayerPosition->position.x =
      mPlatformPositions[mNumberOfPlatforms / 2]->position.x + 2;
  // top
  mPlayerPosition->position.y =
      mPlatformPositions[mNumberOfPlatforms / 2]->position.y - 22;

  mMoveRelativeToPlatform = mNumberOfPlatforms - 1;
  mNextPlatformToMove = 0;
}

int LevelUpdate::GetRandomNumber(int minH, int maxH) {
  // Seed the random number generator with current time
  std::random_device rd;
  std::mt19937 gen(rd());

  // Define a uniform distribution for the desired range
  std::uniform_int_distribution<int> distribution(minH, maxH);

  // Generate a random height within the specified range
  int randomHeight = distribution(gen);

  return randomHeight;
}

void LevelUpdate::Updatefn(float timeSinceLastUpdate) {
  if (!mIsPaused) {
    if (mGameOver) {
      mGameOver = false;
      *mCameraTime = 0;
      mTimeSinceLastPlatform = 0;
      PositionLevelAtStart();
    }

    *mCameraTime += timeSinceLastUpdate;
    mTimeSinceLastPlatform += timeSinceLastUpdate;

    if (mTimeSinceLastPlatform > mPlatformCreationInterval) {
      mPlatformPositions[mNextPlatformToMove]->position.y =
          mPlatformPositions[mMoveRelativeToPlatform]->position.y +
          GetRandomNumber(-40, 40);

      // How far away to create the next platform
      // Bigger gap if lower than previous
      if (mPlatformPositions[mMoveRelativeToPlatform]->position.y <
          mPlatformPositions[mNextPlatformToMove]->position.y) {
        mPlatformPositions[mNextPlatformToMove]->position.x =
            mPlatformPositions[mMoveRelativeToPlatform]->position.x +
            mPlatformPositions[mMoveRelativeToPlatform]->size.x +
            GetRandomNumber(20, 40);
      } else {
        mPlatformPositions[mNextPlatformToMove]->position.x =
            mPlatformPositions[mMoveRelativeToPlatform]->position.x +
            mPlatformPositions[mMoveRelativeToPlatform]->size.x +
            GetRandomNumber(0, 20);
      }

      mPlatformPositions[mNextPlatformToMove]->size.x =
          GetRandomNumber(20, 200);

      mPlatformPositions[mNextPlatformToMove]->size.y = GetRandomNumber(10, 20);

      // Base the time to create the next platform
      // on the width of the one just created
      mPlatformCreationInterval =
          mPlatformPositions[mNextPlatformToMove]->size.x / 90;

      mMoveRelativeToPlatform = mNextPlatformToMove;
      mNextPlatformToMove++;

      if (mNextPlatformToMove == mNumberOfPlatforms) {
        mNextPlatformToMove = 0;
      }

      mTimeSinceLastPlatform = 0;
    }

    // Has the player lagged behind the furthest back platform
    bool laggingBehind = true;
    for (auto platformPosition : mPlatformPositions) {
      if (platformPosition->position.x < mPlayerPosition->position.x) {
        laggingBehind = false;
        break; // At least one platform is behind the player
      }
    }

    if (laggingBehind) {
      mIsPaused = true;
      mGameOver = true;

      SoundEngine::PauseMusic();
    }
  }
}
