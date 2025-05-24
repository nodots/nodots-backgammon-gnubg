#include "backgammon.h"
#include "positionid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "drawboard.h"

typedef struct {
    char move[128];
    float equity;
    float eqdiff;
} MoveInfo;

#define MAX_MOVES 32
MoveInfo moves[MAX_MOVES];
int move_count = 0;

int collect_move_callback(procrecorddata *pr) {
    char szMove[128];
    int index = (int)(ptrdiff_t)pr->avOutputData[PROCREC_HINT_ARGOUT_INDEX];
    const matchstate *pms = pr->avOutputData[PROCREC_HINT_ARGOUT_MATCHSTATE];
    const movelist *pml = pr->avOutputData[PROCREC_HINT_ARGOUT_MOVELIST];
    float rEq = pml->amMoves[index].rScore;
    float rEqTop = pml->amMoves[0].rScore;
    float rEqDiff = rEq - rEqTop;

    FormatMove(szMove, (ConstTanBoard)pms->anBoard, pml->amMoves[index].anMove);

    if (move_count < MAX_MOVES) {
        strncpy(moves[move_count].move, szMove, sizeof(moves[move_count].move) - 1);
        moves[move_count].move[sizeof(moves[move_count].move) - 1] = '\0';
        moves[move_count].equity = rEq;
        moves[move_count].eqdiff = rEqDiff;
        move_count++;
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <PositionID> <dice1> <dice2> <turn> [num_moves]\n", argv[0]);
        return 1;
    }
    const char *pos_id = argv[1];
    int dice1 = atoi(argv[2]);
    int dice2 = atoi(argv[3]);
    int turn = atoi(argv[4]);
    int num_moves = (argc > 5) ? atoi(argv[5]) : 10;
    if (num_moves > MAX_MOVES) num_moves = MAX_MOVES;

    TanBoard anBoard;
    procrecorddata pr = {0};

    if (PositionFromID(anBoard, pos_id) != 0) {
        fprintf(stderr, "Invalid Position ID!\n");
        return 1;
    }

    memcpy(ms.anBoard, anBoard, sizeof(TanBoard));
    ms.anDice[0] = dice1;
    ms.anDice[1] = dice2;
    ms.fTurn = turn;
    ms.gs = GAME_PLAYING;

    pr.pfProcessRecord = collect_move_callback;

    char num_moves_str[8];
    snprintf(num_moves_str, sizeof(num_moves_str), "%d", num_moves);
    move_count = 0;
    hint_move(num_moves_str, 0, &pr);

    printf("[\n");
    for (int i = 0; i < move_count; ++i) {
        printf("  {\"move\": \"%s\", \"equity\": %.5f, \"eqdiff\": %.5f}%s\n",
            moves[i].move, moves[i].equity, moves[i].eqdiff,
            (i == move_count - 1) ? "" : ",");
    }
    printf("]\n");

    return 0;
}