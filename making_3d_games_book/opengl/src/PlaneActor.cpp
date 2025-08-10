#include "PlaneActor.hpp"
#include "Game.hpp"
#include "MeshComponent.hpp"
#include "Renderer.hpp"

PlaneActor::PlaneActor(Game *game) : Actor(game) {
  SetScale(10.0f);
  MeshComponent *mc = new MeshComponent(this);
  auto *mesh = GetGame()->GetRenderer()->GetMesh("../assets/Plane.gpmesh");
  mc->SetMesh(mesh);
}
