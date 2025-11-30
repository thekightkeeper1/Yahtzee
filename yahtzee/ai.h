//
// Created by ty on 11/30/25.
//

#ifndef YAHTZEE_AI_H
#define YAHTZEE_AI_H

#include <stdlib.h>
#include "game.h"

u_int8_t ai_choose_locked(Yahtzee y);
CATEGORIES ai_choose_score(Yahtzee y);
bool ai_is_turn(Yahtzee y);

#endif //YAHTZEE_AI_H