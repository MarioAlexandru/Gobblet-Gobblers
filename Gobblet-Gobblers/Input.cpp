#include "Input.h"
#include "GameLogic.h"
#include <iostream>

using namespace sf;
using namespace std;

void handleInput(RenderWindow& window, GameState& state) {
    auto& T = state.T;
    auto& player = state.player;
    auto& pieceSize = state.pieceSize;
    auto& waitingForLeftClick = state.waitingForLeftClick;
    auto& correctSelection = state.correctSelection;
    auto& old_line = state.old_line;
    auto& old_col = state.old_col;


    int line = -2, col = -2;
    float boardX = (static_cast<float>(window.getSize().x) - tableSize) / 2.f;
    float boardY = (static_cast<float>(window.getSize().y) - tableSize) / 2.f;

    while (const optional event = window.pollEvent())
    {
        if (event->is<Event::Closed>())
        {
            window.close();
        }
        else if (const auto* keyPressed = event->getIf<Event::KeyPressed>())
        {
            if (keyPressed->scancode == Keyboard::Scancode::Escape)
                window.close();
        }
        else if (const auto* keyReleased = event->getIf<Event::KeyReleased>()) {
            /*if (keyReleased->scancode == Keyboard::Scancode::Space) {
                if (player == P1)
                    player = P2;
                else
                    player = P1;
            }*/
            if ((keyReleased->scancode == Keyboard::Scancode::Up) && pieceSize < 3)
                pieceSize += 1;
            else if ((keyReleased->scancode == Keyboard::Scancode::Down) && pieceSize > 1)
                pieceSize -= 1;
        }
        else if (const auto* buttonReleased = event->getIf<Event::MouseButtonReleased>()) {
            if (buttonReleased->button == Mouse::Button::Right && !waitingForLeftClick) {
                printf("Right-click detected.\n");

                int latura = tableSize / squareNumber;
                Vector2i mPos = Mouse::getPosition(window);

                line = static_cast<int>((mPos.y - boardY) / latura);
                col = static_cast<int>((mPos.x - boardX) / latura);

                int varf = T[line + 1][col + 1].nr;

                if (sign(T[line + 1][col + 1].p[varf]) == 1 && player == P1 || sign(T[line + 1][col + 1].p[varf]) == -1 && player == P2) {
                    printf("Piece correctly selected");
                    old_line = line;
                    old_col = col;
                    correctSelection = true;
                }
                waitingForLeftClick = true;
            }
            else if (buttonReleased->button == Mouse::Button::Left && waitingForLeftClick) {
                int latura = tableSize / squareNumber;
                Vector2i mPos = Mouse::getPosition(window);
                line = static_cast<int>((mPos.y - boardY) / latura);
                col = static_cast<int>((mPos.x - boardX) / latura);

                int varf = T[line][col].nr;
                if (correctSelection) {
                    printf("Trying to move piece from line %d, col %d, to new position: %d %d", old_line, old_col, line, col);

                    movePiece(state, old_line, old_col, line, col);
                }
                old_line = -20;
                old_col = -20;
                waitingForLeftClick = false;
                correctSelection = false;
            }
            else if (buttonReleased->button == Mouse::Button::Right) {
                printf("Right click input detected.Cancelling wait.\n");
                waitingForLeftClick = false;
                correctSelection = false;
                old_line = -20;
                old_col = -20;
            }

            else if (buttonReleased->button == Mouse::Button::Left) {
                old_line = -20;
                old_col = -20;
                int latura = tableSize / squareNumber;
                Vector2i mPos = Mouse::getPosition(window);
                line = static_cast<int>((mPos.y - boardY) / latura + 1);
                col = static_cast<int>((mPos.x - boardX) / latura + 1);

                if (boardX < mPos.x && mPos.x < boardX + tableSize && boardY < mPos.y && mPos.y < boardY + tableSize)
                    if (player == P1) {
                        punePiesa(state, line, col, pieceSize);
                    }
                    else
                    {
                        punePiesa(state, line, col, -1 * pieceSize);
                    }
            }

        }
    }
}