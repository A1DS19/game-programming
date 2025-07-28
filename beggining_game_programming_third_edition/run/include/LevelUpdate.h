#pragma once

#include "SFML/Graphics.hpp"
#include "Update.h"
#include <vector>

class LevelUpdate : Update {
private:
  bool mIsPaused = false;
  std::vector<sf::FloatRect *> mPlatformPositions;
  float *mCameraTime = new float;
  sf::FloatRect *mPlayerPosition;
  float mPlatformCreationInterval = 0;
  float mTimeSinceLastPlatform = 0;
  int mNextPlatformToMove = 0;
  int mNumberOfPlatforms = 0;
  int mMoveRelativeToPlatform = 0;
  bool mGameOver = true;
  void PositionLevelAtStart();

public:
  void AddPlatformPosition(sf::FloatRect *newPosition);
  void ConnectToCameraTime(float *cameraTime);
  bool *GetIsGamePaused();
  int GetRandomNumber(int minH, int maxH);

  void Updatefn(float timeSinceLastUpdate) override;
  void Assemble(std::shared_ptr<LevelUpdate> levelUpdate,
                std::shared_ptr<PlayerUpdate> playerUpdate) override;
};
