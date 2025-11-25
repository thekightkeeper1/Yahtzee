#if !defined(GAME_TUI_H)
#define GAME_TUI_H

#include "yahtzee/game.h"
#include "tui.h"

typedef struct {
    wstr_t *playerNames;  // Names of players,
    Table scoreboardTUI;
    int numPlayers;
} Game;

extern wstr_t CATEGORY_LABELS[NUM_CATEGORIES];

Table alloc_score_table(int numHands, const wstr_t *playerNames, int numPlayers);
void free_score_table(Table t);
Game setup_game_ui(wstr_t* playerNames, int numPlayers);
void set_score_and_update(int c, int r, int value, Table t);

#endif // GAME_TUI_H

