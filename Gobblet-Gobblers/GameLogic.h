#pragma once
#include "GameDefs.h"

void initGame(GameState& state);
int sign(int x); // Helper
bool punePiesa(GameState& state, int linia, int coloana, int x);
void movePiece(GameState& state, int old_l, int old_c, int new_l, int new_c);
bool checkWin(const GameState& state);