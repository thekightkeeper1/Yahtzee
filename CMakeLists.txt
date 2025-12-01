cmake_minimum_required(VERSION 4.0)
project(Yahtzee C)

set(CMAKE_C_STANDARD 11)

include_directories(

        testing
        ${CMAKE_SOURCE_DIR}/inlcude
)

add_executable(Yahtzee
        src/game_tui.c
        include/game_tui.h
        main.c
        src/tui.c
        include/tui.h)

add_executable(test_case_1
        src/game.c
        include/game.h
        testing/testing.c
        testing/testing.h
        src/ai.c
        include/ai.h
)

add_executable(prod
        main.c
        src/game.c
        include/game.h
        src/ai.c
        include/ai.h
        src/basic_tui.c
        include/basic_tui.h
)