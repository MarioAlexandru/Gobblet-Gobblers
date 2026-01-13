#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

using namespace sf;

// Constants
#define pieceTypes 3
#define squareNumber 3
#define P1 0
#define P2 1

//arrow sprites
#define bigA 0
#define smallA 1
#define leftA 0
#define rightA 1

#define FRAME_LIMIT 60

enum GameMode {
    PVP,
    PVE_RAND,
    PVE_MIMIMAX
};

enum AppState {
    STATE_MENU,
    STATE_SELECT_MODE,
    STATE_SELECT_DIFFICULTY,
    STATE_CUSTOMIZATION,
    STATE_SAVE_HIGHSCORE,
    STATE_GAME,
    STATE_SETTINGS
};

enum MatchState {
    STATE_PLAY,
    STATE_PAUSED,
    STATE_WIN
};

// The Piece Stack
struct stiva {
    int p[pieceTypes+1];
    int nr;
};

struct boardObj {
    stiva T[5][5];
    int squareNr;
    Vector2f pos;
    float size;
};

//possible moves structure
struct Move {
    int type; // 0=place, 1=move
    int a, b, c, d; // a, b, c for place & a,b,c,d for move
};

struct Character {
    int bodyType;
    int bodyColor;
    Color palette[8];
    int accessory;
    int size;
};

struct TextBox {
    Vector2f pos;//percentage
    Vector2f size;//percentage
    bool Focused;
    bool isEmpty;
};

struct GameState {
    // Board Data
    boardObj board;
    //stiva T[squareNumber+1][squareNumber+1];
    int player;
    int pieces[2][4];

    // Input/State Data
    int pieceSize;
    int old_line;
    int old_col;
    bool waitingForLeftClick;
    bool correctSelection;
    bool heldDown;

    //score data
    int winner;
    int scoreP1, scoreP2;
    time_t startTime;
    int totalElapsedSec;

    //player data
    String name[2];
    Character character[2];
    TextBox tb;

    AppState appState;
    GameMode gameMode;
    MatchState matchState;
};

struct ButtonConfig {
    const char* text;
    float xPerc;
    float yPerc;
    sf::Color color;
    float sizePerc;
};

struct arrowSet {
    Vector2f pos;
    float distPerc;
    float sizePerc;
    int type;
};
