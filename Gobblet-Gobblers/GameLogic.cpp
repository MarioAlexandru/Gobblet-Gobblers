#include "GameLogic.h"
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int sign(int x) { return (x > 0) - (x < 0); }

void initGame(GameState& state) {
    int piecesNr = 2; //default game rules

    state.player = P1;
    state.pieceSize = 1;
    state.waitingForLeftClick = false;
    state.correctSelection = false;
    state.old_line = -20;
    state.old_col = -20;

    for (int i = 0; i <= squareNumber; i++) {
        for (int j = 0; j <= squareNumber; j++) {
            state.T[i][j].nr = 0;
            for (int k = 0; k <= squareNumber; k++) {
                state.T[i][j].p[k] = 0;
            }
        }
    }

    for (int k = 1; k < 4; k++) {
        state.pieces[P1][k] = { piecesNr };
        state.pieces[P2][k] = { piecesNr };
    }
}

bool punePiesa(GameState& state, int linia, int coloana, int piece) {
    auto& T = state.T;
    auto& player = state.player;
    auto movingPiece = state.correctSelection;
    auto& pieces = state.pieces;
    int pieceSize = abs(piece);

    if (pieces[player][pieceSize] <= 0 && !movingPiece) {
        printf("Nu mai sunt piese de dimensiunea %d!\n", pieceSize);
        return false;
    }
    else {
        if (T[linia][coloana].nr == squareNumber) {
            printf("Nu se poate amplasa, deoarece sunt deja 3 piese.\n");
            return false;
        }
        else
        {
            int varf = T[linia][coloana].nr;
            int y = T[linia][coloana].p[varf];
            if (abs(piece) <= abs(y)) {
                printf("Nu se poate amplasa decat o piesa mai mare peste o alta piesa.\n");
                return false;
            }
            else
            {
                T[linia][coloana].nr++;
                T[linia][coloana].p[T[linia][coloana].nr] = piece;

                if (!movingPiece)
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
        printf("Piesa mutata cu succes.\n");
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

//Gama Data functions
bool saveGameState(GameState& state, const char* filename) {
    ofstream saveFile(filename);
    if (!saveFile) return false;

    //Player turn
    saveFile << state.player << '\n';

    //Board order
    for(int i = 1; i <= squareNumber; i++)
        for (int j = 1; j <= squareNumber; j++) {
            const auto &cell = state.T[i][j];

            saveFile << cell.nr;
            for (int k = 1; k <= cell.nr; k++) {
                saveFile << ' ' << cell.p[k];
            }

            saveFile << '\n';
        }
    for (int p = P1; p <= P2; p++) {
        for (int i = 1; i <= pieceTypes; i++)
            saveFile << state.pieces[p][i] << ' ';
        saveFile << '\n';
    }

    return true;
}

bool loadGameState(GameState& state, const char* filename) {
    ifstream file(filename);

    //read player
    if (!(file >> state.player)) return false;

    //read board cells in order
    for (int i = 1; i <= squareNumber; i++) {
        for (int j = 1; j <= squareNumber; j++) {
            auto& cell = state.T[i][j];

            if (!(file >> cell.nr)) return false;
            if (cell.nr < 0 || cell.nr > pieceTypes) return false;

            for (int k = 1; k <= cell.nr; k++) {
                if (!(file >> cell.p[k])) return false;
            }
            //zero for unused
            for (int k = cell.nr + 1; k <= pieceTypes; k++) {
                cell.p[k] = 0;
            }
        }
    }
    //read each player's remaining pieces
    for (int p = P1; p <= P2; p++) {
        for (int k = 1; k <= pieceTypes; k++)
            if (!(file >> state.pieces[p][k])) return false;
    }

    return true;
}