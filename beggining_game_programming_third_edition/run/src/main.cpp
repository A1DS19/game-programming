#include "main.h"
#include "Factory.h"
#include "GameObject.h"
#include "InputDispatcher.h"
#include "SFML/Graphics.hpp"
#include <vector>

int main() {
  const sf::Color BACKGROUND_COLOR(100, 100, 100, 255);
  sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode()),
                          "Run!");

  sf::VertexArray canvas(sf::PrimitiveType::Triangles, 0);

  // This can dispatch events to any object.
  InputDispatcher inputDispatcher(window);

  // Everything will be a game object.
  // This vector will hold them all.
  std::vector<GameObject> gameObjects;

  // This class has all the knowledge
  // to construct game objects that do
  // many different things.
  Factory factory(&window);

  // This call will send the vector of game objects
  // the canvas to draw on and the input dispatcher
  // to the factory to set up the game.
  factory.LoadLevel(gameObjects, canvas, inputDispatcher);

  // A clock for timing.
  sf::Clock clock;

  while (window.isOpen()) {
    // Measure the time taken this frame.
    float timeTakenInSeconds = clock.restart().asSeconds();

    // Handle the player input.
    inputDispatcher.DispatchInputEvents();

    // Clear the previous frame.
    window.clear(BACKGROUND_COLOR);

    // Update all the game objects.
    for (auto &gameObject : gameObjects) {
      gameObject.Updatefn(timeTakenInSeconds);
    }

    // Draw all the game objects to the canvas.
    for (auto &gameObject : gameObjects) {
      gameObject.Draw(canvas);
    }

    // Show the new frame.
    window.display();
  }

  return EXIT_SUCCESS;
}
