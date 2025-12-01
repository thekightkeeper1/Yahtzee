//
// Created by ty on 11/30/25.
//

#ifndef YAHTZEE_BASIC_TUI_H
#define YAHTZEE_BASIC_TUI_H

#include "../include/game.h"
typedef char* str;
void print_scoreboard(Yahtzee y, int player);
void print_dice(Yahtzee y);

extern str CATEGORY_LABELS[NUM_CATEGORIES];

#endif //YAHTZEE_BASIC_TUI_H