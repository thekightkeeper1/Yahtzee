/*
* Interaction between core game logic and the TUI
*/
#include "game_tui.h"
#include "tui.h"
#include <stdlib.h>
#include <threads.h>

// alloc
Table alloc_score_table(wstr_t *hands, int numHands, wstr_t* playerNames, int numPlayers) {

    const int numCols = numPlayers + 1; // To account for the score label column
    const int colHeight = numHands + 1; // To account for the player label row

    // We need to make a buffer of wstr_t to represent all the data in each cell
    // The first column is the labels
    // Then it is the player's scores
    wstr_t **columns = (wstr_t**)malloc(sizeof(wstr_t*) * numCols); // Allocate columns
    if (columns == NULL) {
        malloc_failed();
        exit(-1);
    };

    for (int c = 0; c < numCols; c++) {
        // Alloc'ing for the line of pointers that make up a column
        columns[c] = (wstr_t*)malloc(sizeof(wstr_t) * colHeight);
        if (columns[c] == NULL) {
            malloc_failed();
            exit(-1);
        }
        
        // Iterating thru the column
        for (int r = 0; r < colHeight; r++) {

            // Handling the label column/row
            if (c == 0) {  // hands column
                if (r == 0) { // Corner case
                    columns[c][r] = L"";
                    continue;
                } else {
                    columns[c][r] = hands[r-1]; // The table has a dummy value at 0,0 => it is 1 indexed
                    continue;
                }
            }
            if (r == 0) { // Handling player name row
                columns[c][r] = playerNames[c-1]; // The table has a dummy value at 0,0 => its 1 indexed
                continue;
            }
            
            // Default case where it isnt a label, and the score is 0
            // Allocating room for the actual string
            columns[c][r] = (wstr_t)malloc(sizeof(wchar_t) * 4); // Max yahzee score only have 4 digits + \0

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

// Given an int array, and array of wstrs, it replaces the wstrs with the ints
wchar_t** int_array_to_wchar_array(const int nums[], wchar_t** strs, const int n, const int len) {
    // 
    for (int i = 0; i < n; i++)
    {
        wchar_t *s = strs[i];
        swprintf(s, len, L"%d", nums[n]);
    }
    
}

Game setup_game_ui(wstr_t* playerNames, const int numPlayers) {

    Game game;

    // This is the types of hands that you can win. 
    // Feel free to edit this, just make sure to add a function in todo that handles rules for that category
    wstr_t yahtzeeHands[] = { 
        L"Ones",
        L"Twos",
        L"Threes",
        L"Fours",
        L"Fives",
        L"Sixes",
        L"Bonus",
        L"Three of a Kind",
        L"Four of a Kind",
        L"Small Straight",
        L"Large Straight",
        L"Full House",
        L"Yahtzee",
        L"Chance",
    };  
    
    game.handCategories = yahtzeeHands;
    game.playerNames = playerNames;
    game.numPlayers = numPlayers;
    game.numHands = 14;
    game.scoreboardTUI = alloc_score_table(game.handCategories, game.numHands, game.playerNames, game.numPlayers);

    return game;
}

void set_score_and_update(const int c, const int r, const int value, const Table t) {
    swprintf(t.columnData[c][r], 5, L"%d", value);
    draw_cell_data(t, c, r, FILLED_SCORE);
}
