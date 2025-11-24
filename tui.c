#include "tui.h"

#include <assert.h>
#include <stdlib.h>
#include <ncursesw/menu.h>

void test_menu(wchar_t** choices_w, int n_choices) {

    char **choices = malloc(sizeof(char*) * n_choices);
    for (int i = 0; i < n_choices; i++) {
        choices[i] = malloc(4 * wcslen(choices_w[i]));
        wcstombs(choices[i], choices_w[i], wcslen(choices_w[i]) * 4);

    }

    ITEM **my_items;
    int c;
    MENU *my_menu;
    int i;
    ITEM *cur_item;

    keypad(stdscr, TRUE);
    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

    for(i = 0; i < n_choices; ++i)
        my_items[i] = new_item(choices[i], choices[i]);
    my_items[n_choices] = (ITEM *)NULL;

    move(3, 60);
    my_menu = new_menu((ITEM **)my_items);
    // mvprintw(LINES - 2, 0, "F1 to Exit");
    post_menu(my_menu);
    refresh();

    while((c = getch()) != KEY_F(1))
    {   switch(c)
    {	case KEY_DOWN:
            menu_driver(my_menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(my_menu, REQ_UP_ITEM);
            break;
    }
    }

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    for ( i = 0; i < n_choices; i++) {
        free(choices[i]);
    }
    free(choices);
    endwin();
}


void malloc_failed() {
    endwin();
    printf("There was an issue allocating memory, sorry mate.\n");
    exit(1);
}

// Repeats given wchar_t n times and returns the resultant string.
void repeat(wchar_t* dest, const wchar_t c, const int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = c;
    }
    dest[i] = L'\0'; 
}

int maxlen(wchar_t **arr, int n) {
    int max = 0;
    for (int i = 0; i < n; i++) {
        const int len = (int)wcslen(arr[i]);
        if (len > max) {
            max = len;
        }
    }
    return max;
}

// Allocates a table where the column widths are automatically determined
// columns is the data
// num columns is number of columns in the table

Table make_autosized_table(wstr_t **columns, int numCols, int columnHeight) {
    int *colWidths = malloc(sizeof(int) * numCols); 
    int *cumulativeWidths = malloc(sizeof(int) * (numCols + 1));  // Cummulative widths are shifted. Always the first cell=0, and we need one extra to get the last cell=sum(widths)
    wstr_t *cellBottoms = malloc(sizeof(wstr_t) * numCols);

    if (colWidths == NULL || cumulativeWidths == NULL || cellBottoms == NULL) {
        malloc_failed();
        exit(1);
    }

    // Determining the width of each column
    // We find the largest in the entire column
    
    for (int c = 0; c < numCols; c++)
    {
        colWidths[c] = maxlen(columns[c], columnHeight)+2;
    }

    // Getting the cumulative widths
    cumulativeWidths[0] = 0;
    for (int i = 1; i < numCols+1 ; i++) {
        // Sum of previous cumulative width, plus the column we just passed, plus 1 for the right border
        cumulativeWidths[i] = cumulativeWidths[i-1] + colWidths[i-1] + 1;
    }

    // Getting the strs cached for making the horizontal border between cells
    for (int i = 0; i < numCols; i++) {
        cellBottoms[i] = malloc(sizeof(wchar_t) * (colWidths[i] + 1)); // Originally messed up parenthesis
        if (cellBottoms[i] == NULL) malloc_failed();
        repeat(cellBottoms[i], L'─', colWidths[i]);
    }

    const Table t = {
        .x = 0,
        .y = 0,
        .numCols = numCols,
        .numRows = columnHeight,
        .cumulativeWidths = cumulativeWidths,
        .cellBottoms = cellBottoms,
        .colWidths = colWidths,
        .columnData = columns,
    };
    return t;
    
}

void free_autosized_table(Table t) {
    // Freeing the deepest stuff first, the arrays of strs
    for (int i = 0; i < t.numCols; i++) {
        free(t.cellBottoms[i]);
    }
    // Freeing all the toplevel properties
    free(t.cellBottoms);
    free(t.cumulativeWidths);
    free(t.colWidths);
}

// Gets the x y position of a table cell. It returns writeable area, not where the border is
void get_cell_pos(const int r, const int c, int* x, int* y, const Table t) {
    assert(r < t.numRows && c < t.numCols);

    *y = t.y + 1 + 2*r; // Origin, shift down one to ignore the top border, and then rows are 2 high, so go down 2 per each 
    *x = t.x + 1 + t.cumulativeWidths[c]; // Origin, shifted by cummulative effect
}


void draw_table(const Table t) {
    
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
                mvaddwstr(cellY, t.x + t.cumulativeWidths[col+1], L"│");
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

void draw_table_data(const Table t) {
    for (int c = 0; c < t.numCols; c++) {
        for (int r = 0; r < t.numRows; r++) {
            int y,x;
            get_cell_pos(r, c, &x, &y, t);
            mvaddwstr(y, x, t.columnData[c][r]);
        }
    }
}

void draw_cell_data(Table t, int c, int r, color_pair_t color) {
    int y, x;
    get_cell_pos(r, c, &x, &y, t);
    move(y, x);
    // Clearing old ce
    for (int i = 0; i < t.colWidths[c]; i++) {
        addwstr(L" ");
    }
    attron(COLOR_PAIR(color));
    mvaddwstr(y, x+1, t.columnData[c][r]);
    attroff(COLOR_PAIR(color));
}
