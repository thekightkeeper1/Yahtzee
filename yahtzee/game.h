#ifndef GAME_H
#define GAME_H

#define NUM_INTERACTIVE_CATEGORIES 12 /* The number of scores the user can themselves choose */
#define NUM_CATEGORIES 15 /* Same as above, but includes the total rows, and things like the bonus */

#define MAX_ROLLS 3
#define NUM_DICE 5
#define NUM_DIE_FACES 6

#define LEN_LG_STRAIGHT 4
#define LEN_SM_STRAIGHT 3

#define POINTS_FULL_HOUSE 25
#define POINTS_SM_STRAIGHT 30
#define POINTS_LG_STRAIGHT 40
#define POINTS_YAHTZEE 50

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
    YAHTZEE, /* Yahtzee is considered interactive, but not a required score. It's weird */

    // This begins the none-interactive scores
    UPPER_TOTAL,
    LOWER_TOTAL,
    COMPLETE_TOTAL,
} CATEGORIES;

#define DIE_1 0b00001
#define DIE_2 0b00010
#define DIE_3 0b00100
#define DIE_4 0b01000
#define DIE_5 0b10000

typedef struct {

    /* State for the entire game */
    int numPlayers;
    int** scores;  // First index is the player, second is the particular score

    /* Ephemeral state for each turn */
    int* bufferScore; // The theoretical score you can get for each category between rolls
    int curPlayer;  // The index of the turn player
    int dice[NUM_DICE];
    int currentRoll; // Shows how many times the dice have been rolled this turn
    u_int8_t locked_dice;

} Yahtzee;

/* Functions */
Yahtzee init_yahtzee(int numPlayers, u_int64_t isAI);
void end_yahtzee(const Yahtzee y);
void rollDice(Yahtzee* y);
void update_ephemeral(Yahtzee* y);
#endif // GAME_H
