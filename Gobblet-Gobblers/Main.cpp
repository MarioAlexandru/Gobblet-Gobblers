#include <SFML/Graphics.hpp>
#include "GameDefs.h"
#include "GameLogic.h"
#include "Input.h"
#include "Renderer.h"
#include "PieceStack.h"

using namespace sf;
using namespace std;

int main()
{
    RenderWindow window(VideoMode({ 1000, 1000 }), "Gobblet Gobblers", Style::Default);
    //window.setFramerateLimit(60);

    GameState myGame;
    initGame(myGame);
    myGame.appState = STATE_MENU;

    arrowSet arrows[8];
    Font font;
    if (!font.openFromFile("Minecraftia-Regular.ttf")) return -1;

    Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::Red);

    // Scrolling parameters
    float scrollSpeed = 30.f;  // pixels per second
    float offsetX = 0;
    const int tileSize = 164;

    Texture texture;

    if (!texture.loadFromFile("medium-idle.png"))
    {
        return -1;
    }

    String temporary;
    
    Character player1Test;

    player1Test.bodyType = 0; 
    player1Test.accessory = 0;
    player1Test.size = 2;     

    player1Test.palette[0] = sf::Color::Red;

    player1Test.palette[1] = sf::Color::Blue;
    player1Test.palette[2] = sf::Color::Green;

    player1Test.bodyColor = 2;

    myGame.stack = std::make_shared<PieceStack>(font, texture, sf::Vector2f(700.f, 300.f), 1, 32, player1Test);
    myGame.stack->bindToWindow(window);
    myGame.stack->setRelativePosition({ 50.f,30.f });

    Clock clock;

    while (window.isOpen())
    {
        float dt = clock.restart().asSeconds();
        float fps = 1.0f / dt;

        handleInput(window, myGame, arrows, temporary);

        window.clear();

        offsetX += scrollSpeed * dt;  // wrap at 82

        offsetX = fmod(offsetX, static_cast<float>(tileSize));
        if (offsetX < 0) offsetX += tileSize;

        int tx = static_cast<int>(-offsetX) % tileSize;
        if (tx < 0) tx += tileSize; // ensure positive modulo
        int ty = offsetX;

        drawBackground(window, tx, ty);

        myGame.stack->update(dt);

        switch (myGame.appState) 
        {
            case STATE_MENU:
            {
                //window.draw(*piece);
                window.draw(*myGame.stack);
                drawMenu(window, myGame, font);
                break;
            }
            case STATE_SELECT_MODE:
            {
                drawSelectGameModeMenu(window, myGame, font);
                break;
            }
            case STATE_SELECT_DIFFICULTY:
            {
                drawSelectDifficultyMenu(window, myGame, font);
                break;
            }
            case STATE_GAME:
            {
                drawGame(window, myGame, text, font);
                if (myGame.matchState == STATE_PLAY) {
                    //nothing
                }
                else if (myGame.matchState == STATE_PAUSED) {
                    drawPauseMenu(window, myGame, text, font);
                }
                else if (myGame.matchState == STATE_WIN) {
                    drawWinMenu(window, myGame, text, font);
                }
                break;
            }
            case STATE_CUSTOMIZATION:
            {
                drawCustomizationMenu(window, myGame, arrows, text, font);
                break;
            }
            case STATE_SAVE_HIGHSCORE: 
            {
                drawSaveToLeaderboard(window, myGame, text, font, temporary);
                break;
            }
        }
        //displayFPS(window, font, fps);
        window.display();
    }

    return 0;
}