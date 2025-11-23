#define _XOPEN_SOURCE_EXTENDED /* Required for ncurses to actually use the unicode funcs*/
#include <ncursesw/curses.h>
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "main.h"
#include "utils.h"

#define NUM_CATEGORIES 14



// Gets the x y position of a table cell. It returns writeable area, not where the border is
void get_cell_pos(const int r, const int c, int* x, int* y, const Table t) {
    assert(r < t.numRows && c < t.numCols);

    *y = t.y + 1 + 2*r; // Origin, shift down one to ignore the top border, and then rows are 2 high, so go down 2 per each 
    *x = t.x + 1 + t.cumulativeWidths[c]; // Origin, shifted by cummulative effect
}

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

void draw_table(Table t) {
    
    // Nested iteration to draw every cell in the table
    for (int row = 0; row < t.numRows; row++) {
        
        // For this entire row we keep the same y for cells
        int cellY = t.y + 1 + 2*row;  // Shifted to account for top border of table, since cells dont draw their own top border
 
        for (int col = 0; col < t.numCols; col++) {
            int cellX = t.x + t.cumulativeWidths[col] ;

            // Top border
            if (row == 0) {
                mvaddwstr(t.y, cellX+1, t.cellBottoms[col]);
                addwstr(L"┬");
            }
            
            
            // Next we consider its left border
            mvaddwstr(cellY, cellX, L"│");
            move(cellY+1, cellX);
            if (col == 0) {
                addwstr(L"├");  // Non-bottom left
            } else { 
                if (row == t.numRows-1) {
                    addwstr(L"┴");  // Non-left bottom
                } else {
                    addwstr(L"┼");  // Non-left non-bottom
                }
            }

            // Bottom border is really easy
            addwstr(t.cellBottoms[col]);
        
            // Right border
            if (col == t.numCols-1) {
                addwstr(L"┤");  // no move b/c after drawing the bottom bar, we are exactly in place for this
                mvaddwstr(cellY, t.cumulativeWidths[col+1], L"│");
            } 

        }
        // Drawing the corners
        const int rightEdgeX  = t.x + t.cumulativeWidths[t.numCols];
        const int bottomEdgeY = t.y + 2 * t.numRows;
        mvaddwstr(bottomEdgeY, rightEdgeX, L"╯");
        mvaddwstr(t.y,         rightEdgeX, L"╮");
        mvaddwstr(bottomEdgeY, t.x,        L"╰");
        mvaddwstr(t.y,         t.x,        L"╭");

        
    }
    
}


// Repeats given wchar_t n times and returns the resultant string.
void repeat(wchar_t* dest, const wchar_t c, const int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = c;
    }
    dest[i] = L'\0'; 
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

int main(void) {
    setlocale(LC_ALL, "");

    SCREEN *scr = newterm(NULL, stdout, stdin);

    wstr_t names[] = {L"Belinda", L"Sophie", L"Jason"};
    Game game = setup_game(names, 3);
    draw_table(game.scoreboard);
    getch();
    free_score_table(game.scoreboard);
    endwin();
    delscreen(scr);
    exit(0);
    int x, y;

    // endwin();
}

