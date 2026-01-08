#pragma once
#include "GameDefs.h"

void initGame(GameState& state);
void defaultCustomization(Character characters[]);
void togglePause(GameState& state);
int getCurrentPlayTime(const GameState& state);
int sign(int x); // Helper
//game functions
bool punePiesa(GameState& state, int linia, int coloana, int piece, bool movingPiece);
bool movePiece(GameState& state, int old_l, int old_c, int new_l, int new_c);
int checkWin(GameState& state, bool returnWin);
int evaluate(GameState& state, int player);
void updateScores(GameState& state, int winner);
bool executeMove(GameState& state, Move move);
Move chooseBestMove(GameState& state, int depth);
//computer functions
bool makeRandomMove(GameState& state);
//data functions
bool saveGameState(GameState& state, const char* filename);
bool loadGameState(GameState& state, const char* filename);