#include "GameLogic.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <format>

using namespace std;

int sign(int x) { return (x > 0) - (x < 0); }

void initGame(GameState& state) {
    int piecesNr = 2; //default game rules

    state.matchState = STATE_PLAY;
    state.startTime = time(nullptr);
    state.totalElapsedSec = 0;
    
    //separate function would be good
    state.tb.Focused = false;
    state.tb.isEmpty = true;

    state.winner = -1;
    state.scoreP1 = 0;
    state.scoreP2 = 0;

    state.player = P1;
    state.pieceSize = 1;
    state.waitingForLeftClick = false;
    state.correctSelection = false;
    state.heldDown = false;
    state.old_line = -20;
    state.old_col = -20;

    state.board.size = 500.f;
    state.board.pos = { 0.f,0.f };
    //state.board.squareNr = 3;

    for (int i = 0; i <= squareNumber; i++) {
        for (int j = 0; j <= squareNumber; j++) {
            state.board.T[i][j].nr = 0;
            //state.T[i][j].nr = 0;
            for (int k = 0; k <= squareNumber; k++) {
                state.board.T[i][j].p[k] = 0;
                //state.T[i][j].p[k] = 0;
            }
        }
    }

    for (int k = 1; k < 4; k++) {
        state.pieces[P1][k] = { piecesNr };
        state.pieces[P2][k] = { piecesNr };
    }
}

void defaultCustomization(Character characters[]) {
    for (int p = P1; p <= P2; p++) {
        characters[p].bodyType = 0;
        characters[p].bodyColor = 0;
        characters[p].accessory = 0;
        characters[p].size = 1;

        characters[p].palette[0] = Color(177,62,63-40);
        characters[p].palette[1] = Color(239,125,87 - 40);
        characters[p].palette[2] = Color(255,205,117 - 40);
        characters[p].palette[3] = Color(167,240,112 - 40);
        characters[p].palette[4] = Color(56,183,100 - 40);
        characters[p].palette[5] = Color(37,113,121 - 40);
        characters[p].palette[6] = Color(41,54,111 - 40);
        characters[p].palette[7] = Color(93,39,93 - 40);
    }
}

void togglePause(GameState& state) {
    if (state.matchState == STATE_PAUSED) {
        // Resume: set new start time
        state.startTime = time(nullptr);
        if (state.scoreP1 != 0 || state.scoreP2 !=0) {
            state.matchState = STATE_WIN;
        }
        else {
            state.matchState = STATE_PLAY;
            printf("Jocul a fost reluat.\n");
        }
    }
    else {
        // Pause: accumulate elapsed time so far
        time_t now = time(nullptr);
        if (state.matchState == STATE_PLAY) {
            state.totalElapsedSec += (int)(now - state.startTime);
            printf("Jocul este acum intrerupt. Timp curent: %d sec.\n", state.totalElapsedSec);
        }
        state.matchState = STATE_PAUSED;
    }
}

int getCurrentPlayTime(const GameState& state) {
    if (state.matchState == STATE_PAUSED || state.matchState == STATE_WIN) {
        return state.totalElapsedSec; // no change while paused
    }
    else {
        time_t now = time(nullptr);
        return state.totalElapsedSec + (int)(now - state.startTime);
    }
}

bool punePiesa(GameState& state, int linia, int coloana, int piece, bool movingPiece) {
    auto& T = state.board.T;
    int player = P1;
    if (piece < 0)
        player = P2;
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

                return true;
            }
        }
    }

}

bool movePiece(GameState& state, int old_l, int old_c, int new_l, int new_c) {
    auto& T = state.board.T;
    int varf = T[old_l + 1][old_c + 1].nr;

    int pieceVal = T[old_l + 1][old_c + 1].p[varf];
    if (punePiesa(state, new_l + 1, new_c + 1, pieceVal, true)) {
        T[old_l + 1][old_c + 1].p[varf] = 0;
        T[old_l + 1][old_c + 1].nr--;
        //printf("Piesa mutata cu succes de pe pozitia %d %d pe %d %d.\n", old_l+1,old_c+1,new_l+1,new_c+1);
        return true;
    }
    return false;
}

void gameWon(GameState& state, int winner) {
    printf("Player %d Wins!\n", winner + 1);
    time_t now = time(nullptr);
    state.totalElapsedSec += (int)(now - state.startTime);
    //
    updateScores(state, winner);
    printf("Player 1 scored: %d points!\n", state.scoreP1);
    printf("Player 2 scored: %d points!\n", state.scoreP2);
    state.matchState = STATE_WIN;
    state.winner = winner;
    //window.close();
}

int checkWin(GameState& state, bool returnWin) {
    const auto& T = state.board.T;
    int winner = -1;

    for (int i = 1; i <= 3; i++) {
        if ((T[i][1].nr && T[i][2].nr && T[i][3].nr)
            && sign(T[i][1].p[T[i][1].nr]) == sign(T[i][2].p[T[i][2].nr])
            && sign(T[i][2].p[T[i][2].nr]) == sign(T[i][3].p[T[i][3].nr])) {
            winner = ((sign(T[i][1].p[T[i][1].nr])) == -1) ? P2 : P1;
        }
        else if ((T[1][i].nr && T[2][i].nr && T[3][i].nr) 
            && sign(T[1][i].p[T[1][i].nr]) == sign(T[2][i].p[T[2][i].nr]) 
            && sign(T[2][i].p[T[2][i].nr]) == sign(T[3][i].p[T[3][i].nr]))
            winner = ((sign(T[1][i].p[T[1][i].nr])) == -1) ? P2 : P1;
    }

    if ((T[1][1].nr && T[2][2].nr && T[3][3].nr) && sign(T[1][1].p[T[1][1].nr]) == sign(T[2][2].p[T[2][2].nr]) && sign(T[2][2].p[T[2][2].nr]) == sign(T[3][3].p[T[3][3].nr])
        || (T[1][3].nr && T[2][2].nr && T[3][1].nr) && sign(T[1][3].p[T[1][3].nr]) == sign(T[2][2].p[T[2][2].nr]) && sign(T[2][2].p[T[2][2].nr]) == sign(T[3][1].p[T[3][1].nr]))
        winner = ((sign(T[2][2].p[T[2][2].nr])) == -1) ? P2 : P1;
    if (winner != -1 && returnWin) {
        gameWon(state, winner);
    }
    return winner; //no one won yet
}

int evaluate(GameState& state, int player) {
    if (checkWin(state, false) == player)
        return 10000;
    else if (checkWin(state, false) == (1 - player))
        return -10000;

    int score = 0;

    // Bonus for each line (row/col/diag) where player has top piece
    // and opponent doesn't ? or has smaller piece.
    int lines[8][3][2] = {
        {{1,1},{1,2},{1,3}}, {{2,1},{2,2},{2,3}}, {{3,1},{3,2},{3,3}}, // rows
        {{1,1},{2,1},{3,1}}, {{1,2},{2,2},{3,2}}, {{1,3},{2,3},{3,3}}, // cols
        {{1,1},{2,2},{3,3}}, {{1,3},{2,2},{3,1}}  // diags
    };

    auto& T = state.board.T;

    for (int i = 0; i < 8; i++) {
        int own = 0, opp = 0;
        for (int j = 0; j < 3; j++) {
            int r = lines[i][j][0], c = lines[i][j][1];
            if (T[r][c].nr > 0) {
                int top = T[r][c].p[T[r][c].nr];
                if (top > 0 && player == P1) own++;
                else if (top < 0 && player == P2) own++;
                else opp++;
            }
        }
        if (own == 3) return 10000;
        if (opp == 3) return -10000;
        score += own * 10 - opp * 10;
    }

    // Bonus for larger pieces on board & center control
    int center = T[2][2].nr > 0 ? T[2][2].p[T[2][2].nr] : 0;
    if ((center > 0 && player == P1) || (center < 0 && player == P2))
        score += abs(center) * 5;

    return score;
}

void updateScores(GameState& state, int winner) {
    //Bonus: count unused pieces
    int bonusP1 = 0, bonusP2 = 0;

    for (int s = 1; s <= pieceTypes; s++) {
        bonusP1 += state.pieces[P1][s];
        bonusP2 += state.pieces[P2][s];
    }

    state.scoreP1 += (winner == P1) ? 100 : 0;
    state.scoreP2 += (winner == P2) ? 100 : 0;

    //Time bonus
    if (winner == P1) {
        state.scoreP1 += max(0, 60 - state.totalElapsedSec);
        state.scoreP2 -= max(0, 60 - state.totalElapsedSec);
    }
    else {
        state.scoreP1 -= max(0, 60 - state.totalElapsedSec);
        state.scoreP2 += max(0, 60 - state.totalElapsedSec);
    }

}

//Computer Moves

int generateMoves(GameState& state, Move out[], int maxMoves) {
    int player = state.player;
    int pieceSign = (player == P1) ? 1 : -1;

    int count = 0;

    // Generate valid placements
    for (int size = 1; size <= 3; ++size) {
        if (state.pieces[player][size] > 0) {
            int piece = pieceSign * size;
            for (int r = 1; r <= squareNumber; ++r) {
                for (int c = 1; c <= squareNumber; ++c) {
                    stiva& cell = state.board.T[r][c];
                    if (cell.nr >= 3) continue;
                    if (cell.nr == 0 || abs(piece) > abs(cell.p[cell.nr])) {
                        if (count < maxMoves) {
                            out[count].type = 0;
                            out[count].a = r;
                            out[count].b = c;
                            out[count].c = piece;
                            count++;
                        }
                    }
                }
            }
        }
    }

    // Generate valid moves
    for (int old_r = 1; old_r <= squareNumber; ++old_r) {
        for (int old_c = 1; old_c <= squareNumber; ++old_c) {
            stiva& src = state.board.T[old_r][old_c];
            if (src.nr == 0) continue;
            int topPiece = src.p[src.nr];
            // Check ownership
            if ((topPiece > 0 && player != P1) || (topPiece < 0 && player != P2))
                continue;

            for (int new_r = 1; new_r <= squareNumber; ++new_r) {
                for (int new_c = 1; new_c <= squareNumber; ++new_c) {
                    if (old_r == new_r && old_c == new_c) continue;
                    stiva& dst = state.board.T[new_r][new_c];
                    if (dst.nr >= 3) continue;
                    if (dst.nr == 0 || abs(topPiece) > abs(dst.p[dst.nr])) {
                        if (count < maxMoves) {
                            out[count].type = 1;
                            out[count].a = old_r;
                            out[count].b = old_c;
                            out[count].c = new_r;
                            out[count].d = new_c;
                            count++;
                        }
                    }
                }
            }
        }
    }

    return count;
}

bool executeMove(GameState& state, Move move) {
    if (move.type == 0) {
        // Placement: (r, c, piece)
        return punePiesa(state, move.a, move.b, move.c, false);
    }
    else {
        // Move: (old_r, old_c) (new_r, new_c)
        // movePiece uses 0 index
        return movePiece(state, move.a - 1, move.b - 1, move.c - 1, move.d - 1);
    }
}

int minimax(GameState state, int depth, bool isMaximizing, int player) {
    int winner = checkWin(state, false);
    if (winner == player) return 10000;
    if (winner == 1 - player) return -10000;
    if (depth == 0) return evaluate(state, player);

    Move moves[100];
    int n = generateMoves(state, moves, 100);
    if (n == 0) return evaluate(state, player);

    if (isMaximizing) {
        int best = -100000;
        for (int i = 0; i < n; i++) {
            GameState next = state;
            executeMove(next, moves[i]);
            next.player = 1 - next.player;
            int val = minimax(next, depth - 1, false, player);
            if (val > best) best = val;
        }
        return best;
    }
    else {
        int best = 100000;
        for (int i = 0; i < n; i++) {
            GameState next = state;
            executeMove(next, moves[i]);
            next.player = 1 - next.player;  // if needed
            int val = minimax(next, depth - 1, true, player);
            if (val < best) best = val;
        }
        return best;
    }
}

Move chooseBestMove(GameState& state, int depth) {
    Move moves[100];
    int n = generateMoves(state, moves, 100);
    if (n == 0) { /* handle */ }

    int bestVal = -100000;
    int bestIdx = 0;
    for (int i = 0; i < n; i++) {
        GameState next = state;
        executeMove(next, moves[i]);
        next.player = 1 - next.player;
        int val = minimax(next, depth - 1, false, state.player);
        if (val > bestVal) {
            bestVal = val;
            bestIdx = i;
        }
    }
    return moves[bestIdx];
}

bool makeRandomMove(GameState& state) {
    int player = state.player;

    const int maxMoves = 100;
    Move options[maxMoves];

    int count = generateMoves(state, options, maxMoves);

    //Choose between place and move
    if (count == 0) {
        printf("Nici o mutare valida disponibila pentru jucatorul %d.\n", player + 1);
        return false;
    }

    int idx = rand() % count;
    Move& opt = options[idx];

    executeMove(state, opt);
}

//Gama Data functions
bool saveGameState(GameState& state, const char* filename) {
    ofstream saveFile(filename);
    if (!saveFile) return false;

    //Game mode
    saveFile << state.gameMode << '\n';

    //Player turn
    saveFile << state.player << '\n';

    //Board order
    for (int i = 1; i <= squareNumber; i++)
        for (int j = 1; j <= squareNumber; j++) {
            const auto& cell = state.board.T[i][j];

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

    //time elapsed
    saveFile << state.totalElapsedSec << '\n';

    //Character customizations
    for (int p = P1; p <= P2; p++) {
        saveFile << state.character[p].bodyType << '\n';
        saveFile << state.character[p].bodyColor << '\n';
        saveFile << state.character[p].accessory << '\n';
    }

    return true;
}

bool loadGameState(GameState& state, const char* filename) {
    ifstream file(filename);

    //read game mode
    int modeInt;
    if (!(file >> modeInt)) return false;
    state.gameMode = static_cast<GameMode>(modeInt);

    //read player
    if (!(file >> state.player)) return false;

    //read board cells in order
    for (int i = 1; i <= squareNumber; i++) {
        for (int j = 1; j <= squareNumber; j++) {
            auto& cell = state.board.T[i][j];

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

    if (!(file >> state.totalElapsedSec)) return false;

    for (int p = P1; p <= P2; p++) {
        if (!(file >> state.character[p].bodyType)) return false;
        if (!(file >> state.character[p].bodyColor)) return false;
        if (!(file >> state.character[p].accessory)) return false;
    }

    return true;
}

int getCurrentDateAsInt() {
    using namespace std::chrono;
    // Get current time point
    auto now = system_clock::now();

    // Get local time zone
    auto local_tz = current_zone();

    // Convert to local time
    auto local_time = local_tz->to_local(now);

    // Truncate to days
    auto local_days = floor<days>(local_time);

    // Convert to year_month_day
    year_month_day ymd{ local_days };

    int day = static_cast<unsigned>(ymd.day());
    int month = static_cast<unsigned>(ymd.month());
    int year = static_cast<int>(ymd.year());

    return day * 1000000 + month * 10000 + year;
}
