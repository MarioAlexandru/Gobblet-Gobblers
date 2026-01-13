#include "Renderer.h"
#include "GameLogic.h"
#include <algorithm>
#include <string>
#include <fstream>

using namespace sf;
using namespace std;

#pragma region helperFunctions
float getScaleFactor(const RenderWindow& window) {
    float w = static_cast<float>(window.getSize().x);
    float h = static_cast<float>(window.getSize().y);
    return std::min(w, h);
}

FloatRect getRect(const RenderWindow& window, Vector2f pos, Vector2f size) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = std::min(winW, winH) * 0.9f;

    float height = scale * size.y;
    float width = scale * size.x;

    float xPos = winW * pos.x;
    float yPos = winH * pos.y;

    return FloatRect(
        { xPos - width / 2.0f, yPos },
        { width, height }
    );
}

void centerText(Text& text, float x, float y) {
    FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f });
    text.setPosition({ x, y });
}

void simpleGradient(RenderWindow& window, Color color1, Color color2, IntRect rectangle) {

    // create an array of 4 vertices that define a rectangle
    sf::VertexArray gradientRect(sf::PrimitiveType::TriangleStrip, 4);

    float x1 = static_cast<float>(rectangle.position.x);
    float y1 = static_cast<float>(rectangle.position.y);
    float x2 = static_cast<float>(rectangle.position.x + rectangle.size.x);
    float y2 = static_cast<float>(rectangle.position.y + rectangle.size.y);

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

void drawText(RenderWindow& window, Font& font, Vector2f position, Color color, float sizePerc, String message) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = getScaleFactor(window);

    unsigned int fontSize = static_cast<unsigned int>(scale * sizePerc);
    if (fontSize < 10) fontSize = 10;
    
    Text txt(font, message, fontSize);
    txt.setFillColor(color);
    txt.setOutlineThickness(static_cast<float>(fontSize) * 0.1);
    txt.setOutlineColor(Color(0, 0, 55, 255));

    FloatRect bounds = txt.getLocalBounds();
    txt.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f });

    txt.setPosition(position);
    window.draw(txt);
}

void displayFPS(RenderWindow& window, Font& font, float fpsCounter) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = getScaleFactor(window);

    float perc = 0.04f;

    unsigned int fontSize = static_cast<unsigned int>(scale * perc);
    if (fontSize < 10) fontSize = 10;

    Text fpsDisplay(font);
    fpsDisplay.setCharacterSize(fontSize);
    fpsDisplay.setFillColor(Color::Red);
    fpsDisplay.setPosition({ winW * 0.05f,winH * 0.95f });

    String fpsString("FPS: " + to_string(fpsCounter));

    fpsDisplay.setString(fpsString);
    window.draw(fpsDisplay);

}
#pragma endregion

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

    window.draw(txt);
}

void drawCharacter(RenderWindow& window, float posX, float posY, float scaleFactor, int pieceSize, Character character) {
    Texture spritesheet("spritesheet.png");
    Sprite body(spritesheet);
    Sprite face(spritesheet);

    body.setPosition({ posX, posY });
    face.setPosition({ posX, posY });
    body.scale({ scaleFactor,scaleFactor });
    face.scale({ scaleFactor,scaleFactor });

    switch (pieceSize) {
    case 1:
        body.setTextureRect(IntRect({ 0,32 }, { 32,32 }));
        face.setTextureRect(IntRect({ 0,64 }, { 32,32 }));
        break;
    case 2:
        body.setTextureRect(IntRect({ 32,32 }, { 32,32 }));
        face.setTextureRect(IntRect({ 32,64 }, { 32,32 }));
        break;
    case 3:
        body.setTextureRect(IntRect({ 64,32 }, { 32,32 }));
        face.setTextureRect(IntRect({ 64,64 }, { 32,32 }));
        break;
    }

    body.setColor(character.palette[character.bodyColor]);

    window.draw(body);
    window.draw(face);
}

void drawCell(RenderWindow& window, const GameState& state, int line, int col) {
    auto board = state.board;
    auto& T = board.T;
    auto boardX = board.pos.x;
    auto boardY = board.pos.y;

    int varf = T[line + 1][col + 1].nr;
    int valPiesa = T[line + 1][col + 1].p[varf];
    int pieceSize = abs(valPiesa);

    int player;

    if (valPiesa == 0) return;

    float latura = board.size / squareNumber;

    float centerX = boardX + col * latura ;
    float centerY = boardY + line * latura ;
    
    float scale = latura / 32.f;

    if (valPiesa < 0) {
        player = P2;
    }
    else {
        player = P1;
    }

    drawCharacter(window, centerX, centerY, scale, pieceSize, state.character[player]);
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

void drawRemainingPieces(RenderWindow& window, const GameState& state, float scaleFactor, int player) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = std::min(winW, winH) * 0.8f;
    
    float latura = state.board.size/3;
    float startY = state.board.pos.y;

    float startX = winW * 0.05f;

    float posX;

    if (player == P1) {
        posX = 0.025 * winW;
    }
    else {
        posX = winW * 0.975 - latura;
    }

    if (player != state.player || !state.heldDown) {
        for (int i = 1; i <= 3; i++) {
            drawCharacter(window, posX, startY + latura * (i - 1), scaleFactor, i, state.character[player]);
        }
    }
}

void drawTable(RenderWindow& window, GameState& state) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    auto& boardSize = state.board.size;
    auto& boardX = state.board.pos.x;
    auto& boardY = state.board.pos.y;

    if (winH < winW*0.75f) {
        boardSize = winH * 0.8f;
    }
    else {
        boardSize = winW * 0.5f;
    }

    boardX = (winW - boardSize) / 2.0f;
    boardY = (winH - boardSize) / 2.0f;

    float latura = boardSize / squareNumber;

    Texture spritesheet("spritesheet.png");
    Sprite boardSquare(spritesheet);
    boardSquare.setTextureRect(IntRect({ 96,0 }, { 32,32 }));

    float sizeScale = latura / 32.f;
    boardSquare.scale({ sizeScale,sizeScale });

    // Desenăm grila
    for (int i = 0; i < squareNumber; i++) {
        for (int j = 0; j < squareNumber; j++) {
            boardSquare.setPosition({ boardX + j * (latura), boardY + i * latura });
            window.draw(boardSquare);

            drawCell(window, state, i, j);
        }
    }
    Vector2i mPos = Mouse::getPosition(window);
    Vector2f mouseF(static_cast<float>(mPos.x), static_cast<float>(mPos.y));

    drawRemainingPieces(window, state, latura / 32.f, P1);
    drawRemainingPieces(window, state, latura / 32.f, P2);

    if (state.heldDown) {
        drawCharacter(window, mouseF.x-latura/2.f, mouseF.y-latura/2.f, latura / 32.f, state.pieceSize, state.character[state.player]);
    }
}

void drawBackground(RenderWindow& window, int offsetx, int offsety) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float currentWindowSize = min(winW, winH);

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

void drawGame(RenderWindow& window, GameState& state, Text& text, Font& font) {
    auto player = state.player;

    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    drawTable(window, state);

    if (state.old_col != -20) {
        highlightSquare(window, state.old_col, state.old_line);
    }

    

    //draw display text for current player's turn
    string temporary = "Player " + to_string(state.player + 1);

    ButtonConfig playerTurnCfg = { temporary.c_str(), 0.125f, 0.05f, Color::White, 0.04f };
    drawStyledButton(window, font, playerTurnCfg);

    //draw display text for piece size and remaining pieces
    temporary = "Size: " + to_string(state.pieceSize) + "(x" + to_string(state.pieces[player][state.pieceSize]) + ")";

    ButtonConfig pieceSizeCfg = { temporary.c_str(), 0.85f, 0.05f, Color::White, 0.04f };
    drawStyledButton(window, font, pieceSizeCfg);

    //draw display text for current match elapsedTime in seconds
    int currentTime = getCurrentPlayTime(state);
    temporary = "Time: " + to_string(currentTime);

    ButtonConfig timeDisplayCfg = { temporary.c_str(), 0.5f, 0.05f, Color::White, 0.04f };
    drawStyledButton(window, font, timeDisplayCfg);
}

void drawTextInput(RenderWindow& window, TextBox object, Text& text, String& textString) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    unsigned int uiSize = static_cast<unsigned int>(std::min(winW, winH) * 0.05f);
    //if (uiSize < 12) uiSize = 12;
    text.setCharacterSize(uiSize);

    text.setString(textString);
    text.setOutlineThickness(0.f);
    FloatRect sBounds = text.getLocalBounds();
    FloatRect boundingBox = getRect(window, object.pos, object.size);
    text.setOrigin({ sBounds.size.x / 2.f, 0 });
    text.setPosition({boundingBox.getCenter().x, boundingBox.getCenter().y});
    FloatRect FBounds = text.getGlobalBounds();
    float bX = boundingBox.size.x * 0.925f;
    float bY = boundingBox.size.y * 0.925f;
    if (bX - FBounds.size.x < 15.f || (bY - FBounds.size.y < 10.f)) {
        text.scale({0.99f,0.99f});
    }
    else if(bY - FBounds.size.y > 35.f && (bX - FBounds.size.x > 25.f)){
        text.scale({ 1.05f,1.05f });
    }
    
    text.setFillColor(Color::White);
    if(object.isEmpty)
        text.setFillColor(Color(155,155,155));
    window.draw(text);
}

void drawPauseMenu(RenderWindow& window, GameState state,Text& text, Font& font) {

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

void drawWinMenu(RenderWindow& window, GameState state, Text& text, Font& font) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float currentMenuSize = std::min(winW, winH) * 0.9f;

    unsigned int uiSize = static_cast<unsigned int>(std::min(winW, winH) * 0.05f);
    if (uiSize < 12) uiSize = 12;
    text.setCharacterSize(uiSize);

    Color Menu0 = Color(0, 0, 0, 225);
    RectangleShape Menu;
    float sizeX = winW * 0.85f;
    Menu.setSize({sizeX,winH*0.9f});
    Menu.setOrigin({ sizeX / 2.f, winH * 0.45f });
    Menu.setFillColor(Menu0);
    Menu.setPosition({ winW*0.5f,winH*0.5f});
    Menu.setOutlineThickness(8.f);
    Menu.setOutlineColor(Color::White);
    window.draw(Menu);

    string winMessage;

    if (state.gameMode == PVP) {
        winMessage = "Player ";
        winMessage += to_string(state.winner + 1);
        winMessage += " has won the game!";
    }
    else {
        if (state.winner == P1) {
            winMessage = "Congratulations! You have won the game!";
        }
        else {
            winMessage = "You have lost the game!";
        }
    }

    ButtonConfig winMessageCfg = { winMessage.c_str(), 0.5f, 0.20f, Color::White, 0.05f};
    ButtonConfig saveScoreCfg = { "SAVE TO LEADERBOARD", 0.5f, 0.80f, Color::White, 0.035f };
    ButtonConfig returnMainCfg = { "RETURN TO MAIN MENU", 0.5f, 0.90f, Color::White, 0.035f };
    drawStyledButton(window, font, winMessageCfg);
    drawStyledButton(window, font, saveScoreCfg);
    drawStyledButton(window, font, returnMainCfg);
}

#pragma region MenuFunctions
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

    drawStyledButton(window, font, titleCfg);
    drawStyledButton(window, font, playCfg);
    drawStyledButton(window, font, loadCfg);
    drawStyledButton(window, font, exitCfg);
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
#pragma endregion

void drawTextBox(RenderWindow& window, TextBox object) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = min(winW, winH)*0.9;

    float posX = winW*object.pos.x;
    float posY = winH*object.pos.y;
    float sizeX = scale*object.size.x;
    float sizeY = scale*object.size.y;

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

}

void drawSaveToLeaderboard(RenderWindow& window, GameState& state, Text& text, Font font, String& textString) {
    ButtonConfig saveScoreCfg = { "SAVE HIGHSCORE", 0.5f, 0.95f, Color::Red, 0.04f };
    drawStyledButton(window, font, saveScoreCfg);

    state.tb.pos = { 0.5f,0.5f };
    state.tb.size = { 0.45f,0.1f };
    drawTextBox(window, state.tb);

    if (textString.getSize() >= 1) {
        drawTextInput(window, state.tb, text, textString);
    }
}

void drawScrollArrows(RenderWindow& window, arrowSet arrows) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = std::min(winW, winH) * 0.9f;
    
    Texture arrow;
    if (arrows.type == smallA) {
    arrow.loadFromFile("arrow_small.png");
    }
    else {
    arrow.loadFromFile("arrow_big.png");
    }
    float dist = arrows.distPerc / 2.f;

    Sprite sprite(arrow);

    if (arrows.type == bigA) {
        sprite.setOrigin({ 0.f,16.f });
    }
    else {
        sprite.setOrigin({ 0.f, 8.f });
    }
    sprite.setScale({ arrows.sizePerc,arrows.sizePerc });
    sprite.setPosition({ arrows.pos.x+dist*winW,arrows.pos.y});
    window.draw(sprite);
    sprite.setPosition({ arrows.pos.x-dist*winW,arrows.pos.y});
    sprite.setScale({ -arrows.sizePerc, arrows.sizePerc});
    window.draw(sprite);
    
}

void drawPlrCustomize(RenderWindow& window, Character character, arrowSet arrows[], Font& font, float xPerc, int player) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);
    float scale = getScaleFactor(window) * 0.9f;

    string message = "PLR ";
    message += to_string(player + 1);
    message += +"'S CHARACTER";
    ButtonConfig plrCustCfg = { message.c_str(), xPerc, 0.1f, Color::Red, 0.04f};

    drawStyledButton(window, font, plrCustCfg);

    float dSizeY = scale * 0.4f;
    float dSizeX = dSizeY * 0.8f;
    RectangleShape display1({ dSizeX,dSizeY });
    display1.setOrigin({ dSizeX / 2.f,0 });
    display1.setPosition({ winW * xPerc,winH * 0.3f });
    display1.setOutlineThickness(scale * 0.01f);
    display1.setOutlineColor(Color::Black);
    display1.setFillColor(Color(0,0,0,185));
    window.draw(display1);
    float bigArrowSize = dSizeX / 32.f;
    FloatRect displayBounds = display1.getGlobalBounds();
    drawCharacter(window, displayBounds.position.x, displayBounds.position.y+dSizeX/4.f, bigArrowSize, character.size, character);

    //drawScrollArrows(window, displayBounds.getCenter().x, 0.2f, displayBounds.getCenter().y, bigArrowSize, bigA);
    arrows[4 * player].pos.x = displayBounds.getCenter().x;
    arrows[4 * player].pos.y = displayBounds.getCenter().y-dSizeX/4.f;
    arrows[4 * player].distPerc = 0.2f;
    arrows[4 * player].sizePerc = bigArrowSize;

    drawScrollArrows(window, arrows[4*player]);

    float smallArrowSize = dSizeX / 16.f;

    for (int i = 1; i < 4; i++) {
        arrows[4 * player + i].pos.x = xPerc * winW;
        arrows[4 * player + 1].pos.y = 0.725f * winH;
        arrows[4 * player + i].distPerc = 0.2f;
        arrows[4 * player + i].sizePerc = 0.005 * scale;
        arrows[4 * player + i].type = smallA;
    }

    ButtonConfig bodyTypeCfg = { "bodyType1",  xPerc, 0.725f, Color::White, 0.025f };

    arrows[4 * player + 1].pos.y = 0.725f*winH;
    drawScrollArrows(window, arrows[4 * player + 1]);
    string ccolor = "Color ";
    ccolor += to_string(character.bodyColor);
    ButtonConfig colorPaletteCfg = { ccolor.c_str(),  xPerc, 0.785f, Color::White, 0.025f};

    arrows[4 * player + 2].pos.y = 0.785f * winH;
    drawScrollArrows(window, arrows[4 * player + 2]);
    ButtonConfig accesoryCfg = { "accesory1",  xPerc, 0.845f, Color::White, 0.025f };

    arrows[4 * player + 3].pos.y = 0.845f * winH;
    drawScrollArrows(window, arrows[4 * player + 3]);

    drawStyledButton(window, font, bodyTypeCfg);
    drawStyledButton(window, font, colorPaletteCfg);
    drawStyledButton(window, font, accesoryCfg);
}

void drawCustomizationMenu(RenderWindow& window, GameState& state, arrowSet arrows[], Text& text, Font& font) {
    float winW = static_cast<float>(window.getSize().x);
    float winH = static_cast<float>(window.getSize().y);

    float scale = min(winW, winH) * 0.9f;

    if (state.gameMode == PVP) {
        drawPlrCustomize(window, state.character[P1], arrows, font, 0.25f, P1);
        drawPlrCustomize(window, state.character[P2], arrows, font, 0.75f, P2);
    }
    else {
        drawPlrCustomize(window, state.character[P1], arrows, font, 0.5f, P1);
    }

    ButtonConfig doneCfg = { "DONE CUSTOMIZING",  0.5f, 0.95f, Color::Red, 0.04f };
    drawStyledButton(window, font, doneCfg);

}