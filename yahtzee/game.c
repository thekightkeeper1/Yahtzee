#include <stdio.h>
#include <stdlib.h>

#include "game.h"

Yahtzee init_yahtzee(int numPlayers, u_int64_t isAI) {
    Yahtzee y = {
        .numPlayers = numPlayers,
        .scores = calloc(numPlayers, sizeof(int*)),

        .bufferScore = malloc(NUM_CATEGORIES * sizeof(int)),
        .curPlayer = 0,
        .dice = {0},
        .currentRoll = 0
    };

    for (int n = 0; n < NUM_CATEGORIES; n++) {
        y.scores[n] = calloc(NUM_CATEGORIES, sizeof(int));
    }
    return y;
}

// Frees ANY memory allocated by this library
void end_yahtzee(const Yahtzee y) {
    for (int n = 0; n < NUM_CATEGORIES; n++) {
        free(y.scores[n]);
    }
    free(y.scores);
    free(y.bufferScore);
}

// Rolls the dice in the yahtzee game