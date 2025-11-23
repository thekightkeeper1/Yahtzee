#include "tui.h"

#include <assert.h>


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
        int len = wcslen(arr[i]);
        if (len > max) {
            max = len;
        }
    }
    return max;
}


// Gets the x y position of a table cell. It returns writeable area, not where the border is
void get_cell_pos(const int r, const int c, int* x, int* y, const Table t) {
    assert(r < t.numRows && c < t.numCols);

    *y = t.y + 1 + 2*r; // Origin, shift down one to ignore the top border, and then rows are 2 high, so go down 2 per each 
    *x = t.x + 1 + t.cumulativeWidths[c]; // Origin, shifted by cummulative effect
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
