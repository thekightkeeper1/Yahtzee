#if !defined(TUI_H)
#define TUI_H


#define _XOPEN_SOURCE_EXTENDED /* Required for ncurses to actually use the unicode funcs MUST GO FIRST*/

#include "wchar.h"
#include "./yahtzee/game.h"

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

typedef enum {
    EMPTY_CELL,
    FILLED_SCORE,
    LABEL_CELL
} color_pair_t;

// Labels used for the score table. Fine to change this the table is all dynamic.
const wstr_t YAHTZEE_HANDS[NUM_CATEGORIES] = {
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
    L"Upper total",
    L"Lower total",
    L"Total",
};


Table make_autosized_table(wstr_t **columns, int numCols, int columnHeight);
void get_cell_pos(int r, int c, int* x, int* y, Table t);
void draw_table(Table t);
int maxlen(wchar_t **arr, int n);
void repeat(wchar_t* dest, wchar_t c, int n);
void malloc_failed();
void free_autosized_table(Table t);
void draw_table_data(Table t);
void draw_cell_data(Table t, int c, int r, color_pair_t color);
void test_menu(wchar_t** choices_w, int n_choices);




#endif // TUI_H
