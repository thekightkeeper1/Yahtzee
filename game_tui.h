#if !defined(GAME_TUI_H)
#define GAME_TUI_H

#include <stdbool.h>

#include "tui.h"

typedef struct {
    wstr_t *playerNames;  // Names of players,
    Table scoreboardTUI;
    int numPlayers;
} Game;

extern wstr_t CATEGORY_LABELS[NUM_CATEGORIES];

Table alloc_score_table(int numHands, const wstr_t *playerNames, int numPlayers);
void free_score_table(Table t);
wchar_t** int_array_to_wchar_array(const int nums[], wchar_t** strs, int n, int len);
Game setup_game_ui(wstr_t* playerNames, int numPlayers);
void set_score_and_update(int c, int r, int value, Table t);

#endif // GAME_TUI_H

