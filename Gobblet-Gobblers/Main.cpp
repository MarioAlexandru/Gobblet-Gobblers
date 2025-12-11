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

    Font font;
    if (!font.openFromFile("Minecraftia-Regular.ttf")) return -1;

    Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(Color::Red);

    while (window.isOpen())
    {
        handleInput(window, myGame);

        if (checkWin(myGame)) {
            printf("Player %d Wins!\n", abs(myGame.player-1)+1);
            window.close();
        }

        window.clear();
        drawGame(window, myGame, text);
        window.display();
    }

    return 0;
}