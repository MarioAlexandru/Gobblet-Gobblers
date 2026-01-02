#include "Input.h"
#include "GameLogic.h"
#include "Renderer.h"
#include <iostream>
#include <string>

using namespace sf;
using namespace std;

FloatRect getButtonRect(const RenderWindow& window,float xPerc, float yPerc, float sizePerc, float estimatedWidthPerc) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = std::min(winW, winH);

    float height = scale * sizePerc * 1.5f;
    float width = scale * estimatedWidthPerc;

    float xPos = winW * xPerc;
    float yPos = winH * yPerc;

    return FloatRect(
        { xPos - width / 2.0f, yPos - height / 2.0f },
        { width, height }
    );
}

FloatRect getTextBoxRect(const RenderWindow& window, Vector2f pos, Vector2f size) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = std::min(winW, winH);

    float height = winH * size.y;
    float width = winW * size.x;

    float xPos = winW * pos.x;
    float yPos = winH * pos.y;

    return FloatRect(
        { xPos - width / 2.0f, yPos - height / 2.0f },
        { width, height }
    );
}

void handleMenuInput(RenderWindow& window, GameState& state, const Event& event, TextBox textBox[]) {
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            FloatRect btnPlayRect = getButtonRect(window, 0.5f, 0.45f, 0.05f, 0.4f);
            FloatRect btnLoadRect = getButtonRect(window, 0.5f, 0.60f, 0.05f, 0.4f);
            FloatRect btnExitRect = getButtonRect(window, 0.5f, 0.75f, 0.05f, 0.2f);
            FloatRect btnCustomizeRect = getButtonRect(window, 0.5f, 0.9f, 0.06f, 0.5f);

            if (btnPlayRect.contains(mouseF)) {
                state.appState = STATE_SELECT_MODE;
            }
            else if (btnLoadRect.contains(mouseF))
            {
                initGame(state);
                if (!(loadGameState(state, "save.txt"))) {
                    printf("Failed to load game correctly");
                }
                else {
                    state.appState = STATE_GAME;
                    printf("Last game has been succesfully loaded");
                }
            }
            else if (btnExitRect.contains(mouseF)) {
                window.close();
            }
            else if (btnCustomizeRect.contains(mouseF)) {
                initCustomize(textBox);
                state.appState = STATE_CUSTOMIZATION;
            }
        }
    }
}

void handleSelectGameModeMenuInput(RenderWindow& window, GameState& state, const Event& event)
{
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            FloatRect btnPvpRect = getButtonRect(window, 0.5f, 0.40f, 0.05f, 0.4f);
            FloatRect btnPveRect = getButtonRect(window, 0.5f, 0.60f, 0.05f, 0.4f);

            if (btnPvpRect.contains(mouseF)) {
                initGame(state);
                state.appState = STATE_GAME;
                state.gameMode = PVP;
            }
            else if (btnPveRect.contains(mouseF))
            {
                state.appState = STATE_SELECT_DIFFICULTY;
            }
        }
    }
}

void handleSelectDifficultyMenuInput(RenderWindow& window, GameState& state, const Event& event)
{
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            FloatRect btnRandRect = getButtonRect(window, 0.5f, 0.40f, 0.05f, 0.4f);
            FloatRect btnStratRect = getButtonRect(window, 0.5f, 0.60f, 0.05f, 0.4f);

            if (btnRandRect.contains(mouseF)) {
                initGame(state);
                state.appState = STATE_GAME;
                state.gameMode = PVE_RAND;
            }
            else if (btnStratRect.contains(mouseF))
            {
                initGame(state);
                state.appState = STATE_GAME;
                state.gameMode = PVE_MIMIMAX;
            }
        }
    }
}

void handleCustomizationMenuInput(RenderWindow& window, GameState& state, const Event& event, String& inputBuffer, TextBox textBox[]) {
    if (textBox[0].isEmpty && !textBox[0].Focused) {
        inputBuffer = "your name...";
    }
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            Vector2f size = textBox[0].size;
            Vector2f pos = textBox[0].pos;
            FloatRect textBoxP1 = { {pos.x-size.x/2.f,pos.y},size};
            FloatRect doneButton = getButtonRect(window, 0.25f, 0.95f, 0.04f, 0.4f);

            if (textBoxP1.contains(mouseF) && !textBox[0].Focused) {
                textBox[0].isEmpty = false;
                inputBuffer.clear();
                textBox[0].Focused = true;
            }
            else if (!textBoxP1.contains(mouseF) && textBox[0].Focused){
                textBox[0].Focused = false;
                if (inputBuffer.isEmpty()) {
                    textBox[0].isEmpty = true;
                }
            }
            else if (doneButton.contains(mouseF) && !textBox[0].isEmpty) {
                state.name[P1].insert(0, inputBuffer);
                cout << "String '" << state.name[P1].toAnsiString() << "' succesfully copied to gameState name of player "<<P1+1<<  endl;
                state.appState = STATE_MENU;
            }
        }
    }
    else if (const auto* textEntered = event.getIf<Event::TextEntered>())
    {
        if (textBox[0].Focused) {
            if (textEntered->unicode > 32 && textEntered->unicode < 127 && inputBuffer.getSize()<18) {
                inputBuffer += static_cast<char>(textEntered->unicode);
            }
            else if (textEntered->unicode == 8 && inputBuffer.getSize() >= 1)
                inputBuffer.erase(inputBuffer.getSize() - 1, 1);
        }
    }
    else if (const auto* key = event.getIf<Event::KeyReleased>()) {
        if (key->scancode == Keyboard::Scancode::Enter) {
            textBox[0].Focused = false;
            if (inputBuffer.isEmpty()) {
                textBox[0].isEmpty = true;
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
            togglePause(state);
        }
        if (state.matchState == STATE_PLAY) {
            if (key->scancode == Keyboard::Scancode::Up && pieceSize < 3) pieceSize++;
            else if (key->scancode == Keyboard::Scancode::Down && pieceSize > 1) pieceSize--;
        }
    }

    if (const auto* buttonReleased = event.getIf<Event::MouseButtonReleased>()) {
        Vector2i mPos = Mouse::getPosition(window);
        Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

        int line = static_cast<int>((mPos.y - boardY) / latura);
        int col = static_cast<int>((mPos.x - boardX) / latura);

        if (state.matchState == STATE_PLAY) {
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
                        if (movePiece(state, old_line, old_col, line, col)) {
                            printf("Current state has a value of: %d, for player %d\n", evaluate(state, player), player + 1);
                            checkWin(state,true);
                            player = 1 - player;
                            if (state.gameMode == PVE_RAND && state.matchState == STATE_PLAY) {
                                makeRandomMove(state);
                                printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state,true);
                                player = 1 - player;
                            }
                            else if (state.gameMode == PVE_MIMIMAX && state.matchState == STATE_PLAY) {
                                executeMove(state,chooseBestMove(state,3));
                                printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state, true);
                                player = 1 - player;
                            }
                        }
                    }
                    else if (!waitingForLeftClick) {
                        int val = (player == P1) ? pieceSize : -1 * pieceSize;
                        if (punePiesa(state, line + 1, col + 1, val, false)) {
                            printf("Current state has a value of: %d, for player %d\n", evaluate(state, player), player + 1);
                            checkWin(state,true);
                            player = 1 - player;
                            if (state.gameMode == PVE_RAND && state.matchState == STATE_PLAY) {
                                makeRandomMove(state);
                                printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state,true);
                                player = 1 - player;
                            }
                            else if (state.gameMode == PVE_MIMIMAX && state.matchState == STATE_PLAY) {
                                executeMove(state, chooseBestMove(state, 3));
                                printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state, true);
                                player = 1 - player;
                            }
                        }
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
        else if (state.matchState == STATE_PAUSED) { //pauseMenu
            FloatRect btnResumeRect = getButtonRect(window, 0.5f, 0.18f, 0.04f, 0.42f);
            FloatRect btnSaveRect = getButtonRect(window, 0.5f, 0.78f, 0.04f, 0.35f);
            FloatRect btnExitRect = getButtonRect(window, 0.5f, 0.88f, 0.04f, 0.3f);

            if (buttonReleased->button == Mouse::Button::Left) {
                if (btnSaveRect.contains(mouseF)) {
                    saveGameState(state, "save.txt");
                    state.appState = STATE_MENU;
                }
                else if(btnResumeRect.contains(mouseF)) {
                    togglePause(state);
                }
                else if (btnExitRect.contains(mouseF)) {
                    window.close();
                }
            }
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

void handleInput(RenderWindow& window, GameState& state, String& inputBuffer, TextBox textBox[]) {
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
            {    
                handleMenuInput(window, state, *event, textBox);
                break;
            }
            case STATE_GAME:
            {
                handleGameInput(window, state, *event);
                break;
            }
            case STATE_SELECT_MODE:
            {
                handleSelectGameModeMenuInput(window, state, *event);
                break;
            }
            case STATE_SELECT_DIFFICULTY: 
            {
                handleSelectDifficultyMenuInput(window, state, *event);
                break;
            }
            case STATE_CUSTOMIZATION:
            {
                handleCustomizationMenuInput(window, state, *event, inputBuffer, textBox);
                break;
            }
        }
    }
}