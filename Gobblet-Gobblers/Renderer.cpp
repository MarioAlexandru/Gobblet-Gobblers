#include "Renderer.h"
#include <iostream> // In case you need debug prints

using namespace sf;
using namespace std;

void drawCell(RenderWindow& window, const GameState& state, int line, int col) {
    const auto& T = state.T;
    float boardX = (window.getSize().x - tableSize) / 2;
    float boardY = (window.getSize().y - tableSize) / 2;

    float latura = tableSize / squareNumber * 1.f;

    int varf = T[line + 1][col + 1].nr;

    CircleShape piece(radius);

    switch (T[line + 1][col + 1].p[varf]) {
    case -3:
        piece.setFillColor(Color::Red);
        piece.setRadius(50);
        break;
    case -2:
        piece.setFillColor(Color::Red);
        piece.setRadius(40);
        break;
    case -1:
        piece.setFillColor(Color::Red);
        piece.setRadius(30);
        break;
    case 1:
        piece.setFillColor(Color::Green);
        piece.setRadius(30);
        break;
    case 2:
        piece.setFillColor(Color::Green);
        piece.setRadius(40);
        break;
    case 3:
        piece.setFillColor(Color::Green);
        piece.setRadius(50);
        break;
    }

    float pieceSize = piece.getRadius();

    piece.setPosition({ boardX + col * latura + latura / 2 - pieceSize, boardY + line * latura + latura / 2 - pieceSize });

    window.draw(piece);
}

void drawTable(RenderWindow& window, const GameState& state) {
    auto squareColor = Color::White;

    float boardX = (window.getSize().x - tableSize) / 2;
    float boardY = (window.getSize().y - tableSize) / 2;

    float latura = tableSize / squareNumber * 1.f;
    RectangleShape square;

    square.setSize({ latura,latura });
    square.setFillColor(squareColor);
    square.setOutlineThickness(5.f);
    square.setOutlineColor(Color::Black);
    for (int i = 0; i < squareNumber; i++)
        for (int j = 0; j < squareNumber; j++) {
            square.setPosition({ boardX + j * latura,boardY + i * latura });
            window.draw(square);

            drawCell(window, state, i, j);
        }

    square.setPosition({ boardX, boardY });
    square.setSize({ tableSize,tableSize });
    square.setOutlineThickness(10.f);
    square.setFillColor(Color::Transparent);
    square.setOutlineColor(Color::Red);
    window.draw(square);

}

void highlightSquare(RenderWindow& window, int col, int line) {
    float boardX = (window.getSize().x - tableSize) / 2;
    float boardY = (window.getSize().y - tableSize) / 2;
    float latura = tableSize / squareNumber;
    RectangleShape square;
    square.setSize({ latura,latura });
    square.setOutlineThickness(12.f);
    square.setOutlineColor(Color::Red);
    square.setPosition({ boardX + col * latura,boardY + line * latura });
    square.setFillColor(Color::Transparent);

    window.draw(square);
}

// THE NEW WRAPPER FUNCTION
void drawGame(RenderWindow& window, const GameState& state, Text& text) {
    drawTable(window, state);

    if (state.old_col != -20) {
        highlightSquare(window, state.old_col, state.old_line);
    }

    text.setString("Player " + to_string(state.player + 1));
    text.setPosition({ 100, 50 });
    window.draw(text);

    text.setString("Size: " + to_string(state.pieceSize));
    text.setPosition({ (float)tableSize + 150, 50 });
    window.draw(text);
}