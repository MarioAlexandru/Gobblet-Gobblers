#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

// Constants
#define pieceTypes 3
#define squareNumber 3
#define P1 0
#define P2 1


enum AppState {
    STATE_MENU,
    STATE_GAME,
    STATE_SETTINGS,
    STATE_WIN
};

// The Piece Stack
struct stiva {
    int p[pieceTypes+1];
    int nr;
};

struct GameState {
    // Board Data
    stiva T[squareNumber+1][squareNumber+1];
    int player;
    int pieces[2][4];

    // Input/State Data
    int pieceSize;
    int old_line;
    int old_col;
    bool waitingForLeftClick;
    bool correctSelection;

    AppState appState;
};

struct ButtonConfig {
    const char* text;
    float yPerc;
    sf::Color color;
    float sizePerc;
};