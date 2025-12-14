#include "Renderer.h"
#include "GameLogic.h"
#include <algorithm>
#include <string>
#include <fstream>

using namespace sf;
using namespace std;


float getScaleFactor(const RenderWindow& window) {
    float w = static_cast<float>(window.getSize().x);
    float h = static_cast<float>(window.getSize().y);
    return std::min(w, h);
}

void centerText(Text& text, float x, float y) {
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
    text.setPosition({ x, y });
}

void drawStyledButton(RenderWindow& window, Font& font, const ButtonConfig& cfg) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = getScaleFactor(window);

    unsigned int fontSize = static_cast<unsigned int>(scale * cfg.sizePerc);
    if (fontSize < 10) fontSize = 10;

    Text txt(font, cfg.text, fontSize);
    txt.setFillColor(cfg.color);

    FloatRect bounds = txt.getLocalBounds();
    txt.setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
    txt.setPosition({ winW * cfg.xPerc, winH * cfg.yPerc });

    window.draw(txt);
}

void drawCell(RenderWindow& window, const GameState& state, int line, int col, float latura, float boardX, float boardY) {
    int varf = state.T[line + 1][col + 1].nr;
    int valPiesa = state.T[line + 1][col + 1].p[varf];

    if (valPiesa == 0) return;

    float maxRadius = latura / 2.0f;
    float radius = 0.f;
    Color color = Color::White;

    switch (valPiesa) {
    case -3:
        color = Color::Red;
        radius = maxRadius * 0.85f; // Mare
        break;
    case -2:
        color = Color::Red;
        radius = maxRadius * 0.65f; // Mediu
        break;
    case -1:
        color = Color::Red;
        radius = maxRadius * 0.40f; // Mic
        break;
    case 1:
        color = Color::Green;
        radius = maxRadius * 0.40f; // Mic
        break;
    case 2:
        color = Color::Green;
        radius = maxRadius * 0.65f; // Mediu
        break;
    case 3:
        color = Color::Green;
        radius = maxRadius * 0.85f; // Mare
        break;
    }

    CircleShape piece(radius);
    piece.setFillColor(color);
    piece.setOrigin({ radius, radius });

    float centerX = boardX + col * latura + latura / 2.0f;
    float centerY = boardY + line * latura + latura / 2.0f;

    piece.setPosition({ centerX, centerY });
    window.draw(piece);
}


void highlightSquare(RenderWindow& window, int col, int line) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float currentTableSize = std::min(winW, winH) * 0.8f;

    float boardX = (winW - currentTableSize) / 2.0f;
    float boardY = (winH - currentTableSize) / 2.0f;
    float latura = currentTableSize / squareNumber;

    RectangleShape square({ latura, latura });
    square.setFillColor(Color::Transparent);
    square.setOutlineThickness(8.f);
    square.setOutlineColor(Color::Red);

    square.setPosition({ boardX + col * latura, boardY + line * latura });

    window.draw(square);
}

void drawTable(RenderWindow& window, const GameState& state) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float currentTableSize = std::min(winW, winH) * 0.8f;


    float boardX = (winW - currentTableSize) / 2.0f;
    float boardY = (winH - currentTableSize) / 2.0f;

    float latura = currentTableSize / squareNumber;

    RectangleShape square({ latura, latura });
    square.setFillColor(Color::White);
    square.setOutlineThickness(5.f);
    square.setOutlineColor(Color::Black);

    // Desenăm grila
    for (int i = 0; i < squareNumber; i++) {
        for (int j = 0; j < squareNumber; j++) {
            square.setPosition({ boardX + j * latura, boardY + i * latura });
            window.draw(square);

            drawCell(window, state, i, j, latura, boardX, boardY);
        }
    }

    RectangleShape border({ currentTableSize, currentTableSize });
    border.setPosition({ boardX, boardY });
    border.setFillColor(Color::Transparent);
    border.setOutlineThickness(10.f);
    border.setOutlineColor(Color::Red);
    window.draw(border);
}

void drawGame(RenderWindow& window, const GameState& state, Text& text, Font& font) {
    auto player = state.player;

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    drawTable(window, state);

    if (state.old_col != -20) {
        highlightSquare(window, state.old_col, state.old_line);
    }

    unsigned int uiSize = static_cast<unsigned int>(std::min(winW, winH) * 0.05f);
    if (uiSize < 12) uiSize = 12;
    text.setCharacterSize(uiSize);

    text.setString("Player " + to_string(state.player + 1));
    text.setOrigin({ 0.f, 0.f });
    text.setPosition({ winW * 0.05f, winH * 0.05f });
    window.draw(text);

    text.setString("Size: " + to_string(state.pieceSize) + "(x" + to_string(state.pieces[player][state.pieceSize]) + ")");
    FloatRect sBounds = text.getLocalBounds();
    text.setOrigin({ sBounds.size.x, 0.f });
    text.setPosition({ winW * 0.95f, winH * 0.05f });
    window.draw(text);

    ButtonConfig saveCfg = { "Save",             0.1f, 0.95f, Color::Red,  0.04f };
    drawStyledButton(window, font, saveCfg);
}

void drawMenu(RenderWindow& window,GameState state, Font& font) {
    Color loadColor = Color(150, 150, 150);

    ifstream testFile("save.txt");
    bool saveExists = testFile.good();
    testFile.close();
    
    if (saveExists)
        loadColor = Color::White;

    ButtonConfig titleCfg = { "GOBBLET GOBBLERS",0.5f, 0.20f, Color::Yellow, 0.08f };
    ButtonConfig playCfg = { "NEW GAME",         0.5f, 0.45f, Color::White,  0.05f };
    ButtonConfig loadCfg = { "LOAD GAME",        0.5f, 0.60f, loadColor, 0.05f };
    ButtonConfig exitCfg = { "EXIT",             0.5f, 0.75f, Color::White,  0.05f };

    drawStyledButton(window, font, titleCfg);
    drawStyledButton(window, font, playCfg);
    drawStyledButton(window, font, loadCfg);
    drawStyledButton(window, font, exitCfg);
}

void drawSelectGameModeMenu(RenderWindow& window, GameState state, Font& font) {

    ButtonConfig pvpCfg = { "Player Vs Player",  0.5f, 0.40f, Color::White, 0.05f };
    ButtonConfig pveCfg = { "Player Vs Computer",0.5f, 0.60f, Color(150,150,150),  0.05f};

    drawStyledButton(window, font, pvpCfg);
    drawStyledButton(window, font, pveCfg);
}