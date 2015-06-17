//
// Created by vanya on 10.06.15.
//

#include "ClientConsole.h"


GameState ClientConsole::makeTurn(GameState state) {
    if (!state.isPossibleMove()) {
        printf("You cann't make turn\n");
        return state;
    }
    state.printBoard();
    bool flagRev = 0;
    if (state.player == 2) {
        flagRev = 1;
        state.reverseColor();
    }
    int x, y;
    bool flag = 1;
    //state.printBoard();
    for (; flag;) {
        flag = 0;
        if (scanf("%d %d", &x, &y) != 2) {
            flag = 1;
            char buffer[100];
            fgets(buffer, sizeof(buffer), stdin);
            printf("Incorrect turn. Bad format. Try again\n");
        }
        else if (!state.check(x, y)) {
            flag = 1;
            printf("Incorrect turn. Not in the field. Try again\n");
        }
        else {
            GameState next;
            if (!state.go(x, y, next, 1)) {
                flag = 1;
                printf("Incorrect turn. CntFlip == 0. Try again\n");
            }
        }
    }
    GameState next;
    state.go(x, y, next);
    if (flagRev)
        next.reverseColor();
    return next;
}


