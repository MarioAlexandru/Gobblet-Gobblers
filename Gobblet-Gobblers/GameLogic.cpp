#include "GameLogic.h"
#include <iostream>
#include <cmath>

using namespace std;

int sign(int x) { return (x > 0) - (x < 0); }

void initGame(GameState& state) {
    state.player = P1;
    state.pieceSize = 1;
    state.waitingForLeftClick = false;
    state.correctSelection = false;
    state.old_line = -20;
    state.old_col = -20;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            state.T[i][j].nr = 0;
            for (int k = 0; k < 4; k++) {
                state.T[i][j].p[k] = 0;
            }
        }
    }

    for (int i = 1; i < 4; i++) {
        state.pieces[P1][i] = { 2 };
        state.pieces[P2][i] = { 2 };
    }
}

bool punePiesa(GameState& state, int linia, int coloana, int piece) {
    auto& T = state.T;
    auto& player = state.player;
    auto& pieces = state.pieces;
    auto& movingPiece = state.correctSelection;
    int pieceSize = abs(piece);

    if (pieces[player][pieceSize] <= 0 && !movingPiece) {
        printf("Nu mai aveti piese de dimensiunea %d!", pieceSize);
        return false;
    }
    else {
        if (T[linia][coloana].nr == 3) {
            cout << "Nu se poate amplasa, deoarece sunt deja 3 piese.";
            return false;
        }
        else
        {
            int varf = T[linia][coloana].nr;
            int y = T[linia][coloana].p[varf];
            if (abs(piece) <= abs(y)) {
                cout << "Nu se poate amplasa decat o piesa mai mare peste o alta piesa.";
                return false;
            }
            else
            {
                T[linia][coloana].nr++;
                T[linia][coloana].p[T[linia][coloana].nr] = piece;
                if(!movingPiece)
                    pieces[player][pieceSize]--;
                if (player == P1)
                    player = P2;
                else
                    player = P1;
                return true;
            }
        }
    }
}

void movePiece(GameState& state, int old_l, int old_c, int new_l, int new_c) {
    auto& T = state.T;
    int varf = T[old_l + 1][old_c + 1].nr;

    int pieceVal = T[old_l + 1][old_c + 1].p[varf];
    if (punePiesa(state, new_l + 1, new_c + 1, pieceVal)) {
        T[old_l + 1][old_c + 1].p[varf] = 0;
        T[old_l + 1][old_c + 1].nr--;
        printf("Piece succesfully moved");
    }
}

bool checkWin(const GameState& state) {
    const auto& T = state.T;

    for (int i = 1; i <= 3; i++) {
        if ((T[i][1].nr && T[i][2].nr && T[i][3].nr) && sign(T[i][1].p[T[i][1].nr]) == sign(T[i][2].p[T[i][2].nr]) && sign(T[i][2].p[T[i][2].nr]) == sign(T[i][3].p[T[i][3].nr]))
            return true;
        else if ((T[1][i].nr && T[2][i].nr && T[3][i].nr) && sign(T[1][i].p[T[1][i].nr]) == sign(T[2][i].p[T[2][i].nr]) && sign(T[2][i].p[T[2][i].nr]) == sign(T[3][i].p[T[3][i].nr]))
            return true;
    }

    if ((T[1][1].nr && T[2][2].nr && T[3][3].nr) && sign(T[1][1].p[T[1][1].nr]) == sign(T[2][2].p[T[2][2].nr]) && sign(T[2][2].p[T[2][2].nr]) == sign(T[3][3].p[T[3][3].nr])
        || (T[1][3].nr && T[2][2].nr && T[3][1].nr) && sign(T[1][3].p[T[1][3].nr]) == sign(T[2][2].p[T[2][2].nr]) && sign(T[2][2].p[T[2][2].nr]) == sign(T[3][1].p[T[3][1].nr]))
        return true;
    return false;
}