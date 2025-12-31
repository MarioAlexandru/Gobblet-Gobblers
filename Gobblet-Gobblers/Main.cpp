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
    window.setFramerateLimit(60);

    Shader gradient;
    if (!gradient.loadFromFile("multiGradient.frag", sf::Shader::Type::Fragment))
    {
        return -1;
    }

    GameState myGame;
    TextBox tb[2];//for listening to keyboard input / when typing plr name
    initGame(myGame);
    myGame.appState = STATE_MENU;
    int winner;

    Font font;
    if (!font.openFromFile("Minecraftia-Regular.ttf")) return -1;

    Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::Red);

    // Scrolling parameters
    int scrollSpeedFrames = 3;  // shift every 3 frames ¨ slower scroll
    int frameCounter = 0;
    int offsetX = 0;
    const int tileSize = 164;

    String temporary;

    while (window.isOpen())
    {
        handleInput(window, myGame, temporary, tb);

        window.clear();

        if (++frameCounter >= scrollSpeedFrames)
        {
            frameCounter = 0;
            offsetX = (offsetX + 1) % tileSize;  // wrap at 82
        }

        int tx = (-offsetX) % tileSize;
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
                    //nothing rn
                }
                break;
            }
            case STATE_CUSTOMIZATION:
            {
                drawCustomizationMenu(window, text, font, temporary, tb);
                break;
            }
        }
        window.display();
    }

    return 0;
}