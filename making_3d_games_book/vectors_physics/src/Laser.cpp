#include "Laser.hpp"

#include "Actor.hpp"
#include "Asteroid.hpp"
#include "CircleComponent.hpp"
#include "Game.hpp"
#include "MoveComponent.hpp"
#include "SpriteComponent.hpp"

Laser::Laser(Game *game) : Actor(game), mDeathTimer(1.0f) {
  // Game sprite
  SpriteComponent *sc = new SpriteComponent(this);
  sc->SetTexture(game->GetTexture("../assets/Laser.png"));

  // Movement
  MoveComponent *mc = new MoveComponent(this);
  mc->SetForwardSpeed(800.0f);

  // Collision
  mCircle = new CircleComponent(this);
  mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltaTime) {
  mDeathTimer -= deltaTime;
  if (mDeathTimer <= 0.0f) {
    SetState(EDead);
  } else {
    // Do we intersect with asteroid?
    for (auto ast : GetGame()->GetAsteroids()) {
      if (Intersect(*mCircle, *(ast->GetCircle()))) {
        // On intersect set laser and asteroid to EDead
        SetState(EDead);
        ast->SetState(EDead);
        break;
      }
    }
  }
}
