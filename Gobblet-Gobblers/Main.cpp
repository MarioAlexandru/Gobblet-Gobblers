#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "GameDefs.h"
#include "GameLogic.h"
#include "Input.h"
#include "Renderer.h"
#include "PieceStack.h"

using namespace sf;
using namespace std;

void drawText(RenderWindow& window, Text& object, string message, unsigned int fontSize, float xPerc, float yPerc, Color color) {
    int winW = static_cast<float>(window.getSize().x);
    int winH = static_cast<float>(window.getSize().y);

    object.setCharacterSize(fontSize);
    object.setString(message);
    object.setPosition({ winW * xPerc, winH * yPerc });
    object.setFillColor(color);

    window.draw(object);
}

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
    Text text2(font);
    Text text3(font);
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

    string temporary;
    
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

    Texture bgTexture("bgSimple.png");
    bgTexture.setRepeated(true);
    Sprite bgSprite(bgTexture);

    Texture titleTx("bgTitle.png");
    Sprite bgTitle(titleTx);
    bgTitle.setOrigin({ 240,0 });
    Angle titleRot = degrees(0);
    float rotSpeed = 5.f;
    int dir = 0;

    Texture detTx1("detail1.png");
    Sprite bgDet1(detTx1);
    float transparency = 255;
    float speed = 70.f;
    int dir2 = 0;

    Texture detTx2("detail2.png");
    Sprite bgDet2(detTx2);
    float offsetY = 0;
    float floatSpeed = 40.f;
    int dir1 = 0;

    Music main("main.wav");
    main.setLoopPoints({ milliseconds(500), seconds(135) });

    main.play();

    while (window.isOpen())
    {
        float winW = static_cast<float>(window.getSize().x);
        float winH = static_cast<float>(window.getSize().y);

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

        //drawBackground(window, tx, ty);

        if (dir == 0) {
            titleRot += degrees(rotSpeed * dt);
        }
        else if (dir == 1) {
            titleRot -= degrees(rotSpeed * dt);
        }

        if (titleRot >= degrees(10.f)) {
            dir = 1;
        }
        else if (titleRot <= degrees(-15.f)) {
            dir = 0;
        }
        
        bgTitle.setRotation(titleRot);
        

        if (dir1 == 0) {
            offsetY += floatSpeed * dt;
        }
        else if (dir1 == 1) {
            offsetY -= floatSpeed * dt;
        }

        if (offsetY >= winH*0.1f) {
            dir1 = 1;
        }
        else if (offsetY <= 0) {
            dir1 = 0;
        }

        bgDet2.setPosition({ 0, -offsetY });

        if (dir2 == 0) {
            transparency += speed * dt;
        }
        else if (dir2 == 1) {
            transparency -= speed * dt;
        }

        if (transparency <= 0.f) {
            dir2 = 0;
        }
        else if (transparency >= 255.f) {
            dir2 = 1;
        }

        bgDet1.setColor(Color(255, 255, 255, static_cast<int>(transparency)));

        myGame.stack->update(dt);

        switch (myGame.appState) 
        {
            case STATE_MENU:
            {
                //window.draw(*piece);
                drawMainBG(window, bgSprite, bgTitle, bgDet1, bgDet2);
                //window.draw(*myGame.stack);
                drawMenu(window, myGame, font);
                break;
            }
            case STATE_SELECT_MODE:
            {
                drawScrollBG(window, tx, ty);
                drawSelectGameModeMenu(window, myGame, font);
                break;
            }
            case STATE_SELECT_DIFFICULTY:
            {
                drawScrollBG(window, tx, ty);
                drawSelectDifficultyMenu(window, myGame, font);
                break;
            }
            case STATE_GAME:
            {
                drawScrollBG(window, tx, ty);
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
                drawScrollBG(window, tx, ty);
                drawCustomizationMenu(window, myGame, arrows, text, font);
                break;
            }
            /*case STATE_LEADERBOARD:
            {
                drawScrollBG(window, tx, ty);
                drawCustomizationMenu(window, myGame, arrows, text, font);
                break;
            }*/
            case STATE_SAVE_HIGHSCORE: 
            {
                drawScrollBG(window, tx, ty);
                //drawSaveToLeaderboard(window, myGame, text, font, temporary);
                break;
            }
        }
        //displayFPS(window, font, fps);
        window.display();
    }

    return 0;
}