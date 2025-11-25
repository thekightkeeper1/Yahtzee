/*
* Interaction between core game logic and the TUI
*/

#include <stdlib.h>

#include "game_tui.h"
#include "tui.h"

// The labels themselves can be changed for styling. The number and position cannot
wstr_t CATEGORY_LABELS[NUM_CATEGORIES] = {
	L"Ones",
	L"Twos",
	L"Threes",
	L"Fours",
	L"Fives",
	L"Sixes",
	L"Three of a Kind",
	L"Four of a Kind",
	L"Small Straight",
	L"Large Straight",
	L"Full House",
	L"Yahtzee",
	L"Upper Total",
	L"Lower Total",
	L"Total"
};

// alloc
Table alloc_score_table(const wstr_t *playerNames, const int numPlayers) {
	const int numCols = numPlayers + 1; // To account for the score label column
	const int colHeight = NUM_CATEGORIES + 1; // To account for the player label row

	// We need to make a buffer of wstr_t to represent all the data in each cell
	// The first column is the labels
	// Then it is the player's scores
	wstr_t **columns = (wstr_t **) malloc(sizeof(wstr_t *) * numCols); // Allocate columns
	if (columns == NULL) {
		malloc_failed();
		exit(-1);
	};

	for (int c = 0; c < numCols; c++) {
		// Alloc'ing for the line of pointers that make up a column
		columns[c] = (wstr_t *) malloc(sizeof(wstr_t) * colHeight);
		if (columns[c] == NULL) {
			malloc_failed();
			exit(-1);
		}

		// Iterating thru the column
		for (int r = 0; r < colHeight; r++) {
			// Handling the label column/row
			if (c == 0) {
				// hands column
				if (r == 0) {
					// Corner case
					columns[c][r] = L"";
					continue;
				} else {
					columns[c][r] = CATEGORY_LABELS[r - 1]; // The table has a dummy value at 0,0 => it is 1 indexed
					continue;
				}
			}
			if (r == 0) {
				// Handling player name row
				columns[c][r] = playerNames[c - 1]; // The table has a dummy value at 0,0 => its 1 indexed
				continue;
			}

			// Default case where it isnt a label, and the score is 0
			// Allocating room for the actual string
			columns[c][r] = (wstr_t) malloc(sizeof(wchar_t) * 4); // Max yahzee score only have 4 digits + \0

			wcscpy(columns[c][r], L"-");
		}
	}

	return make_autosized_table(columns, numCols, colHeight);
}

// Pretty much the inverse of alloc_score table. uses the same iteration logic, just frees instead of allocing
void free_score_table(const Table t) {
	free_autosized_table(t);
	for (int c = 0; c < t.numCols; c++) {
		for (int r = 0; r < t.numRows; r++) {
			if (r != 0 && c != 0) {
				free(t.columnData[c][r]);
			}
		}
		free(t.columnData[c]);
	}
	free(t.columnData);
}

Game setup_game_ui(wstr_t *playerNames, const int numPlayers) {
	Game game_tui;

	game_tui.playerNames = playerNames;
	game_tui.numPlayers = numPlayers;
	game_tui.scoreboardTUI = alloc_score_table(game_tui.playerNames, game_tui.numPlayers);

	return game_tui;
}

void set_score_and_update(const int c, const int r, const int value, const Table t) {
	swprintf(t.columnData[c][r], 5, L"%d", value);
	draw_cell_data(t, c, r, FILLED_SCORE);
}
