#include <stdio.h>
#include <stdlib.h>

#define NUM_INTERACTIVE_CATEGORIES 14 /* The number of scores the user can themselves choose */
#define NUM_CATEGORIES 17 /* Same as above, but includes the total rows, and things like the bonus */

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



Yahtzee init_yahtzee(int numPlayers, int* isAI) {
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


// Runs some basic testing stuff. Pass args to it to test particular things?
int main(int argc, char *argv[]) {
    const char* arg = argv[1];

    if (arg == NULL) {
        printf("no args provided \n");
        exit(1);
    }

    int caseNum = atoi(arg);
    switch (caseNum) {
        case 1:
            break;
        default:
            break;
    }

}