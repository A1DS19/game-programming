#include "Asteroid.hpp"

#include "Actor.hpp"
#include "CircleComponent.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "MoveComponent.hpp"
#include "Random.hpp"
#include "SpriteComponent.hpp"

Asteroid::Asteroid(Game *game) : Actor(game) {
  Vector2 randPos =
      Random::GetVector(Vector2(-512.0f, -384.0f), Vector2(512.0f, 384.0f));

  SetPosition(randPos);
  SetRotation(Random::GetFloatRange(0.0f, Math::TwoPi));

  SpriteComponent *sc = new SpriteComponent(this);
  sc->SetTexture(game->GetTexture("../assets/Asteroid.png"));

  MoveComponent *mc = new MoveComponent(this);
  mc->SetForwardSpeed(150.0f);

  mCircle = new CircleComponent(this);
  mCircle->SetRadius(40.0f);

  game->AddAsteroid(this);
}

Asteroid::~Asteroid() { GetGame()->RemoveAsteroid(this); }
