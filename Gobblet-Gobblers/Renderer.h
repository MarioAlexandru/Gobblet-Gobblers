#pragma once
#include <SFML/Graphics.hpp>
#include "GameDefs.h"

void drawGame(sf::RenderWindow& window, const GameState& state, sf::Text& text, sf::Font& font);
void drawMenu(sf::RenderWindow& window,GameState state, sf::Font& font);