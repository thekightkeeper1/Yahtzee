#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "game.h"

Yahtzee init_yahtzee(int numPlayers, u_int64_t isAI) {
    const Yahtzee y = {
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

// Rolls the dice in the yahtzee game.
void rollDice(Yahtzee* y) {
    // Handles all dice rolling logic, including resetting locked dice if we are on 0 rolls
    // And advancing the number of rolls

    // Check we can roll, then increment
    assert(y->currentRoll < MAX_ROLLS);  // Game driver should stop this from failing
    y->currentRoll++;
    if (y->currentRoll == 0) y->locked_dice = 0;

    // Iterating bits of locked dice, and only rolling the correct ones
    for (int i = 0; i < 5; i++) {
        if (~(y->locked_dice >> i) & 0b1) { // If the bit at the ith position from the right is toggled
            y->dice[i] = 1 + rand() % 6;
        }
    }
}

// Updates the ephemeral scoreboard during rolls
void update_ephemeral(Yahtzee* y) {

    assert(y->curPlayer < y->numPlayers);

    // iterate current player
    // If they already have a score, then we skip
    // Else if they dont have a score, then we calculate it from the current ro
}