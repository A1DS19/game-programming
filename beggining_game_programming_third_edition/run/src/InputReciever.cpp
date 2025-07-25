#include "InputReciever.h"

void InputReciever::AddEvent(const sf::Event &event) noexcept {
  mEvents.push_back(event);
}

void InputReciever::ClearEvents() noexcept { mEvents.clear(); }
