#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameDefs.h"

void simpleGradient(sf::RenderWindow& window, sf::Color color1, sf::Color color2, sf::IntRect rectangle);
void displayFPS(RenderWindow& window, Font& font, float fpsCounter);

void drawBackground(sf::RenderWindow & window, int offsetx, int offsety);

void drawGame(sf::RenderWindow& window, const GameState& state, sf::Text& text, sf::Font& font);
void drawPauseMenu(RenderWindow& window, GameState state, Text& text, Font& font);
void drawWinMenu(RenderWindow& window, GameState state, Text& text, Font& font);

void drawMenu(sf::RenderWindow& window,GameState state, sf::Font& font);
void drawSelectGameModeMenu(sf::RenderWindow& window, GameState state, sf::Font& font);
void drawSelectDifficultyMenu(RenderWindow& window, GameState state, Font& font);
void drawCustomizationMenu(RenderWindow& window, GameState& state, arrowSet arrows[], Text& text, Font& font);
void drawSaveToLeaderboard(RenderWindow& window, GameState& state, Text& text, Font font, String& textString);

void drawStyledButton(RenderWindow& window, Font& font, const ButtonConfig& cfg);


