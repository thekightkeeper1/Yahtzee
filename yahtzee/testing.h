#pragma once

#include "game.h"
void print_dice(Yahtzee y);
void print_buffer_score(Yahtzee y);

typedef wchar_t* wstr_t;

// The labels themselves can be changed for styling. The number and position cannot
extern wstr_t CATEGORY_LABELS[NUM_CATEGORIES];
void print_scoreboard(Yahtzee y);