//
// Created by ty on 11/30/25.
//

// #include <stdio.h>
#include "ai.h"
#include "game.h"
#include <stdlib.h>
#include  "assert.h"


CATEGORIES chose_max_of_buffer(const Yahtzee y) {
	int max = ONES;
	for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
		if (y.bufferScore[cat] > y.bufferScore[max]) {
			max = cat;
		}
	}
	return max;
}

u_int8_t ai_choose_locked(const Yahtzee y) {

	// In the following two cases we want to abort rolling dicce and instead skip straight to choosing
	if (y.currentRoll == MAX_ROLLS) return CHOOSE_SCORE;
	if (y.bufferScore[FULL_HOUSE] > 0) return CHOOSE_SCORE;

	// Caching info for choosing dice
	const DiceInfo dInfo = get_occurrence_info(y.dice);
	const int curPlayer = y.curPlayer;
	const int *score = y.scores[curPlayer];

	// The first case is we already rolled some dice in a row
	if (score[SM_STRAIGHT] == NOT_CHOSEN && y.bufferScore[LG_STRAIGHT] != CANT_CHOOSE ) {

		u_int8_t toLock;
		int _;
		if ((toLock = best_straight(y.dice, dInfo, &_)) != 0) {
			// We have at least 3 dice in a row, so we can try lockking them
			// printf("Going for a straight\n");
			return toLock ^ y.locked_dice;
		}
		// Otherwise its no worth going for a straight

	}

	// Next we pursue multiple dice of the same face
	for (int threshold = NUM_DICE; threshold > 0; threshold--) {
		for (int face = NUM_DIE_FACES; face > 0 ; face--) {

			// Now we just lock in the higest occurring die that we can
			if (dInfo.occurrences[face-1] == threshold) {
				u_int8_t to_toggle = toggle_with_face(y, dInfo, face-1);
				return to_toggle;
			}
		}
	}

	// Genuinely dont know how we got here, but we will just tell it to choose a score
	return CHOOSE_SCORE;
	assert(false);
}

CATEGORIES ai_choose_score(const Yahtzee y) {
	if (y.bufferScore[FULL_HOUSE] == POINTS_FULL_HOUSE) return FULL_HOUSE;
	DiceInfo dice_info = get_occurrence_info(y.dice);
	int maxFace;
	for (maxFace = 1; maxFace < NUM_DIE_FACES; maxFace++) {
		if (dice_info.occurrences[maxFace-1] >= 3) {
			if (y.bufferScore[maxFace-1] != CANT_CHOOSE) {
				switch (maxFace-1) {
					case ONES:
						return ONES;
					case TWOS:
						return TWOS;
					case THREES:
						return THREES;
					case FOURS:
						return FOURS;
					case FIVES:
						return FIVES;
					case SIXES:
						return SIXES;
					default: ;
				}
			}
		}
	}
	return chose_max_of_buffer(y);
}


bool ai_is_turn(const Yahtzee y) {
	if (y.isAI & ( 1 << y.curPlayer)) {
		return true;
	}
	return false;
}
