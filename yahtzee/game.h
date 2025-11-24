#ifndef GAME_H
#define GAME_H

#define NUM_INTERACTIVE_CATEGORIES 14 /* The number of scores the user can themselves choose */
#define NUM_CATEGORIES 17 /* Same as above, but includes the total rows, and things like the bonus */

#include <stdlib.h>

typedef enum {
    ONES,
    TWOS,
    THREES,
    FOURS,
    FIVES,
    SIXES,
    THREE_OF,
    FOUR_OF,
    FULL_HOUSE,
    SM_STRAIGHT,
    LG_STRAIGHT,

    // This begins the none-interative scores
    UPPER_TOTAL,
    LOWER_TOTAL,
    COMPLETE_TOTAL,
    YAHTZEE // Yahtzee is considered interactive, but not a required score. It's weird
} CATEGORIES;

typedef struct {

    /* State for the entire game */
    int numPlayers;
    int** scores;  // First index is the player, second is the particular score

    /* Ephemeral state for each turn */
    int* bufferScore; // The theoretical score you can get for each category between rolls
    int curPlayer;  // The index of the turn player
    int dice[5];
    int currentRoll; // Shows how many times the dice have been rolled this turn

} Yahtzee;

/* Functions */
Yahtzee init_yahtzee(int numPlayers, u_int64_t isAI);
void end_yahtzee(const Yahtzee y);

#endif // GAME_H
