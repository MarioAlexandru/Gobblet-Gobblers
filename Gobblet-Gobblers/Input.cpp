#include "Input.h"
#include "GameLogic.h"
#include <iostream>

using namespace sf;
using namespace std;

FloatRect getButtonRect(const RenderWindow& window, float yPerc, float sizePerc, float estimatedWidthPerc) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = std::min(winW, winH);

    float height = scale * sizePerc * 1.5f;
    float width = scale * estimatedWidthPerc;

    return FloatRect(
        { winW / 2.0f - width / 2.0f, (winH * yPerc) - height / 2.0f },
        { width, height }
    );
}

void handleMenuInput(RenderWindow& window, GameState& state, const Event& event) {
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            FloatRect btnPlayRect = getButtonRect(window, 0.45f, 0.05f, 0.4f);
            FloatRect btnLoadRect = getButtonRect(window, 0.60f, 0.05f, 0.4f);
            FloatRect btnExitRect = getButtonRect(window, 0.75f, 0.05f, 0.2f);

            if (btnPlayRect.contains(mouseF)) {
                initGame(state);
                state.appState = STATE_GAME;
            }
            else if (btnLoadRect.contains(mouseF))
            {
                printf("Load Game: Coming Soon!\n");
            }
            else if (btnExitRect.contains(mouseF)) {
                window.close();
            }
        }
    }
}

void handleGameInput(RenderWindow& window, GameState& state, const Event& event) {
    auto& T = state.T;
    auto& player = state.player;
    auto& pieceSize = state.pieceSize;
    auto& waitingForLeftClick = state.waitingForLeftClick;
    auto& correctSelection = state.correctSelection;
    auto& old_line = state.old_line;
    auto& old_col = state.old_col;

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float currentTableSize = std::min(winW, winH) * 0.8f;
    float boardX = (winW - currentTableSize) / 2.f;
    float boardY = (winH - currentTableSize) / 2.f;
    float latura = currentTableSize / static_cast<float>(squareNumber);

    if (const auto* key = event.getIf<Event::KeyReleased>()) {
        if (key->scancode == Keyboard::Scancode::Escape) {
            state.appState = STATE_MENU;
            return;
        }
        if (key->scancode == Keyboard::Scancode::Up && pieceSize < 3) pieceSize++;
        else if (key->scancode == Keyboard::Scancode::Down && pieceSize > 1) pieceSize--;
    }

    if (const auto* buttonReleased = event.getIf<Event::MouseButtonReleased>()) {
        Vector2i mPos = Mouse::getPosition(window);

        int line = static_cast<int>((mPos.y - boardY) / latura);
        int col = static_cast<int>((mPos.x - boardX) / latura);

        if (buttonReleased->button == Mouse::Button::Right && !waitingForLeftClick) {
            if (line >= 0 && line < squareNumber && col >= 0 && col < squareNumber) {
                int varf = T[line + 1][col + 1].nr;
                int pVal = T[line + 1][col + 1].p[varf];

                if ((sign(pVal) == 1 && player == P1) || (sign(pVal) == -1 && player == P2)) {
                    old_line = line;
                    old_col = col;
                    correctSelection = true;
                }
                waitingForLeftClick = true;
            }
        }
        else if (buttonReleased->button == Mouse::Button::Left) {
            if (line >= 0 && line < squareNumber && col >= 0 && col < squareNumber) {
                if (waitingForLeftClick && correctSelection) {
                    movePiece(state, old_line, old_col, line, col);
                }
                else if (!waitingForLeftClick) {
                    int val = (player == P1) ? pieceSize : -1 * pieceSize;
                    punePiesa(state, line + 1, col + 1, val);
                }
            }
            old_line = -20; old_col = -20;
            waitingForLeftClick = false; correctSelection = false;
        }
        else if (buttonReleased->button == Mouse::Button::Right) {
            waitingForLeftClick = false; correctSelection = false;
            old_line = -20; old_col = -20;
        }
    }
}

void handleResize(RenderWindow& window, const Event::Resized& resizeEvent) {
    FloatRect visibleArea(
        { 0.f, 0.f },
        { static_cast<float>(resizeEvent.size.x), static_cast<float>(resizeEvent.size.y) }
    );
    window.setView(View(visibleArea));
}

void handleInput(RenderWindow& window, GameState& state) {
    while (const optional event = window.pollEvent()) {
        if (event->is<Event::Closed>())
        {
            window.close();
        }
        else if(const auto* resizeEvent = event->getIf<Event::Resized>())
        {
            handleResize(window, *resizeEvent);
        }

        switch (state.appState) {
        case STATE_MENU:
            handleMenuInput(window, state, *event);
            break;
        case STATE_GAME:
            handleGameInput(window, state, *event);
            break;
        }
    }
}