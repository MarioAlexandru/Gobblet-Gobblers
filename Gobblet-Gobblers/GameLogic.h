#pragma once
#include "GameDefs.h"

void initGame(GameState& state);
int sign(int x); // Helper
//game functions
bool punePiesa(GameState& state, int linia, int coloana, int x);
void movePiece(GameState& state, int old_l, int old_c, int new_l, int new_c);
bool checkWin(const GameState& state);
//data functions
bool saveGameState(GameState& state, const char* filename);
bool loadGameState(GameState& state, const char* filename);