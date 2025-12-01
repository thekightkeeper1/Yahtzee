#include "testing.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <wchar.h>
#include "../include/ai.h"
#include "../include/game.h"


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

int lock_sixes(const int dice[NUM_DICE]) {
    u_int8_t locked = 0;
    for (int i = 0; i < NUM_DICE; ++i) {
        if (dice[i] == 6)
            locked |= 0b1 << i;
    }
    if (!locked) return CHOOSE_SCORE;
    return locked;
}

void test_game_driver() {

    // Init the game and then start it.
    // Starting it just advances current round and current player fromm -1 --> 0; You could do this manually too
    srand(time(NULL));
    Yahtzee game = init_yahtzee(2, 0b0);
    Yahtzee* y = &game;
    advance_player(y);

    // This is the game loop that should always be followed for well-defined behavior
    while (!is_over(game)) {
        printf("--- Round %d  Player %d/%d --- \n", y->round+1, y->curPlayer+1, y->numPlayers);

        // This is the dice rolling loop that should always be used for well-defined behavior
        y->currentRoll = 0;
        for (int i = 0; i < MAX_ROLLS; i++) {
            printf("rolling %d\n", i);
            roll_dice(y);  // Goes first

            const u_int8_t locked = lock_sixes(game.dice); // Replace function with user interaction
            if (locked == CHOOSE_SCORE) {  // Required check before using toggle_dice
                break;
            }
            if (locked & CHOOSE_SCORE) {
                // The above checks if they had the same exact bits.
                // This checks if any of the bits on the locked dice are out of range
                // This doesn't really need to be checked here, it's just a usage note
                printf("Developer accidentally used the wrong bits, exiting.");
                exit(1);
            }
            toggle_dice(y, locked);
        }
        const int chosen_category = y->round;  // Replace with user interaction
        update_score(y, chosen_category);
        print_dice(game);  // Replace with anything to display what the user did on their turn.
        advance_player(y);
    }
        print_scoreboard(game);
}

int game_total() {

    Yahtzee game = init_yahtzee(1, 0b11);
    Yahtzee* y = &game;
    advance_player(y);

    while (!is_over(game)) {
        CATEGORIES chosen;
        for (int i = 0; i < MAX_ROLLS; i++) {
            roll_dice(y);  // Goes first always
            u_int8_t locked = 0b11111111;
            if (ai_is_turn(*y) ){
                locked = ai_choose_locked(*y);
            };
            if (locked == CHOOSE_SCORE) {
                break;
            }
            toggle_dice(y, locked);
        }
        if (ai_is_turn(*y)) {
            chosen = ai_choose_score(*y);
        } else {
            assert(false);
        }

        update_score(y, chosen);
        advance_player(y);
    }
    int total = y->scores[0][COMPLETE_TOTAL];
    end_yahtzee(*y);
    return total;
}

void test_ai() {
    // Init the game and then start it.
    // Starting it just advances current round and current player fromm -1 --> 0; You could do this manually too
    srand(time(NULL));
    Yahtzee game = init_yahtzee(1, 0b11);
    Yahtzee* y = &game;
    advance_player(y);

    // This is the game loop that should always be followed for well-defined behavior
    while (!is_over(game)) {
        printf("--- Round %d  Player %d/%d --- \n", y->round+1, y->curPlayer+1, y->numPlayers);

        // This is the dice rolling loop that should always be used for well-defined behavior
        CATEGORIES chosen;
        printf("Rolling:\n");
        for (int i = 0; i < MAX_ROLLS; i++) {
            roll_dice(y);  // Goes first always
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
                // The above checks if they had the same exact bits.
                // This checks if any of the bits on the locked dice are out of range
                // This doesn't really need to be checked here, its just a usage note
                printf("Developer accidentally used the wrong bits, press anything to exit.");
                getchar();
                exit(1);
            }
            toggle_dice(y, locked);
            print_dice(*y);
        }
        // Now we get to choosing the actual score
        if (ai_is_turn(*y)) {
            chosen = ai_choose_score(*y);
        } else {
            // This test case should not have playble characters
            assert(false);
        }
        printf("%ls for %d points.\n", CATEGORY_LABELS[chosen], y->bufferScore[chosen]);
        printf(" \n");

        update_score(y, chosen);
        // print_dice(game);  // Replace with anything to display what the user did on their turn.
        advance_player(y);
        printf("\n");
    }
    print_scoreboard(game);
}

void test_average() {
    srand(time(NULL));
    double total = 0;
    const int numTimes = 1e4;
    for (int i = 0; i < numTimes; ++i) {
        total += game_total();
    }
    printf("\ntotal of 100 games: %.2lf\n", total / (double) numTimes);
}

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

    int numRounds = 0;
    for (int round = 0; round < NUM_ROUNDS; round++) {
        for (int plr = 0; plr < y->numPlayers; plr++) {
            advance_player(y);
            roll_dice(y);
            roll_dice(y);
            roll_dice(y);
            update_score(y, round);
            if (is_over(game)) {
                print_cross();
                printf("The api thought the game was over when it isnt...\n");
            }
            if (round == y->round && plr == y->curPlayer) {
                print_check();
            } else {
                print_cross();
            }
            printf("stored=expected | round: %-2d=%2d player %d=%d\n", y->round, round, y->curPlayer, plr);

        }
        numRounds++;
    }

    printf("We had %d rounds completed\n", numRounds);
    printf("--- Testing game over functionality ---\n");
    advance_player(y);
    if (is_over(game)) {
        print_check();
        printf(" Successfuly detected end of the game\n");
    } else {
        print_cross();
        printf("The api didnt't return true that the game is over\n");

    }

    printf("The next round advancment should cause an assertion to fail. If it doesnt, game api doesnt catch an error\n");
    advance_player(y);

}

void test_score_updating() {
    Yahtzee game = init_yahtzee(1, 0b0);
    Yahtzee* y = &game;
    y->curPlayer=0;
    y->round=0;

    for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
        int expected_total = (int) ((cat+1) * (cat+2)) / 2;
        y->bufferScore[cat] = cat+1;
        assert(update_score(y, cat));
        if (expected_total == y->scores[0][COMPLETE_TOTAL] && y->scores[0][cat] == cat+1) {
            print_check()
;            printf("Updated %-15ls <- %d; matches expected %d | Totals match too. \n", CATEGORY_LABELS[cat], y->scores[0][cat], cat+1);
        } else {
            print_cross();
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
    y->locked_dice = toLock;
    y->currentRoll = resetRolls ? 0 : y->currentRoll;
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

void print_etc(Yahtzee *y, int dice, const char* msg, int scores, int cur_roll) {
    printf("\n\n%s\n", msg);
    y->currentRoll = cur_roll;
    int d1 = dice / 1 % 10;
    int d2 = dice / 10 % 10;
    int d3 = dice / 100 % 10;
    int d4 = dice / 1000 % 10;
    int d5 = dice / 10000 % 10;

    for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
        y->scores[y->curPlayer][cat] = (scores >> cat )& 1;
        if (y->scores[y->curPlayer][cat] == 0)
            y->scores[y->curPlayer][cat] = NOT_CHOSEN;
    }

    set_dice(y, d1, d2, d3, d4, d5);
    print_dice(*y);
    update_ephemeral(y);
    print_buffer_score(*y);
    u_int8_t toLock =  ai_choose_locked(*y) ;


    if (toLock == CHOOSE_SCORE) {
        CATEGORIES categories = ai_choose_score(*y);
        printf("AI choose: locked: N/A | category: %ls\n", CATEGORY_LABELS[categories]);
    } else {
        toLock ^=y->locked_dice;
        int to_toggle = 0;
        for (int i = 0; i < NUM_DICE; i++) {
            to_toggle += (i+1) * (toLock >> i & 1);
            to_toggle *= 10;
        }
        to_toggle /= 10;
        printf("AI choose: locked: %d | category: N/A\n", to_toggle);
    }

}

void test_update_ephemeral_driver() {
    int score;
    printf("\n\n\n ---- Testing scoring ----\n");
    srand(time(NULL));
    Yahtzee game = init_yahtzee(1, 0b0);
    game.round = 0;
    game.curPlayer = 0;
    Yahtzee* y = &game;

    print_etc(y, 66333, "Full House with rolls",  0, 2);
    print_etc(y, 11111, "Yahtzee", 0b0, 1);
    print_etc(y, 11111, "Yahtzee detects already filled score", 1<<YAHTZEE, 1);

    print_etc(y, 11112, "Upper with re-rolls", 0, 1);
    print_etc(y, 11112, "Upper without availble",  0b111111, 1);
    print_etc(y, 11112, "Upper without re-rolls",  0, 3);
    print_etc(y, 11112, "Upper without availble or re-rolls",  0b111111, 3);

    print_etc(y, 66662, "high upper with re-rolls", 0, 1);
    print_etc(y, 66662, "high upper without upper availble",  0b111111, 3);


    print_etc(y, 23454, "SM STRAIGHT no rerolls", 0, 3);
    print_etc(y, 23454, "SM STRAIGHT with re-rolls", 0, 1);
    print_etc(y, 12341, "SM STRAIGHT on the edge", 0, 1);
    print_etc(y, 12345, "LG straight", 0, 1);

}

void test_dice_driver() {
    srand(time(NULL));

    // Testing the locking of each individual die
    printf("Testing each die\n");
    Yahtzee y = init_yahtzee(1, 0b0);
    y.curPlayer = 0;
    y.round = 0;
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
    y.currentRoll = 0;
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
            test_update_ephemeral_driver();
            break;
        case 3:
            test_score_updating();
            break;

        case 4:
            test_round_advancement();
            break;
        case 5:
            test_game_driver();
            break;
        case 6:
            printf("Boutta test it mate");
            test_ai();
            break;
        case 7:
            printf("Testing average of 100 games of ai");
            test_average();
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