#include  "game.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "testing.h"
#include <wchar.h>

#include "../game_tui.h"

wstr_t CATEGORY_LABELS[NUM_CATEGORIES] = {
    L"Ones",
    L"Twos",
    L"Threes",
    L"Fours",
    L"Fives",
    L"Sixes",
    L"Three of a Kind",
    L"Four of a Kind",
    L"Full House",
    L"Small Straight",
    L"Large Straight",
    L"Yahtzee",
    L"Upper Total",
    L"Lower Total",
    L"Total"
};

// Function for testing
void test_dice(Yahtzee* y, u_int8_t toLock, bool resetRolls) {
    y->currentRoll = resetRolls ? 0 : y->currentRoll;
    y->locked_dice = toLock;
    rollDice(y);
    print_dice(*y);
}

void set_dice(Yahtzee *y, int d1, int d2, int d3, int d4, int d5) {
  y->dice[0] = d1;
  y->dice[1] = d2;
  y->dice[2] = d3;
  y->dice[3] = d4;
  y->dice[4] = d5;
}

void test_scoring_driver() {
    srand(time(NULL));
    Yahtzee game = init_yahtzee(1, 0b0);
    Yahtzee* y = &game;
    set_dice(y, 1, 1, 1, 1, 1);
    update_ephemeral(y);
    print_buffer_score(game);

}

void test_dice_driver() {
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
            test_dice_driver();
            break;
        case 2:
            test_scoring_driver();
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
    printf("| ");
    for (int i = 0; i < SIXES+1; i++) {
        printf("%d's: %d | ", i+1, y.bufferScore[i]);
    }
    printf("\n");
    for (int i = SIXES + 1; i < NUM_INTERACTIVE_CATEGORIES; i++) {

        printf("%-20ls: %d\n", CATEGORY_LABELS[i], y.bufferScore[i]);
    }
}

void print_scoreboard() {
}