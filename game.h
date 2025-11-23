#if !defined(GAME_H)
#define GAME_H

#include <stdbool.h>

#include "tui.h"

typedef struct {
    wstr_t *handCategories;  // Names of categories of hands you can score
    wstr_t *playerNames;  // Names of players,
    Table scoreboard;
    int numHands;
    int numPlayers;
    bool *isAI;
    int **scores;

} Game;

Table alloc_score_table(wstr_t *hands, int numHands, wstr_t* playerNames, int numPlayers);
void free_score_table(Table t);
wchar_t** int_array_to_wchar_array(const int nums[], wchar_t** strs, const int n, const int len);
Game setup_game(wstr_t* playerNames, const int numPlayers);

#endif // GAME_H

