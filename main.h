#if !defined(MAIN_H)
#define MAIN_H
#include <wchar.h>


typedef wchar_t* wstr_t;
void repeat(wchar_t* dest, wchar_t c, const int n);

typedef struct {
   
    // Also, to know location of each cell, we need to know the sum of widths before it, hence the cummulative widths.
    int x, y;
    int numRows, numCols;
    int* cumulativeWidths;  // Guarenteed to have size numCols + 1
    // We cache strings for the bottoms of cells.
    // If i could draw taller cells i would probably cache the verticle separator
    wstr_t *cellBottoms;  // Size numCols + 1

} Table;

typedef struct {
    wstr_t *handCategories;  // Names of categories of hands you can score
    wstr_t *playerNames;  // Names of players,
    Table scoreboard;
    int numHands;
    int numPlayers;
    bool *isAI;
    int **scores;

} Game;


#endif // MAIN_H

