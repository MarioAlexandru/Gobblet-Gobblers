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
    txt.setOutlineThickness(static_cast<float>(fontSize)*0.1);
    txt.setOutlineColor(Color(0,0,55,255));

    FloatRect bounds = txt.getLocalBounds();
    txt.setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
    txt.setPosition({ winW * cfg.xPerc, winH * cfg.yPerc });
    FloatRect gBounds = txt.getGlobalBounds();
    gBounds.size.x *= 1.2f;
    gBounds.size.y *= 1.2f;
    float scaleY = scale * cfg.sizePerc * 1.5f;

    window.draw(txt);
}

void drawCell(RenderWindow& window, const GameState& state, int line, int col, float latura, float boardX, float boardY) {
    int varf = state.T[line + 1][col + 1].nr;
    int valPiesa = state.T[line + 1][col + 1].p[varf];

    if (valPiesa == 0) return;

    Texture spritesheet("spritesheet.png");
    Sprite piece(spritesheet);

    float maxRadius = latura / 2.0f;
    float radius = 0.f;
    Color color = Color::White;

    switch (valPiesa) {
    case -3:
        piece.setTextureRect(IntRect({ 64,32 }, { 32,32 }));
        break;
    case -2:
        piece.setTextureRect(IntRect({ 32,32 }, { 32,32 }));
        break;
    case -1:
        piece.setTextureRect(IntRect({ 0,32 }, { 32,32 }));
        break;
    case 1:
        piece.setTextureRect(IntRect({ 0,0 }, { 32,32 }));
        break;
    case 2:
        piece.setTextureRect(IntRect({ 32,0 }, { 32,32 }));
        break;
    case 3:
        piece.setTextureRect(IntRect({ 64,0 }, { 32,32 }));
        break;
    }

    float centerX = boardX + col * latura ;
    float centerY = boardY + line * latura ;

    piece.setPosition({ centerX, centerY });
    float sizeScale = latura / 32;
    piece.scale({ sizeScale,sizeScale });
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

    Texture spritesheet("spritesheet.png");
    Sprite boardSquare(spritesheet);
    boardSquare.setTextureRect(IntRect({ 96,0 }, { 32,32 }));

    float sizeScale = latura / 32;
    boardSquare.scale({ sizeScale,sizeScale });

    // Desenăm grila
    for (int i = 0; i < squareNumber; i++) {
        for (int j = 0; j < squareNumber; j++) {
            boardSquare.setPosition({ boardX + j * (latura), boardY + i * latura });
            window.draw(boardSquare);

            drawCell(window, state, i, j, latura, boardX, boardY);
        }
    }
}

void simpleGradient(RenderWindow& window, Color color1, Color color2, IntRect rectangle) {

    // create an array of 4 vertices that define a rectangle
    sf::VertexArray gradientRect(sf::PrimitiveType::TriangleStrip, 4);

    float x1 = static_cast<float>(rectangle.position.x);
    float y1 = static_cast<float>(rectangle.position.y);
    float x2 = static_cast<float>(rectangle.position.x+rectangle.size.x);
    float y2 = static_cast<float>(rectangle.position.y+rectangle.size.y);

    // define the position of the rectangle's points
    gradientRect[0].position = sf::Vector2f(x1, y1);
    gradientRect[1].position = sf::Vector2f(x1, y2);
    gradientRect[2].position = sf::Vector2f(x2, y1);
    gradientRect[3].position = sf::Vector2f(x2, y2);

    // define the color of the rectangle's points
    gradientRect[0].color = color1;
    gradientRect[1].color = color1;
    gradientRect[2].color = color2;
    gradientRect[3].color = color2;

    window.draw(gradientRect);
}

void drawBackground(RenderWindow& window, int offsetx, int offsety) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float currentWindowSize = min(winW, winH) * 0.8f;

    Color grColor1(124, 244, 255);
    Color grColor2(255, 124, 196);

    simpleGradient(window, grColor1, grColor2, IntRect({ 0,0 }, { static_cast<int>(winW),static_cast<int>(winH) }));

    float sizeScale = currentWindowSize / 328.f;

    Texture scroll("scroll.png");
    scroll.setRepeated(true);
    Sprite scrollBackground(scroll);
    scrollBackground.setTextureRect(IntRect({ offsetx,offsety }, { 2000,2000 }));
    scrollBackground.setColor(Color(255,255,255,185));
    scrollBackground.scale({ sizeScale,sizeScale });
    window.draw(scrollBackground);
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

    //draw display text for current player's turn
    text.setString("Player " + to_string(state.player + 1));
    text.setOrigin({ 0.f, 0.f });
    text.setPosition({ winW * 0.05f, winH * 0.05f });
    window.draw(text);

    //draw display text for piece size and remaining pieces
    text.setString("Size: " + to_string(state.pieceSize) + "(x" + to_string(state.pieces[player][state.pieceSize]) + ")");
    FloatRect sBounds = text.getLocalBounds();
    text.setOrigin({ sBounds.size.x, 0.f });
    text.setPosition({ winW * 0.95f, winH * 0.05f });
    window.draw(text);

    //draw display text for current match elapsedTime in seconds
    int currentTime = getCurrentPlayTime(state);
    text.setString("Time: " + to_string(currentTime));
    sBounds = text.getLocalBounds();
    text.setOrigin({ sBounds.size.x/2.f, 0.f });
    text.setPosition({ winW * 0.5f, winH * 0.05f });
    window.draw(text);
}

void drawText(RenderWindow& window, TextBox object, Text& text, String& textString, FloatRect boundingBox) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    unsigned int uiSize = static_cast<unsigned int>(std::min(winW, winH) * 0.05f);
    //if (uiSize < 12) uiSize = 12;
    text.setCharacterSize(uiSize);

    text.setString(textString);
    FloatRect sBounds = text.getLocalBounds();
    text.setOrigin({ sBounds.size.x / 2.f, 0.f });
    text.setPosition({ boundingBox.getCenter().x, boundingBox.getCenter().y});
    FloatRect FBounds = text.getGlobalBounds();
    float bX = boundingBox.size.x * 0.925f;
    float bY = boundingBox.size.y * 0.925f;
    if (bX - FBounds.size.x < 25.f) {
        text.scale({0.99f,0.99f});
    }
    else if(bY - FBounds.size.y > 55.f && (bX - FBounds.size.x > 35.f)){
        text.scale({ 1.05f,1.05f });
    }
    
    text.setFillColor(Color::White);
    if(object.isEmpty)
        text.setFillColor(Color(155,155,155));
    window.draw(text);
}

void drawPauseMenu(RenderWindow& window, GameState state,Text& text, Font& font) {
    auto player = state.player;

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float currentMenuSize = std::min(winW, winH) * 0.7f;

    unsigned int uiSize = static_cast<unsigned int>(std::min(winW, winH) * 0.05f);
    if (uiSize < 12) uiSize = 12;
    text.setCharacterSize(uiSize);

    Color pauseMenu0 = Color(0, 0, 0, 185);
    RectangleShape Menu;
    Menu.setSize({currentMenuSize,winH*0.9f});
    Menu.setFillColor(pauseMenu0);
    Menu.setPosition({(winW-currentMenuSize)/2.f,(winH-winH*0.9f)/2.f});
    Menu.setOutlineThickness(8.f);
    Menu.setOutlineColor(Color::White);
    window.draw(Menu);

    ButtonConfig resumeCfg = {"RESUME MATCH", 0.5f, 0.20f, Color::White, 0.05f};
    ButtonConfig saveCfg = { "SAVE STATE", 0.5f, 0.80f, Color::White, 0.05f };
    ButtonConfig exitCfg = { "EXIT GAME", 0.5f, 0.90f, Color::White, 0.05f };
    drawStyledButton(window, font, resumeCfg);
    drawStyledButton(window, font, saveCfg);
    drawStyledButton(window, font, exitCfg);
}

void drawMenu(RenderWindow& window,GameState state, Font& font) {
    Color loadColor = Color(150, 150, 150);

    ifstream testFile("save.txt");
    bool saveExists = testFile.good();
    testFile.close();
    
    if (saveExists)
        loadColor = Color::White;

    ButtonConfig titleCfg = { "GOBBLET GOBBLERS",  0.5f, 0.20f, Color::Yellow, 0.08f };
    ButtonConfig playCfg = { "NEW GAME",           0.5f, 0.45f, Color::White,  0.05f };
    ButtonConfig loadCfg = { "LOAD GAME",          0.5f, 0.60f, loadColor, 0.05f };
    ButtonConfig exitCfg = { "EXIT",               0.5f, 0.75f, Color::White,  0.05f };
    ButtonConfig customizeCfg = { "CUSTOMIZATION", 0.5f, 0.9f, Color::Red,  0.06f };

    drawStyledButton(window, font, titleCfg);
    drawStyledButton(window, font, playCfg);
    drawStyledButton(window, font, loadCfg);
    drawStyledButton(window, font, exitCfg);
    drawStyledButton(window, font, customizeCfg);
}

void drawSelectGameModeMenu(RenderWindow& window, GameState state, Font& font) {

    ButtonConfig pvpCfg = { "Player Vs Player",  0.5f, 0.40f, Color::White, 0.05f };
    ButtonConfig pveCfg = { "Player Vs Computer",0.5f, 0.60f, Color::White,  0.05f};

    drawStyledButton(window, font, pvpCfg);
    drawStyledButton(window, font, pveCfg);
}

void drawSelectDifficultyMenu(RenderWindow& window, GameState state, Font& font) {

    ButtonConfig randomCfg = { "Random Computer AI",  0.5f, 0.40f, Color::White, 0.05f };
    ButtonConfig strategyCfg = { "Strategy Computer AI",0.5f, 0.60f, Color::White,  0.05f };

    drawStyledButton(window, font, randomCfg);
    drawStyledButton(window, font, strategyCfg);
}

FloatRect drawTextBox(RenderWindow& window, TextBox object) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = min(winW, winH)*0.9;

    float posX = object.pos.x;
    float posY = object.pos.y;
    float sizeX = object.size.x;
    float sizeY = object.size.y;

    RectangleShape textbox({ sizeX,sizeY });
    textbox.setOrigin({ sizeX / 2.f, 0 });
    textbox.setPosition({ posX,posY });
    if (object.Focused) {
        textbox.setFillColor(Color(85, 85, 85));
        textbox.scale({ 1.1f,1.1f });
    }
    else {
        textbox.setFillColor(Color(105, 105, 105));
        textbox.scale({ 1.f,1.f });
    }
    textbox.setOutlineColor(Color::Black);
    textbox.setOutlineThickness(scale*0.008f);
    window.draw(textbox);

    return textbox.getGlobalBounds();
}

void drawCustomizationMenu(RenderWindow& window, Text& text, Font& font, String& textString, TextBox textBox[]) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = min(winW, winH) * 0.9f;

    RectangleShape p1({ winW / 2.f,winH });
    RectangleShape p2({ winW / 2.f,winH });
    p2.setPosition({ winW / 2.f,0.f });
    p1.setFillColor(Color(55,155,0,55));
    p2.setFillColor(Color(155,25,55,55));
    p1.setOutlineColor(Color(0,0,0,200));
    p1.setOutlineThickness(10.f);
    

    

    ButtonConfig plrCustCfg = { "PLR 1'S CHARACTER",  0.25f, 0.1f, Color::Red, 0.04f };
    ButtonConfig doneCfg = { "DONE CUSTOMIZING",  0.25f, 0.95f, Color::Red, 0.04f };

    

    drawStyledButton(window, font, plrCustCfg);
    drawStyledButton(window, font, doneCfg);

    //TextBox plr1Box;
    //plr1Box.pos = { 0.5f,0.5f };
    //plr1Box.size = { 0.5f,0.1f };
    
    
    

    window.draw(p1);
    window.draw(p2);

    float dSizeY = scale*0.4f;
    float dSizeX = dSizeY * 0.8f;
    RectangleShape display1({dSizeX,dSizeY});
    display1.setOrigin({ dSizeX / 2.f,0});
    display1.setPosition({ winW*0.25f,winH*0.3f });
    display1.setFillColor(Color::Black);
    window.draw(display1);

    ButtonConfig bodyTypeCfg = { "bodyType1",  0.25f, 0.725f, Color::White, 0.025f };
    ButtonConfig colorPaletteCfg = { "color1",  0.25f, 0.785f, Color::White, 0.025f };
    ButtonConfig accesoryCfg = { "accesory1",  0.25f, 0.845f, Color::White, 0.025f };

    drawStyledButton(window, font, bodyTypeCfg);
    drawStyledButton(window, font, colorPaletteCfg);
    drawStyledButton(window, font, accesoryCfg);

    textBox[0].pos = { winW*0.25f, winH * 0.15f };
    textBox[0].size = { scale*0.45f , scale * 0.1f };
    FloatRect boundingBox = drawTextBox(window, textBox[0]);
    if (textString.getSize() >= 1)
        drawText(window, textBox[0], text, textString, boundingBox);
}