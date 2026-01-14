#include "Input.h"
#include "GameLogic.h"
#include "PieceStack.h"
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
    float scale = std::min(winW, winH)*0.9f;

    float height = scale * size.y;
    float width = scale * size.x;

    float xPos = winW * pos.x;
    float yPos = winH * pos.y;

    return FloatRect(
        { xPos - width / 2.0f, yPos},
        { width, height }
    );
}

FloatRect getArrowRect(const RenderWindow& window, arrowSet arrow, int type) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = std::min(winW, winH) * 0.9f;

    Texture texture;
    if (arrow.type == smallA) {
        texture.loadFromFile("arrow_small.png");
    }
    else {
        texture.loadFromFile("arrow_big.png");
    }
    float dist = arrow.distPerc / 2.f;

    Sprite sprite(texture);

    if (arrow.type == bigA) {
        sprite.setOrigin({ 0.f,16.f });
    }
    else {
        sprite.setOrigin({ 0.f, 8.f });
    }
    if (type == rightA) {
        sprite.setScale({ arrow.sizePerc,arrow.sizePerc });
        sprite.setPosition({ arrow.pos.x + dist * winW,arrow.pos.y });
    }
    else if (type == leftA) {
        sprite.setPosition({ arrow.pos.x - dist * winW,arrow.pos.y });
        sprite.setScale({ -arrow.sizePerc, arrow.sizePerc });
    }

    return sprite.getGlobalBounds();
}

void handleMenuInput(RenderWindow& window, GameState& state,  const Event& event) {
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            FloatRect btnPlayRect = getButtonRect(window, 0.5f, 0.45f, 0.05f, 0.4f);
            FloatRect btnLoadRect = getButtonRect(window, 0.5f, 0.60f, 0.05f, 0.4f);
            FloatRect btnExitRect = getButtonRect(window, 0.5f, 0.75f, 0.05f, 0.2f);

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
                state.gameMode = PVP;

                defaultCustomization(state.character);
                state.appState = STATE_CUSTOMIZATION;
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
                state.gameMode = PVE_RAND;

                defaultCustomization(state.character);
                state.appState = STATE_CUSTOMIZATION;
            }
            else if (btnStratRect.contains(mouseF))
            {
                state.gameMode = PVE_MIMIMAX;

                defaultCustomization(state.character);
                state.appState = STATE_CUSTOMIZATION;
            }
        }
    }
}

void handleCustomizationMenuInput(RenderWindow& window, GameState& state, arrowSet arrows[], const Event& event) {
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            FloatRect doneButton = getButtonRect(window, 0.5f, 0.95f, 0.04f, 0.45f);

            FloatRect bigArrow0 = getArrowRect(window, arrows[4 * P1], leftA);
            FloatRect bigArrow1 = getArrowRect(window, arrows[4 * P1], rightA);

            FloatRect smallArrow0 = getArrowRect(window, arrows[4 * P1+2], leftA);
            FloatRect smallArrow1 = getArrowRect(window, arrows[4 * P1+2], rightA);

            FloatRect accessoryArrow0 = getArrowRect(window, arrows[4 * P1 + 3], leftA);
            FloatRect accessoryArrow1 = getArrowRect(window, arrows[4 * P1 + 3], rightA);

            FloatRect bigArrow2 = getArrowRect(window, arrows[4 * P2], leftA);
            FloatRect bigArrow3 = getArrowRect(window, arrows[4 * P2], rightA);

            FloatRect smallArrow2 = getArrowRect(window, arrows[4 * P2 + 2], leftA);
            FloatRect smallArrow3 = getArrowRect(window, arrows[4 * P2 + 2], rightA);

            FloatRect accessoryArrow2 = getArrowRect(window, arrows[4 * P2 + 3], leftA);
            FloatRect accessoryArrow3 = getArrowRect(window, arrows[4 * P2 + 3], rightA);
            
            if (doneButton.contains(mouseF)) {
                //state.name[P1].insert(0, inputBuffer);
                //cout << "String '" << state.name[P1].toAnsiString() << "' succesfully copied to gameState name of player "<<P1+1<<  endl;
                initGame(state);
                state.appState = STATE_GAME;
            }
            else if (bigArrow0.contains(mouseF)) {
                state.character[P1].size -= 1;
                if (state.character[P1].size < 1) state.character[P1].size = 3;
            }
            else if (bigArrow1.contains(mouseF)) {
                state.character[P1].size += 1;
                if (state.character[P1].size > 3) state.character[P1].size = 1;
            }
            else if (smallArrow0.contains(mouseF)) {
                state.character[P1].bodyColor -= 1;
                if (state.character[P1].bodyColor < 0) state.character[P1].bodyColor = 7;
            }
            else if (smallArrow1.contains(mouseF)) {
                state.character[P1].bodyColor += 1;
                if (state.character[P1].bodyColor > 7) state.character[P1].bodyColor = 0;
            }
            else if (accessoryArrow0.contains(mouseF)) {
                state.character[P1].accessory -= 1;
                if (state.character[P1].accessory < 0) state.character[P1].accessory = 5;
            }
            else if (accessoryArrow1.contains(mouseF)) {
                state.character[P1].accessory += 1;
                if (state.character[P1].accessory > 5) state.character[P1].accessory = 0;
            }
            else if (state.gameMode == PVP) {
                if (bigArrow2.contains(mouseF)) {
                    state.character[P2].size -= 1;
                    if (state.character[P2].size < 1) state.character[P2].size = 3;
                }
                else if (bigArrow3.contains(mouseF)) {
                    state.character[P2].size += 1;
                    if (state.character[P2].size > 3) state.character[P2].size = 1;
                }
                else if (smallArrow2.contains(mouseF)) {
                    state.character[P2].bodyColor -= 1;
                    if (state.character[P2].bodyColor < 0) state.character[P2].bodyColor = 7;
                }
                else if (smallArrow3.contains(mouseF)) {
                    state.character[P2].bodyColor += 1;
                    if (state.character[P2].bodyColor > 7) state.character[P2].bodyColor = 0;
                }
                else if (accessoryArrow2.contains(mouseF)) {
                    state.character[P2].accessory -= 1;
                    if (state.character[P2].accessory < 0) state.character[P2].accessory = 5;
                }
                else if (accessoryArrow3.contains(mouseF)) {
                    state.character[P2].accessory += 1;
                    if (state.character[P2].accessory > 5) state.character[P2].accessory = 0;
                }
            }
        }
    }
}

void handleSaveScoreInput(RenderWindow& window, GameState& state, const Event& event, String& inputBuffer) {
    if (state.tb.isEmpty && !state.tb.Focused) {
        inputBuffer = "your name...";
    }
    if (const auto* mouseEvent = event.getIf<Event::MouseButtonReleased>()) {
        if (mouseEvent->button == Mouse::Button::Left) {

            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

            Vector2f size = state.tb.size;
            Vector2f pos = state.tb.pos;
            FloatRect textBoxP1 = getTextBoxRect(window, pos, size);
            FloatRect saveScoreButton = getButtonRect(window, 0.5f, 0.95f, 0.04f, 0.45f);

            if (textBoxP1.contains(mouseF) && !state.tb.Focused) {
                state.tb.isEmpty = false;
                inputBuffer.clear();
                state.tb.Focused = true;
            }
            else if (!textBoxP1.contains(mouseF) && state.tb.Focused){
                state.tb.Focused = false;
                if (inputBuffer.isEmpty()) {
                    state.tb.isEmpty = true;
                }
            }
            else if (saveScoreButton.contains(mouseF)) {
                //state.name[P1].insert(0, inputBuffer);
                //cout << "String '" << state.name[P1].toAnsiString() << "' succesfully copied to gameState name of player "<<P1+1<<  endl;
                //initGame(state);
                state.appState = STATE_MENU;
            }
        }
    }
    else if (const auto* textEntered = event.getIf<Event::TextEntered>())
    {
        if (state.tb.Focused) {
            if (textEntered->unicode > 32 && textEntered->unicode < 127 && inputBuffer.getSize()<18) {
                inputBuffer += static_cast<char>(textEntered->unicode);
            }
            else if (textEntered->unicode == 8 && inputBuffer.getSize() >= 1)
                inputBuffer.erase(inputBuffer.getSize() - 1, 1);
        }
    }
    else if (const auto* key = event.getIf<Event::KeyReleased>()) {
        if (key->scancode == Keyboard::Scancode::Enter) {
            state.tb.Focused = false;
            if (inputBuffer.isEmpty()) {
                state.tb.isEmpty = true;
            }
        }
    }
}

void handleGameInput(RenderWindow& window, GameState& state, const Event& event) {
    auto& T = state.board.T;
    auto& player = state.player;
    auto& pieceSize = state.pieceSize;
    auto& waitingForLeftClick = state.waitingForLeftClick;
    auto& correctSelection = state.correctSelection;
    auto& old_line = state.old_line;
    auto& old_col = state.old_col;

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    auto boardSize = state.board.size;
    auto boardX = state.board.pos.x;
    auto boardY = state.board.pos.y;
    float latura = boardSize / squareNumber;

    FloatRect pieces[3];

    if (const auto* key = event.getIf<Event::KeyReleased>()) {
        if (state.matchState != STATE_WIN && key->scancode == Keyboard::Scancode::Escape) {
            togglePause(state);
        }
        /*if (state.matchState == STATE_PLAY) {
            if (key->scancode == Keyboard::Scancode::Up && pieceSize < 3) pieceSize++;
            else if (key->scancode == Keyboard::Scancode::Down && pieceSize > 1) pieceSize--;
        }*/
    }

    if (const auto* buttonHeldDown = event.getIf<Event::MouseButtonPressed>()) {
        Vector2i mPos = Mouse::getPosition(window);
        Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));
        if (buttonHeldDown->button == Mouse::Button::Left && !state.heldDown) {
            Vector2i mPos = Mouse::getPosition(window);
            Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));
            float posX;
            float startY = state.board.pos.y;

            if (player == P1) {
                posX = 0.025 * winW;
            }
            else {
                posX = winW * 0.975 - latura;
            }
            for (int p = 0; p < 3; p++) {
                pieces[p] = drawCharacter(window, posX, startY + latura * p, latura / 32.f, p+1, state.character[player], false, false);
            }
            correctSelection = false;
            if (pieces[0].contains(mouseF)) {
                state.heldDown = true;
                pieceSize = 1;
            }
            else if (pieces[1].contains(mouseF)) {
                state.heldDown = true;
                pieceSize = 2;
            }
            else if (pieces[2].contains(mouseF)) {
                state.heldDown = true;
                pieceSize = 3;
            }
            else if (mouseF.x >= boardX && mouseF.x <= boardX + state.board.size && mouseF.y >= boardY && mouseF.y <= boardY + state.board.size) {
                int line = static_cast<int>((mPos.y - boardY) / latura);
                int col = static_cast<int>((mPos.x - boardX) / latura);
                if (T[line+1][col+1].nr > 0) {
                    if (player == P1 && sign(T[line + 1][col + 1].p[T[line + 1][col + 1].nr]) == 1 || player == P2 && sign(T[line + 1][col + 1].p[T[line + 1][col + 1].nr]) == -1) {
                        state.heldDown = true;

                        pieceSize = abs(T[line + 1][col + 1].p[T[line + 1][col + 1].nr]);
                        correctSelection = true;
                        old_line = line;
                        old_col = col;
                    }
                    
                }

            }

        }
    }

    if (const auto* buttonReleased = event.getIf<Event::MouseButtonReleased>()) {
        Vector2i mPos = Mouse::getPosition(window);
        Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

        int line = -20, col = -20;

        if (state.matchState == STATE_PLAY) {
            if (buttonReleased->button == Mouse::Button::Left && state.heldDown) {
                state.heldDown = false;
                if (mouseF.x >= boardX && mouseF.x <= boardX + state.board.size && mouseF.y >= boardY && mouseF.y <= boardY + state.board.size) {
                    line = static_cast<int>((mPos.y - boardY) / latura);
                    col = static_cast<int>((mPos.x - boardX) / latura);
                    if (correctSelection) {
                        if (movePiece(state, old_line, old_col, line, col)) {
                            //printf("Current state has a value of: %d, for player %d\n", evaluate(state, player), player + 1);
                            checkWin(state, true);
                            player = 1 - player;
                            if (state.gameMode == PVE_RAND && state.matchState == STATE_PLAY) {
                                makeRandomMove(state);
                                //printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state, true);
                                player = 1 - player;
                            }
                            else if (state.gameMode == PVE_MIMIMAX && state.matchState == STATE_PLAY) {
                                executeMove(state, chooseBestMove(state, 3));
                                //printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state, true);
                                player = 1 - player;
                            }
                        }
                    }
                    else {
                        int val = (player == P1) ? pieceSize : -1 * pieceSize;
                        if (punePiesa(state, line + 1, col + 1, val, false)) {
                            //printf("Current state has a value of: %d, for player %d\n", evaluate(state, player), player + 1);
                            checkWin(state, true);
                            player = 1 - player;
                            if (state.gameMode == PVE_RAND && state.matchState == STATE_PLAY) {
                                makeRandomMove(state);
                                //printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state, true);
                                player = 1 - player;
                            }
                            else if (state.gameMode == PVE_MIMIMAX && state.matchState == STATE_PLAY) {
                                executeMove(state, chooseBestMove(state, 3));
                                //printf("Current state has a value of: %d, for player %d\n", evaluate(state, P2), P2 + 1);
                                checkWin(state, true);
                                player = 1 - player;
                            }
                        }
                    }
                } 
                old_line = -20; old_col = -20;
                correctSelection = false;
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
        else if (state.matchState == STATE_WIN) {
            FloatRect btnMainMenuRect = getButtonRect(window, 0.5f, 0.89f, 0.027f, 0.45f);
            FloatRect btnSaveScoreRect = getButtonRect(window, 0.5f, 0.78f, 0.04f, 0.5f);

            if (buttonReleased->button == Mouse::Button::Left) {
                if (btnMainMenuRect.contains(mouseF)) {
                    state.appState = STATE_MENU;
                }
                else if (btnSaveScoreRect.contains(mouseF) && state.gameMode == PVP || btnSaveScoreRect.contains(mouseF) && state.winner == P1) {
                    state.appState = STATE_SAVE_HIGHSCORE;
                }
            }
        }
    }
}

void handleResize(RenderWindow& window, const Event::Resized& resizeEvent,GameState& state) {
    FloatRect visibleArea(
        { 0.f, 0.f },
        { static_cast<float>(resizeEvent.size.x), static_cast<float>(resizeEvent.size.y) }
    );
    window.setView(View(visibleArea));

    state.stack->handleResize();
}

void handleInput(RenderWindow& window, GameState& state, arrowSet arrows[], String& inputBuffer) {
    while (const optional event = window.pollEvent()) {
        if (event->is<Event::Closed>())
        {
            window.close();
        }
        else if(const auto* resizeEvent = event->getIf<Event::Resized>())
        {
            handleResize(window, *resizeEvent, state);
        }

        switch (state.appState) {
            case STATE_MENU:
            {    
                handleMenuInput(window, state, *event);
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
                handleCustomizationMenuInput(window, state, arrows, *event);
                break;
            }
            case STATE_SAVE_HIGHSCORE:
            {
                handleSaveScoreInput(window, state, *event, inputBuffer);
                break;
            }
        }
    }
}