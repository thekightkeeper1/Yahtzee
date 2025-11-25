#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "game.h"

Yahtzee init_yahtzee(const int numPlayers, u_int64_t isAI) {
    const Yahtzee y = {

        .numPlayers = numPlayers,
        .scores = calloc(numPlayers, sizeof(int*)),

        // Ephemeral state
        .bufferScore = calloc(NUM_INTERACTIVE_CATEGORIES , sizeof(int)),
        .curPlayer = 0,
        .dice = {0},
        .currentRoll = 0
    };

    for (int n = 0; n < numPlayers; n++) {
        y.scores[n] = calloc(NUM_CATEGORIES, sizeof(int));
    }
    return y;
}

// Frees ANY memory allocated by this library
void end_yahtzee(const Yahtzee y) {
    for (int n = 0; n < y.numPlayers; n++) {
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
    update_ephemeral(y);
}

bool has_full_house(int occurrences[NUM_DIE_FACES]) {

    // A full house should have exactly 3 of one dice, and 2 of another
    int three = 0;
    int two = 0;
    for (int face = 0; face < NUM_DIE_FACES; face++) {

        if (occurrences[face] == 2) { //
            two++;
        } else if (occurrences[face] == 3) {
            three++;
        }
    }

    if (two == 1 && three == 1) {
        return true;
    }
    return false;
}

bool is_continuous(int occurrences[NUM_DIE_FACES], int start, int length) {

    assert(start < NUM_DIE_FACES);
    assert(start + length <= NUM_DIE_FACES);

    bool isContinuous = true;
    for (int face = 0; face < start+length; face++) {
        if (occurrences[face] == 0) return false;
    }
    return true;
}

// Returns whether we have a straight of at least 4
bool has_large_straight(int occurrences[NUM_DIE_FACES]) {
    for (int curStart = 0; curStart <= NUM_DIE_FACES - LEN_LG_STRAIGHT; curStart++) {
       if (is_continuous(occurrences, curStart, LEN_LG_STRAIGHT)) return true;
    }
    return false;
}

// Returns whether we have a straight of at least 4
bool has_small_straight(int occurrences[NUM_DIE_FACES]) {
    for (int curStart = 0; curStart <= NUM_DIE_FACES - LEN_SM_STRAIGHT; curStart++) {
        if (is_continuous(occurrences, curStart, LEN_SM_STRAIGHT)) return true;
    }
    return false;
}


// Returns the highest face with 3, and the highest face with 4 occurrences
void of_a_kind_faces(int occurrences[NUM_DIE_FACES], int* three, int* four) {
    *three = -1;
    *four = -1;
    for (int face = 0; face < NUM_DIE_FACES; face++) {
        if (occurrences[face] >= 3) *three = face;
        if (occurrences[face] >= 4) *four = face;
    }
}

bool has_yahtzee(int occurrences[NUM_DIE_FACES]) {

    for (int face = 0; face <= NUM_DIE_FACES; face++) {
        if (occurrences[face] == 5) return true;
    }
    return false;
}

// Updates the ephemeral scoreboard during rolls
void update_ephemeral(Yahtzee* y) {

    assert(y->curPlayer < y->numPlayers);
    // First we tally up all the face values
    int diceOccurrences[NUM_DIE_FACES] = {0};
    for (int curDie = 0; curDie< NUM_DICE; curDie++) {
        const int dieValue = y->dice[curDie];
        diceOccurrences[dieValue-1]++;
    }
    // calculating the upper scores
    for (int cat = ONES; cat < SIXES+1; cat++) {
        y->bufferScore[cat] = diceOccurrences[cat] * (1 + cat); // Dice faces are 1 index, but cat is 0 index
    }

    // Computing the lower score section
    // Of a kind scores
    int threeFace, fourFace;
    of_a_kind_faces(diceOccurrences, &threeFace, &fourFace);
    y->bufferScore[THREE_OF] = threeFace > -1 ?  (threeFace+1) * LEN_SM_STRAIGHT : 0;
    y->bufferScore[FOUR_OF]  = fourFace  > -1 ?  (fourFace+1)  * LEN_LG_STRAIGHT : 0;

    // Full house
    printf("we should have a full house because we have a yahtzee: %s\n", has_yahtzee(diceOccurrences) ? "true" : "false");
    y->bufferScore[FULL_HOUSE] = has_full_house(diceOccurrences) || has_yahtzee(diceOccurrences) ? POINTS_FULL_HOUSE : 0;

    // Straights/yahtzee
    y->bufferScore[LG_STRAIGHT] = has_large_straight(diceOccurrences) ? POINTS_LG_STRAIGHT : 0;
    y->bufferScore[SM_STRAIGHT] = has_small_straight(diceOccurrences) ? POINTS_SM_STRAIGHT : 0;
    y->bufferScore[YAHTZEE] = has_yahtzee(diceOccurrences) ? POINTS_YAHTZEE : 0;

    // Totals:


}