#pragma once

#include "Actor.hpp"

class Game;
class CircleComponent;

class Asteroid: public Actor{
  public:
    Asteroid(Game *game);
    ~Asteroid();

    CircleComponent *GetCircle(){return mCircle;}

  private:
    CircleComponent* mCircle;
};
