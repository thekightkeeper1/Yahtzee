#include "game.h"
#include "tui.h"

#include <stdlib.h>

Table alloc_score_table(wstr_t *hands, int numHands, wstr_t* playerNames, int numPlayers) {

    const int numCols = numPlayers + 1; // To account for the score label column

    int *colWidths = malloc(sizeof(int) * numCols); 
    int *cumulativeWidths = malloc(sizeof(int) * (numCols + 1));  // Cummulative widths are shifted. Always the first cell=0, and we need one extra to get the last cell=sum(widths)
    wstr_t *cellBottoms = malloc(sizeof(wstr_t) * numCols);

    
    // Getting the width of each player's name
    colWidths[0] = maxlen(hands, numHands) + 2;  // Largest of the column, plus extra spacing between border and cell text
    char c;

    for (int i = 0; i < numPlayers; i++) {
        colWidths[i+1] = wcslen(playerNames[i]) + 2; // Extra spacing between border and cell text
    }

    // Getting cumulative widths
    cumulativeWidths[0] = 0;
    for (int i = 1; i < numPlayers + 2; i++) {
        // Sum of previous cumulative width, plus the column we just passed, plus 1 for the right border
        cumulativeWidths[i] = cumulativeWidths[i-1] + colWidths[i-1] + 1;
    }

    // Getting the strs cached for making the horizontal border between cells
    for (int i = 0; i < numCols; i++) {
        cellBottoms[i] = malloc(sizeof(wchar_t) * (colWidths[i] + 1)); // Originally messed up parenthesis
        repeat(cellBottoms[i], L'─', colWidths[i]);
    }

    free(colWidths);
    Table t = {
        .x = 0,
        .y = 0,
        .numCols = numCols,
        .numRows = numHands,
        .cumulativeWidths = cumulativeWidths,
        .cellBottoms = cellBottoms,
    };
    return t;
}

void free_score_table(Table t) {
    
    // Freeing the deepest stuff first, the arrays of strs
    for (int i = 0; i < t.numCols; i++) {
        free(t.cellBottoms[i]);
    }

    // Freeing all the toplevel properties
    free(t.cellBottoms);
    free(t.cumulativeWidths);
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

Game setup_game(wstr_t* playerNames, const int numPlayers) {
    
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
    game.scoreboard = alloc_score_table(game.handCategories, game.numHands, game.playerNames, game.numPlayers);
    
    
    return game;
}