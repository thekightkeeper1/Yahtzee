//
// Created by ty on 11/30/25.
//

#include "../include/basic_tui.h"

#include <assert.h>
#include <stdio.h>
#include "../include/game.h"



char* CATEGORY_LABELS[NUM_CATEGORIES] = {
	"Ones",
	"Twos",
	"Threes",
	"Fours",
	"Fives",
	"Sixes",
	"Three of a Kind",
	"Four of a Kind",
	"Full House",
	"Small Straight",
	"Large Straight",
	"Yahtzee",
	"Upper Total",
	"Lower Total",
	"Total"
};

void print_scoreboard(const Yahtzee y, const int player) {
	assert(player < y.numPlayers && 0 <= player);

	printf("--- P%d Scoreboard ---\n", player);
	for (int i = 0; i < NUM_CATEGORIES; i++) {

		// ReSharper disable once CppPrintfBadFormat
		printf("%-20s: %d\n", CATEGORY_LABELS[i], y.scores[player][i]);
	}
}

void print_dice(const Yahtzee y) {
	for (int i = 0; i < NUM_DICE; i++) {
		const char locked = (y.locked_dice >> i) & 0b1 ? 'L' : ' ';
		printf("%d%c ", y.dice[i], locked);
	}
	printf("\n");
}
