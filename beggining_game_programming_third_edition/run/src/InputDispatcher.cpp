#include "InputDispatcher.h"
#include "InputReciever.h"

InputDispatcher::InputDispatcher(sf::RenderWindow &window) : mWindow(&window) {}

void InputDispatcher::DispatchInputEvents() {
  while (mWindow->isOpen()) {
    while (const std::optional event = mWindow->pollEvent()) {
      if (event->is<sf::Event::Closed>()) {
        mWindow->close();
      }

      if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
          mWindow->close();
        }
      }

      const sf::Event &e = *event;
      for (auto &ir : mInputRecievers) {
        ir->AddEvent(e);
      }
    }
  }
}

void InputDispatcher::RegisterNewInputReciever(InputReciever &ir) {
  mInputRecievers.push_back(&ir);
}
