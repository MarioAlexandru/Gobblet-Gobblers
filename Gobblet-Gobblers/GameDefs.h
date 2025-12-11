#pragma once

// Constants
#define tableSize 600
#define squareNumber 3
#define radius 30
#define P1 0
#define P2 1

// The Piece Stack
struct stiva {
    int p[4];
    int nr;
};

struct GameState {
    // Board Data
    stiva T[4][4];
    int player;

    // Input/State Data
    int pieces[2][4]; //player pieces array

    int pieceSize;
    int old_line;
    int old_col;
    bool waitingForLeftClick;
    bool correctSelection;
};