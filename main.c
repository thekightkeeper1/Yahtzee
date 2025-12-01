#include "include/game.h"
#include <stdio.h>
#include  "include/ai.h"
#include <assert.h>
#include <time.h>
#include <limits.h>

#include "include/basic_tui.h"


int main(void) {
	printf("Welcome to automated yahtzee. ");
	printf("Choose how many AI players you want:\n");

	int numPlayers;
    while (true) {

	scanf("%d", &numPlayers);
        if (numPlayers < 0 || numPlayers > sizeof(uint) * 8) {
            printf("That number was either too big or negative, try another number.\n");
        } else {
            break;
        }
    }

	// Init the game and then start it.
    srand(time(NULL));
    const int aiPlayers = UINT_MAX;  // No matter how many players all the bits are filled so they are all ai
    Yahtzee game = init_yahtzee(numPlayers, aiPlayers);
    Yahtzee* y = &game;
    advance_player(y); // Advances game from impossible state to the first player

    // This is the game loop that should always be followed for well-defined behavior
    while (!is_over(game)) {
        printf("--- Round %d  Player %d/%d --- \n", y->round+1, y->curPlayer+1, y->numPlayers);

        // This is the dice rolling loop
        CATEGORIES chosen;
        for (int i = 0; i < MAX_ROLLS; i++) {
            roll_dice(y);  // Goes first always
            printf("Rolled: ");
            print_dice(*y);
            u_int8_t locked;
            if (ai_is_turn(*y) ){
                locked = ai_choose_locked(*y);
            } else {
                // This test case shouldn't have detected playable characters;
               assert(false);
            }
            if (locked == CHOOSE_SCORE) {
                printf("Choosing: ");
                break;
            }
            if (locked & CHOOSE_SCORE) {
                assert(false);
            }
            y->locked_dice ^= locked;
            printf("Locked: ");
            print_dice(*y);
        }
        // Now we get to choosing the actual score
        if (ai_is_turn(*y)) {
            chosen = ai_choose_score(*y);
        } else {
            // This test case should not have playble characters
            assert(false);
        }
        printf("%s for %d points.\n", CATEGORY_LABELS[chosen], y->bufferScore[chosen]);
        printf(" \n");

        update_score(y, chosen);
        // print_dice(game);  // Replace with anything to display what the user did on their turn.
        advance_player(y);
        printf("\n");
    }
    for (int i = 0; i < y->numPlayers; ++i) {
        print_scoreboard(game, i);
        printf("\n");
    }
}

