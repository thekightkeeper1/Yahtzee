#if !defined(TUI_H)
#define TUI_H


#define _XOPEN_SOURCE_EXTENDED /* Required for ncurses to actually use the unicode funcs MUST GO FIRST*/

#include "wchar.h"
#include <ncursesw/ncurses.h>

typedef wchar_t* wstr_t;

typedef struct {
    
    // Also, to know location of each cell, we need to know the sum of widths before it, hence the cummulative widths.
    int x, y;
    int numRows, numCols;
    int* cumulativeWidths;  // Guarenteed to have size numCols + 1
    int* colWidths;
    // We cache strings for the bottoms of cells.
    // If i could draw taller cells i would probably cache the verticle separator
    wstr_t *cellBottoms;  // Size numCols + 1
    wstr_t **columnData;
    
} Table;

void get_cell_pos(const int r, const int c, int* x, int* y, const Table t);
void draw_table(Table t);
int maxlen(wchar_t **arr, int n);
void repeat(wchar_t* dest, const wchar_t c, const int n);
void malloc_failed();
Table make_autosized_table(wstr_t **columns, int numCols, int columnHeight);
void free_autosized_table(Table t);
void draw_table_data(const Table t);


#endif // TUI_H
