#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameDefs.h"

void handleInput(RenderWindow& window, GameState& state, arrowSet arrows[], std::string& inputBuffer);
void handleResize(sf::RenderWindow& window, const Event::Resized& resizeEvent,GameState& State);