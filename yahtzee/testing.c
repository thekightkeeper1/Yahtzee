#include  "game.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "testing.h"
#include "game_tui.h"


// Function for testing
void test_dice(Yahtzee* y, u_int8_t toLock, bool resetRolls) {
    y->currentRoll = resetRolls ? 0 : y->currentRoll;
    y->locked_dice = toLock;
    rollDice(y);
    print_dice(*y);
}
void test_scoring() {
    srand(time(NULL));

}

void test_score_buffer() {
    srand(time(NULL));

    // Testing the locking of each individual die
    printf("Testing each die\n");
    Yahtzee y = init_yahtzee(1, 0b0);
    test_dice(&y, 0b0, true);
    test_dice(&y, DIE_1, true);
    test_dice(&y, DIE_2, true);
    test_dice(&y, DIE_3, true);
    test_dice(&y, DIE_4, true);
    test_dice(&y, DIE_5, true);
    print_buffer_score(y);

    // Testing locking all
    printf("\nTesting locking all\n");
    test_dice(&y, 0b11111, true);
    test_dice(&y, 0b11111, true);

    // Testing more than three
    printf("\nTesting that it lets me do only 3\n");
    test_dice(&y, 0b0, true);
    test_dice(&y, 0b0, false);
    test_dice(&y, 0b0, false);
    test_dice(&y, 0b0, false);
}


// Runs some basic testing stuff. Pass args to it to test particular things?
int main(int argc, char *argv[]) {
    const char* arg = argv[1];

    if (arg == NULL) {
        printf("no args provided \n");
        exit(1);
    }

    const int caseNum = atoi(arg);
    switch (caseNum) {
        case 1:
            break;
        case 2:
            test_score_buffer();
            break;
        default:
            break;
    }

}

void print_dice(Yahtzee y) {
    for (int i = 0; i < NUM_DICE; i++) {
        const char locked = (y.locked_dice >> i) & 0b1 ? 'L' : ' ';
        printf("%d%c ", y.dice[i], locked);
    }
    printf("\n");
}

void print_buffer_score(Yahtzee y) {
    for (int i = 0; i < NUM_INTERACTIVE_CATEGORIES; i++) {
        printf("%d: %d\n", i, y.bufferScore[i]);
    }
}

void print_scoreboard() {
}