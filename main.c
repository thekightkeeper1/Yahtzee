#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>

#include "main.h"
#include "tui.h"
#include "game_tui.h"



int main(void) {
    setlocale(LC_ALL, "");

    SCREEN *scr = newterm(NULL, stdout, stdin);

    wstr_t names[] = {L"Belinda", L"Sophie", L"Jason"};
    Game game = setup_game(names, 3);
    draw_table(game.scoreboard);
    getch();
    free_score_table(game.scoreboard);
    endwin();
    delscreen(scr);
    exit(0);
    int x, y;

    // endwin();
}

