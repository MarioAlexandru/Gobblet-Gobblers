#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "GameDefs.h"

void simpleGradient(sf::RenderWindow& window, sf::Color color1, sf::Color color2, sf::IntRect rectangle);
void displayFPS(RenderWindow& window, Font& font, float fpsCounter);

void drawMainBG(RenderWindow& window, Sprite background, Sprite title, Sprite detail1, Sprite detail2);
void drawScrollBG(RenderWindow& window, int offsetx, int offsety);

void drawGame(sf::RenderWindow& window, GameState& state, sf::Text& text, sf::Font& font);
void drawPauseMenu(RenderWindow& window, GameState state, Text& text, Font& font);
void drawWinMenu(RenderWindow& window, GameState state, Text& text, Font& font);

void drawMenu(RenderWindow& window, GameState state, Font& font);
void drawSelectGameModeMenu(sf::RenderWindow& window, GameState state, sf::Font& font);
void drawSelectDifficultyMenu(RenderWindow& window, GameState state, Font& font);
void drawCustomizationMenu(RenderWindow& window, GameState& state, arrowSet arrows[], Text& text, Font& font);
void drawSaveToLeaderboard(RenderWindow& window, GameState& state, Text& text, Font font, String& textString);

FloatRect drawCharacter(RenderWindow& window, float posX, float posY, float scaleFactor, int pieceSize, Character character, bool draw, bool unavailable);
void drawStyledButton(RenderWindow& window, Font& font, const ButtonConfig& cfg);


