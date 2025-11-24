#include <stdio.h>
#include <locale.h>
#include <unistd.h>
#include <stdlib.h>

#include "main.h"
#include "tui.h"
#include "game_tui.h"



int main(void) {
    setlocale(LC_ALL, "");

    wstr_t names[] = {L"Blinda", L"Sophie", L"Jason", L"ty"};
    const Game game = setup_game(names, 4);


    const Table t = game.scoreboard;
    for (int c = 0; c < t.numCols; c++) {
        for (int r = 0; r < t.numRows; r++) {

            // wprintf(L"%ls\n", t.columnData[c][r]);

        }
    }




    SCREEN *scr = newterm(NULL, stdout, stdin);
    draw_table(game.scoreboard);
    draw_table_data(t);
    getch();
    endwin();
    delscreen(scr);

    free_score_table(game.scoreboard);
    // endwin();
}

