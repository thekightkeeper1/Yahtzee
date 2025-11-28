#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

// Toggles the dice at the specifi
void toggle_dice(Yahtzee *y, const u_int8_t toToggle) {
	// First, check that the wrong bits, the 3 highest, haven't been accidentally used
	assert(! (toToggle & 0b11100000));
	y->locked_dice ^= toToggle;
}

// Advances to. the next player, wrapping around and incrementing round number too
// This does NOT, however, do anything other than offer an except when we are already over the number of rounds
void advance_player(Yahtzee *y) {
	// It should not be possible to advance the round, so disallow it
	assert(!is_over(*y));

	y->curPlayer = (y->curPlayer + 1) % y->numPlayers;

	if (y->curPlayer == 0) {
		y->round++;
	}
	y->locked_dice = 0;
	y->currentRoll = 0;
}

bool ai_is_turn(const Yahtzee y) {
	if (y.isAI & ( 1 << y.curPlayer)) {
		return true;
	}
	return false;
}

// Returns true if all the scores have been filled and no more players should take their turn
// YOU MUST ADVANCE PLAYER BEFORE CALLING THIS, IT DOESNT DETECT ANYTHING BEFORE ADVANCING PLAYER
// This is so that you can do while (!is_over(y)) and it will only fail when it is on an invalid player turn
bool is_over(Yahtzee y) {
	return y.round >= NUM_ROUNDS;
}

// Sets the score given the buffer array of scores, and an index
// Returns false if they chose a category that was already filled
bool update_score(const Yahtzee *y, const int category) {
	const int plr = y->curPlayer;
	// Data sanitization
	assert(category < NUM_INTERACTIVE_CATEGORIES); // Is a valid category

	// It is resonable to assume that a dev might not want to ensure they chose a valid category
	// This is thus not an assertion, but just a return value
	if (y->scores[plr][category] != NOT_CHOSEN) return false;

	// Now we update the score
	y->scores[plr][category] = y->bufferScore[category];

	// Now we either update the lower, or the upper total
	if (category <= SIXES) {
		y->scores[plr][UPPER_TOTAL] += y->bufferScore[category];
	} else {
		y->scores[plr][LOWER_TOTAL] += y->bufferScore[category];
	}
	y->scores[plr][COMPLETE_TOTAL] += y->bufferScore[category];

	return true;
}

Yahtzee init_yahtzee(const int numPlayers, u_int64_t isAI) {
	const Yahtzee y = {

		.numPlayers = numPlayers,
		.scores = malloc(numPlayers * sizeof(int *)),

		// Ephemeral state
		.bufferScore = calloc(NUM_INTERACTIVE_CATEGORIES, sizeof(int)),
		.curPlayer = -1, //  must call next_round after init game
		.round = -1, //  must called next_round after init game
		.dice = {0},
		.currentRoll = 0,
		.isAI = isAI,
	};


	// Allocating the score arrays, and initiilizing them to NOT_CHOSEN for the interactive categories
	// For the total categories, let them be set to 0, since that is the total of points
	for (int plr = 0; plr < numPlayers; plr++) {
		y.scores[plr] = malloc(NUM_CATEGORIES * sizeof(int));
		for (int cat = 0; cat < NUM_CATEGORIES; cat++) {
			y.scores[plr][cat] = cat < NUM_INTERACTIVE_CATEGORIES ? NOT_CHOSEN : 0;
		}
	}
	return y;
}

// Frees ANY memory allocated by this library
void end_yahtzee(const Yahtzee y) {
	for (int n = 0; n < y.numPlayers; n++) {
		free(y.scores[n]);
	}
	free(y.scores);
	free(y.bufferScore);
}

// Rolls the dice in the yahtzee game.
void roll_dice(Yahtzee *y) {
	// Handles all dice rolling logic, including resetting locked dice if we are on 0 rolls
	// And advancing the number of rolls

	// Check we can roll, then increment
	assert(y->currentRoll < MAX_ROLLS); // Game driver should stop this from failing
	y->currentRoll++;
	// if (y->currentRoll == 0) y->locked_dice = 0;

	// Iterating bits of locked dice, and only rolling the correct ones
	for (int i = 0; i < 5; i++) {
		if (~(y->locked_dice >> i) & 0b1) {
			// If the bit at the ith position from the right is toggled
			y->dice[i] = 1 + rand() % 6;
		}
	}
	update_ephemeral(y);
}

bool has_FULL_HOUSE(const int occurrences[NUM_DIE_FACES]) {
	// A full house should have exactly 3 of one dice, and 2 of another
	int three = 0;
	int two = 0;
	for (int face = 0; face < NUM_DIE_FACES; face++) {
		if (occurrences[face] == 2) {
			//
			two++;
		} else if (occurrences[face] == 3) {
			three++;
		}
	}

	if (two == 1 && three == 1) {
		return true;
	}
	return false;
}

bool is_continuous(const int occurrences[NUM_DIE_FACES], int start, int length) {
	assert(start < NUM_DIE_FACES);
	assert(start + length <= NUM_DIE_FACES);

	for (int face = 0; face < start + length; face++) {
		if (occurrences[face] == 0) return false;
	}
	return true;
}

// Returns whether we have a straight of at least 4
bool has_LG_STRAIGHT(int occurrences[NUM_DIE_FACES]) {
	for (int curStart = 0; curStart <= NUM_DIE_FACES - LEN_LG_STRAIGHT; curStart++) {
		if (is_continuous(occurrences, curStart, LEN_LG_STRAIGHT)) return true;
	}
	return false;
}

// Returns whether we have a straight of at least 4
bool has_SM_STRAIGHT(int occurrences[NUM_DIE_FACES]) {
	for (int curStart = 0; curStart <= NUM_DIE_FACES - LEN_SM_STRAIGHT; curStart++) {
		if (is_continuous(occurrences, curStart, LEN_SM_STRAIGHT)) return true;
	}
	return false;
}

// Returns number of dice repeated, and sets the total to the sum of all faces
int of_a_kind_faces(const int occurrences[NUM_DIE_FACES], int *sum) {
	*sum = 0;
	int result = 0;
	for (int face = 0; face < NUM_DIE_FACES; face++) {
		if (occurrences[face] == 3) result = 3;
		if (occurrences[face] >= 4) result = 4;
		*sum += (face+1) * occurrences[face];
	}
	return result;
}

bool has_YAHTZEE(const int occurrences[NUM_DIE_FACES]) {
	for (int face = 0; face <= NUM_DIE_FACES; face++) {
		if (occurrences[face] == 5) return true;
	}
	return false;
}

// Updates the ephemeral scoreboard during rolls
void update_ephemeral(Yahtzee* y) {
    assert(y->curPlayer < y->numPlayers);

    // First we initilze every category to disabled. Can only be enabled if we haven't scored it already
    const int* score = y->scores[y->curPlayer];
    for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
        y->bufferScore[cat] = CANT_CHOOSE;
    }

    // tally up all the face values
    int diceOccurrences[NUM_DIE_FACES] = {0};
    for (int curDie = 0; curDie< NUM_DICE; curDie++) {
        const int dieValue = y->dice[curDie];
        diceOccurrences[dieValue-1]++;
    }

    // calculating the upper scores
    for (int cat = ONES; cat < SIXES+1; cat++) {
        if (score[cat] == NOT_CHOSEN)
            y->bufferScore[cat] = diceOccurrences[cat] * (1 + cat); // Dice faces are 1 index, but cat is 0 index
    }

    // Computing the lower score section
    // Of a kind scores
    int total;
    const int maxRepeated = of_a_kind_faces(diceOccurrences, &total);
    if (score[FOUR_OF] == NOT_CHOSEN && maxRepeated == 4)
        y->bufferScore[FOUR_OF] = total;
    if (score[THREE_OF] == NOT_CHOSEN && maxRepeated > 3)
        y->bufferScore[THREE_OF] = total;

    // Full house
    if (score[FULL_HOUSE] == NOT_CHOSEN)
        y->bufferScore[FULL_HOUSE] = has_FULL_HOUSE(diceOccurrences) || has_YAHTZEE(diceOccurrences) ? POINTS_FULL_HOUSE : 0;

    // Straights/yahtzee
    if (score[LG_STRAIGHT] == NOT_CHOSEN)
        y->bufferScore[LG_STRAIGHT] = has_LG_STRAIGHT(diceOccurrences) ? POINTS_LG_STRAIGHT : 0;
    if (score[SM_STRAIGHT] == NOT_CHOSEN)
        y->bufferScore[SM_STRAIGHT] = has_SM_STRAIGHT(diceOccurrences) ? POINTS_SM_STRAIGHT : 0;
    if (score[YAHTZEE] == NOT_CHOSEN)
        y->bufferScore[YAHTZEE] = has_YAHTZEE(diceOccurrences) ? POINTS_YAHTZEE : 0;

}


void bubble_sort(int *array, int length) {
	for (int pass = 0; pass < length - 1; pass++)
		for (int index = 0; index < length - pass - 1; index++)

			// If this is greater than the next
			if (array[index] > array[index + 1]) {
				// Swapping
				int temp = array[index];
				array[index] = array[index + 1];
				array[index + 1] = temp;
			}
}

// Returns dice indicies if there is a straight starting not from an index, but a dice facevalue
u_int8_t best_straight(const dice_t dice, DiceInfo dInfo) {

	// First we sort dice, so we can use the property that each die should be sequential
	int sorted[NUM_DICE];
	for (int i = 0; i < NUM_DICE; i++) {
		sorted[i] = dice[i];
	}
	bubble_sort(sorted, NUM_DICE);

	// Next we just use each die face as starting position and check for a
	// We don't care about: straight <= 2  or if straight == 3 but it is on either edge, starting at one or ending at 6
	int maxLength = 1;
	int maxStart = 0;
	const int (*positions)[5] = dInfo.positions;
	u_int8_t toLock = 0;
	for (int start = 0; start < NUM_DICE; start++) {
		int prev = sorted[start]; // Previous face initilzer, used for the
		toLock = 1 << positions[prev][0];
		int len = 1;
		for (int i = start + 1; i < NUM_DICE; i++) {
			if (sorted[i] == prev) { // Skip repeated values
				continue;
			}
			if (prev == sorted[i] - 1) {
				toLock |= 1 << positions[prev][0];
				len++;
				prev = sorted[i];
			} else {
				break;
			}
		}

		if (len > maxLength) {
			maxStart = start;
			maxLength = len;
		}
	}
	if (maxLength < 3) return 0;
	if (maxLength == 3) {
		assert(dice[maxStart] < 5);
		assert(dice[maxStart] != 3);
		if (dice[maxStart] == 1 || dice[maxStart == NUM_DIE_FACES - 2]) return 0; // The straights dont have edges open
	}
	assert(toLock != 0);
	return toLock;
}

u_int8_t ai_choose_locked(const Yahtzee y, CATEGORIES *chosen) {
	// Caching info
	const DiceInfo dInfo = get_occurrence_info(y.dice);
	const int curPlayer = y.curPlayer;
	const int *score = y.scores[curPlayer];


	// Handling all the lower section items first, since they tend to yield lots of points from rare configurations
	if (y.bufferScore[YAHTZEE] > 0 ) {
		*chosen = YAHTZEE;
		return YAHTZEE;
	}

	if (y.bufferScore[LG_STRAIGHT] > 0) {
		*chosen = LG_STRAIGHT;
		return LG_STRAIGHT;
	}

	if (y.bufferScore[FULL_HOUSE] > 0) {
		*chosen = FULL_HOUSE;
		return FULL_HOUSE;
	}

	if (y.bufferScore[SM_STRAIGHT] > 0 && score[LG_STRAIGHT] > 0 && y.currentRoll) { // Dont choose yet if we have rerolls left
		*chosen = SM_STRAIGHT;
		return SM_STRAIGHT;
	}


	/* Now we are checking things that might lock in,
	 * since we can use rerolls for even better results
	 * without compromising our current best score
	 */

	// The first case is we can go for some sort of straight
	if (score[SM_STRAIGHT] == NOT_CHOSEN || score[LG_STRAIGHT] == NOT_CHOSEN ) {
		u_int8_t toLock;
		if ((toLock = best_straight(y.dice, dInfo)) != 0) {
			// WE have a decent chance at making a straight given that those dice are locked
			return toLock ^ y.locked_dice;
		}
		// Otherwise its no worth going for a straight

	}

	// Handling multiple dice in a row:
	// Its simple, you just lock them in, then choose the highest score we can
	for (int threshold = 4; threshold > 0; threshold--) {
		for (int face = NUM_DIE_FACES; face > 0 ; face--) {
			if (y.currentRoll == MAX_ROLLS) {
				*chosen = chose_max_of_buffer(y);
				return CHOOSE_SCORE;
			}

			// Now we just lock in the higest occuring die that we can
			if (dInfo.occurrences[face-1] == threshold) {
				u_int8_t to_toggle = toggle_with_face(y, dInfo, face-1);
				return to_toggle;
			}
		}
	}
	assert(false);
}

CATEGORIES chose_max_of_buffer(const Yahtzee y) {
	int max = ONES;
	for (int cat = 0; cat < NUM_INTERACTIVE_CATEGORIES; cat++) {
		if (y.bufferScore[cat] > y.bufferScore[max]) {
			max = cat;
		}
	}
	return max;
}

u_int8_t toggle_with_face(const Yahtzee y, const DiceInfo dInfo, const int face) {
	assert (face < NUM_DIE_FACES);

	u_int8_t locked = 0;
	const int *positions = dInfo.positions[face]; // Array of dices with the face of interest
	for (int die = 0; die < NUM_DICE; die++) {
		if ((y.locked_dice >> die) & 1) {
			// It is locked. Unlock it if we dont care about it
			if (y.dice[die] != face+1)
				locked |= 1<<die;
		} else {
			// It is not locked, lock it if we care about it
			if (y.dice[die] == face+1)
				locked |= 1<<die;
		}
	}

	return locked;
}

DiceInfo get_occurrence_info(const dice_t dice) {
	DiceInfo diceInfo = {{0}};

	for (int face = 0; face < NUM_DIE_FACES; face++)
		diceInfo.positions[face][0] = -1;

	for (int curDie = 0; curDie < NUM_DICE; curDie++) {
		const int dieFace = dice[curDie];
		diceInfo.occurrences[dieFace - 1]++;

		// Next, we just need to push the position of that occurence onto an array
		// The length of the array is equal to occurences, and that -1 will be the last index of that array
		// So we push the item right there
		const int numOccurences = diceInfo.occurrences[dieFace - 1];
		diceInfo.positions[dieFace - 1][numOccurences - 1] = curDie;
		diceInfo.positions[dieFace - 1][numOccurences] = -1;

	}

	return diceInfo;
}

