#pragma once

#include "Component.h"
#include <memory>
class LevelUpdate;
class PlayerUpdate;
class Update : public Component {
public:
  Update();
  virtual void Assemble(std::shared_ptr<LevelUpdate> levelUpdate,
                        std::shared_ptr<PlayerUpdate> playerUpdate) = 0;
  virtual void Updatefn(float timeSinceLastUpdate) = 0;
};
