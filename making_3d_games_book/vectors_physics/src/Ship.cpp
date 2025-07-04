#include "Ship.hpp"

#include "Actor.hpp"
#include "Game.hpp"
#include "InputComponent.hpp"
#include "Laser.hpp"
#include "SpriteComponent.hpp"

Ship::Ship(Game *game) : Actor(game), mLaserCooldown(0.0f) {
  SpriteComponent *sc = new SpriteComponent(this, 150);
  sc->SetTexture(game->GetTexture("../assets/Ship.png"));

  InputComponent *ic = new InputComponent(this);
  ic->SetForwardKey(SDL_SCANCODE_W);
  ic->SetBackKey(SDL_SCANCODE_S);
  ic->SetClockwiseKey(SDL_SCANCODE_A);
  ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
  ic->SetMaxForwardSpeed(300.0f);
  ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Ship::UpdateActor(float deltaTime) { mLaserCooldown -= deltaTime; }

void Ship::ActorInput(const uint8_t *keyState) {
  if (keyState[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f) {
    // Create a laser and set its position / rotation to mine
    Laser *laser = new Laser(GetGame());
    laser->SetPosition(GetPosition());
    laser->SetRotation(GetRotation());
    // Reset laser cooldown(half second)
    mLaserCooldown = 0.5f;
  }
}
