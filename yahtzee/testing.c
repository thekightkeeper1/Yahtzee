#include  "game.h"
#include <stdio.h>

// Function for testing
void test_dice() {

}


// Runs some basic testing stuff. Pass args to it to test particular things?
int main(int argc, char *argv[]) {
    const char* arg = argv[1];

    if (arg == NULL) {
        printf("no args provided \n");
        exit(1);
    }

    int caseNum = atoi(arg);
    switch (caseNum) {
        case 1:
            break;
        default:
            break;
    }

}