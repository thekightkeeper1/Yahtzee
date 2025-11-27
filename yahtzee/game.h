#ifndef GAME_H
#define GAME_H
#include <stdbool.h>
#include <stdlib.h>

#define NUM_INTERACTIVE_CATEGORIES 12 /* The number of scores the user can themselves choose */
#define NUM_CATEGORIES 15 /* Same as above, but includes the total rows, and things like the bonus */
#define NUM_ROUNDS NUM_INTERACTIVE_CATEGORIES

#define MAX_ROLLS 3
#define NUM_DICE 5
#define NUM_DIE_FACES 6

#define LEN_LG_STRAIGHT 4
#define LEN_SM_STRAIGHT 3

#define POINTS_FULL_HOUSE 25
#define POINTS_SM_STRAIGHT 30
#define POINTS_LG_STRAIGHT 40
#define POINTS_YAHTZEE 50

#define NOT_CHOSEN (-1)
#define CANT_CHOOSE (-1)
#define CHOOSE_SCORE (0b11100000) // u_int8_t

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
	YAHTZEE,

	// This begins the none-interactive scores. Instead calculated by the scoring program
	UPPER_TOTAL,
	LOWER_TOTAL,
	COMPLETE_TOTAL,
} CATEGORIES;

#define DIE_1 0b00001
#define DIE_2 0b00010
#define DIE_3 0b00100
#define DIE_4 0b01000
#define DIE_5 0b10000

typedef int dice_t[NUM_DICE];

typedef struct {
	/* State for the entire game */
	int numPlayers;
	int **scores; // First index is the player, second is the particular score
	unsigned int isAI;

	/* Ephemeral state for each turn */
	int *bufferScore; // The theoretical score you can get for each category between rolls
	int curPlayer; // The index of the turn player
	int dice[NUM_DICE];
	int currentRoll; // Shows how many times the dice have been rolled this turn
	int round;
	u_int8_t locked_dice;
} Yahtzee;

typedef struct {
	int occurrences[NUM_DIE_FACES]; // Alternatively, gives the length of each nested array in the matrix below
	int positions[NUM_DIE_FACES][NUM_DICE]; // A reverse mapping, of occurences
} DiceInfo;

/* Functions */
Yahtzee init_yahtzee(int numPlayers, u_int64_t isAI);

void end_yahtzee(Yahtzee y);

void roll_dice(Yahtzee *y);

void update_ephemeral(Yahtzee *y);

bool update_score(const Yahtzee *y, int category);

void advance_player(Yahtzee *y);

bool is_over(Yahtzee y);

void toggle_dice(Yahtzee *y, u_int8_t toToggle);

DiceInfo get_occurrence_info(const dice_t dice);

void get_reachable_hands(Yahtzee y, CATEGORIES reachable[NUM_INTERACTIVE_CATEGORIES], int *n);

u_int8_t toggle_with_face(Yahtzee y, DiceInfo dInfo, int face);

CATEGORIES chose_max_of_buffer(Yahtzee y);

void ai_take_turn(Yahtzee *y);

u_int8_t ai_choose_locked(Yahtzee y, CATEGORIES *chosen);

bool ai_is_turn(Yahtzee y);


#endif // GAME_H
