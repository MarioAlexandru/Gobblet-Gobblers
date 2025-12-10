#pragma once
#include <SFML/Graphics.hpp>
#include "GameDefs.h"

// Draws the board, the pieces, the highlight, AND the text
void drawGame(sf::RenderWindow& window, const GameState& state, sf::Text& text);