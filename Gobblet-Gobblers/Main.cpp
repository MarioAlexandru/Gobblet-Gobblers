#include <SFML/Graphics.hpp>
#include "GameDefs.h"
#include "GameLogic.h"
#include "Input.h"
#include "Renderer.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode({ 1000, 1000 }), "Gobblet Gobblers", Style::Default);

    GameState myGame;
    initGame(myGame);
    myGame.appState = STATE_MENU;

    Font font;
    if (!font.openFromFile("Minecraftia-Regular.ttf")) return -1;

    Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::Red);

    while (window.isOpen())
    {
        handleInput(window, myGame);

        window.clear();

        if (myGame.appState == STATE_MENU) {
            drawMenu(window, font);
        }
        else if (myGame.appState == STATE_GAME)
        {
            drawGame(window, myGame, text);
            if (checkWin(myGame)) {
                printf("Player %d Wins!\n", abs(myGame.player-1) + 1);
                window.close();
            }
        }
        window.display();
    }

    return 0;
}