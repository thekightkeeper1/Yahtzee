#include  "game.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include "testing.h"

#include <assert.h>
#include <wchar.h>


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

void print_check() {
    printf("\033[1;32m");
    printf("✓ ");
    printf("\033[0m");
}

void print_cross() {
    printf("\033[1;31m");
    printf("✘ ");
    printf("\033[0m");
}

void test_round_advancement() {
    printf("\n\n\n--- Testing the round advancement --- \n");
    Yahtzee game = init_yahtzee(2, 0b0);
    Yahtzee* y = &game;
    for (int round = 0; round < NUM_ROUNDS; round++) {
        for (int plr = 0; plr < y->numPlayers; plr++) {
            if (round == y->round && plr == y->curPlayer) {
                print_check();
            } else {
                print_cross();
            }
            printf("stored=expected | round: %-2d=%2d player %d=%d\n", y->round, round, y->curPlayer, plr);
            roll_dice(y);
            roll_dice(y);
            roll_dice(y);
            update_score(y, round);
            advance_player(y);
        }
    }
    printf("The next round advancment should cause an assertion to fail. If it doesnt, game api doesnt catch an error\n");
    advance_player(y);
}

void test_score_updating() {
    Yahtzee game = init_yahtzee(1, 0b0);
    Yahtzee* y = &game;

    for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
        int expected_total = (int) ((cat+1) * (cat+2)) / 2;
        y->bufferScore[cat] = cat+1;
        assert(update_score(y, cat));
        if (expected_total == y->scores[0][COMPLETE_TOTAL] && y->scores[0][cat] == cat+1) {
            printf("\033[1;32m");
            printf("✓ ");
            printf("\033[0m");
            printf("Updated %-15ls <- %d; matches expected %d | Totals match too. \n", CATEGORY_LABELS[cat], y->scores[0][cat], cat+1);
        } else {
            printf("\033[1;31m");
            printf("✘ ");
            printf("\033[0m");
            printf("Updated %-15ls <- %d, but the expected total is %d \n", CATEGORY_LABELS[cat], y->scores[0][cat], cat+1);

        }

        // print_scoreboard(game);

    }

    for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
        y->bufferScore[cat] = cat+1;
        assert(!update_score(y, cat));
        printf("\033[1;32m");
        printf("✓ ");
        printf("\033[0m");
        printf("Detected %-15ls : %d to be locked in already\n", CATEGORY_LABELS[cat], y->scores[0][cat]);
    }
}

void test_dice(Yahtzee* y, u_int8_t toLock, bool resetRolls) {
    y->currentRoll = resetRolls ? 0 : y->currentRoll;
    y->locked_dice = toLock;
    roll_dice(y);
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
        case 3:
            test_score_updating();
            break;

        case 4:
            test_round_advancement();
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

void print_buffer_score(const Yahtzee y) {
    printf("--- Buffer ---\n");
    printf("| ");
    for (int i = 0; i < SIXES+1; i++) {
        printf("%d's: %d | ", i+1, y.bufferScore[i]);
    }
    printf("\n");
    for (int i = SIXES + 1; i < NUM_INTERACTIVE_CATEGORIES; i++) {

        printf("%-20ls: %d\n", CATEGORY_LABELS[i], y.bufferScore[i]);
    }
}

void print_scoreboard(const Yahtzee y) {

    printf("--- Scoreboard ---\n");
    for (int i = 0; i < NUM_CATEGORIES; i++) {

        printf("%-20ls: %d\n", CATEGORY_LABELS[i], y.scores[0][i]);
    }

}