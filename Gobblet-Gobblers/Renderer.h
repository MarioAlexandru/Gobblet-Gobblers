#pragma once
#include <SFML/Graphics.hpp>
#include "GameDefs.h"

void drawGame(sf::RenderWindow& window, const GameState& state, sf::Text& text);
void drawMenu(sf::RenderWindow& window, sf::Font& font);