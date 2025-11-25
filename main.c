#include <stdio.h>
#include <locale.h>
#include "main.h"
#include "tui.h"
#include "game_tui.h"
#include <ncursesw/ncurses.h>




int main(void) {
    setlocale(LC_ALL, "");

    wstr_t names[] = {L"Blinda", L"Sophie", L"Jason", L"ty"};
     Game game = setup_game_ui(names, 4);


    const Table t = game.scoreboardTUI;
    for (int c = 0; c < t.numCols; c++) {
        for (int r = 0; r < t.numRows; r++) {

            // wprintf(L"%ls\n", t.columnData[c][r]);

        }
    }

    noecho();
    SCREEN *scr = newterm(NULL, stdout, stdin);
    // start_color();
    // init_color(COLOR_GREEN, 100, 600, 100);
    // init_color(COLOR_BLACK, 100, 100, 100);
    // init_pair(FILLED_SCORE, COLOR_GREEN, COLOR_BLACK);
    draw_table(game.scoreboardTUI);
    draw_table_data(t);

    // game.scoreboard.x = 60;
    // attron(A_BOLD);
    // draw_table(game.scoreboard);
    // draw_table_data(game.scoreboard);
    // set_score_and_update(3, 1, 2000, t);
    // test_menu(names, 4);
    getch();
    endwin();
    delscreen(scr);

    free_score_table(game.scoreboardTUI);
    // endwin();
}

