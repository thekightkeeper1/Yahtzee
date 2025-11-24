#if !defined(GAME_TUI_H)
#define GAME_TUI_H

#include <stdbool.h>

#include "tui.h"

typedef struct {
    wstr_t *handCategories;  // Names of categories of hands you can score
    int numCategories;
    wstr_t *playerNames;  // Names of players,
    Table scoreboardTUI;
    int numPlayers;
    int numHands;
    bool *isAI;
    int **scores;

} Game;

Table alloc_score_table(wstr_t *hands, int numHands, wstr_t* playerNames, int numPlayers);
void free_score_table(Table t);
wchar_t** int_array_to_wchar_array(const int nums[], wchar_t** strs, int n, int len);
Game setup_game_ui(wstr_t* playerNames, int numPlayers);
void set_score_and_update(int c, int r, int value, Table t);

#endif // GAME_TUI_H

