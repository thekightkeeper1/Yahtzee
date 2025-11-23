#define _XOPEN_SOURCE_EXTENDED /* Required for ncurses to actually use the unicode funcs*/
#include <ncursesw/curses.h>
#include <stdio.h>
#include <locale.h>
#include <wchar.h>  /* Req for c to actual use wchars properly, even if it doesnt throw erros without it*/

typedef struct {
    bool top;
    bool bottom;
    bool left;
    bool right;
} Borders;

typedef struct {
    wchar_t **handCategories;
    int numHandCategories;
} Game;

typedef struct {

} table;

typedef wchar_t* wstr_t;

void draw_table(wstr_t** data, bool isRow, Borders borders);

int maxlen(wchar_t **arr, int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        int len = wcslen(arr[i]);
        if (len > max) {
            max = len;
        }
    }
    return max;
}

// Repeats given wchar_t n times and returns the resultant string.
void repeat(wchar_t* dest, wchar_t c, const int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = c;
    }
    dest[i+1] = L"\0"; 
}



void print_score_table(wchar_t **playerNames, wchar_t **handCategories, int **playerScores, int numCols, wstr_t row_bar, int NUM_TABLE_ROWS, wstr_t* yahtzeeHands, int row_width_exclusive) {
    // The top left corner of the table is given below
    // We go by column, since information is grouped into types by column
    // We draw the border separately. The loops control drawing the left and bottops parts of the cell:
    // │    foo
    // ┼────────────

    // Arbitrary origin
    int table_x = 6;
    int table_y = 3;
    int cell_x, cell_y;



    // Draw the top border of the entire table
    mvaddwstr(table_y, table_x, L"╭");  // mv table beginning, draw TL corner
    for (int i = 0; i < numCols; i++) {
        addwstr(row_bar);
        if (i < numCols - 1) addwstr(L"┬");
        else addwstr(L"╮");
    }
    
    
    
    // Draw the right border of the table
    // todo
    
    /* Drawing the columns */
    
    // Draw the leftmost column
    cell_x = table_x; // Include L Border, and then cell x just is the leftmost border of the cell, shifted per column
    cell_y = table_y+1;  // Initial cell y starts one below the top border
    for (int row = 0; row < NUM_TABLE_ROWS; row++) {
        
        mvaddwstr(cell_y, cell_x, L"│"); // draw left border
        mvaddwstr(cell_y+1, cell_x, L"├"); // Draw bottom left corner
        addwstr(row_bar);
        mvaddwstr(cell_y, cell_x + 1, yahtzeeHands[row]);
        cell_y+=2;
        
    }
    
    
    // Draw internal columns
    for (int col = 1; col < numCols; col++)
    {
        cell_x = table_x + (row_width_exclusive + 1) * col; // Include L Border, and then cell x just is the leftmost border of the cell, shifted per column
        cell_y = table_y+1;  // Initial cell y starts one below the top borde
        for (int row = 0; row < NUM_TABLE_ROWS; row++) {
            
            // Draw left border
            mvaddwstr(cell_y, cell_x, L"│"); // draw left border
            mvaddwstr(cell_y+1, cell_x, L"┼"); // Draw bottom left corner
            addwstr(row_bar);
            mvaddwstr(cell_y, cell_x + 1, yahtzeeHands[row]);
            cell_y+=2;
            
        }
    }
 
    // ORDER DOES MATTER WHEN DRAWING BORDERS.
    
    // Draw the right border of the table
    int table_R_corner_x = table_x + (row_width_exclusive + 1) * numCols;  // Origin + row_width_include_L_Border * num_cols
    for (int row = 0; row < NUM_TABLE_ROWS; row++) {
        // Move to y of current column, and back to the right column
        mvaddwstr((table_y+1) + 2*row, table_R_corner_x, L"│");
        mvaddwstr((table_y+2) + 2*row, table_R_corner_x, L"┤");
    }

       
    // Draw the bottop border of the table
    mvaddwstr(table_y + NUM_TABLE_ROWS * 2, table_x, L"╰"); // each cell is 2 high: row containing contents, and bottom border.
    for (int i = 0; i < numCols; i++) {
        addwstr(row_bar);
        if (i < numCols-1) addwstr(L"┴");
        else addwstr(L"╯");
    }
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

Game *setup_game() {
    
    // This is the types of hands that you can win. 
    // Feel free to edit this, just make sure to add a function in todo that handles rules for that category
    wchar_t *yahtzeeHands[] = { 
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
    
}

int main(void) {
    setlocale(LC_ALL, "");
    

    const int NUM_YAHTZEE_HANDS = 14;
    const int NUM_TABLE_ROWS = NUM_YAHTZEE_HANDS + 1; // Total isnt a hand, but it is in the rows
    const int MAX_WSTR = 25;


    wchar_t *row_bar = {L"────────────────"};
    const int row_width_exclusive = wcslen(row_bar);

    const int numCols = 3;
   

    initscr();




    

    refresh();
    getch();
    
    endwin();
    printf("%lc", ACS_HLINE);
}

