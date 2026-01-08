#include <SFML/Graphics.hpp>
#include "GameDefs.h"
#include "GameLogic.h"
#include "Input.h"
#include "Renderer.h"

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

    String temporary;

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
        

        switch (myGame.appState) 
        {
            case STATE_MENU:
            {
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