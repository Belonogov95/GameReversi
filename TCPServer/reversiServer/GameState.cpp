//
// Created by vanya on 09.06.15.
//

#include <bits/stdc++.h>
#include "GameState.h"
#include "debug.h"

int GameState::getCntUsed() {
    return __builtin_popcountll(used);
}

int GameState::getCntWhite() {
    return __builtin_popcountll(white);
}

int GameState::getCntBlack() {
    return getCntUsed() - getCntWhite();
}


GameState::GameState() {
    int data[8][8];
    memset(data, 0, sizeof(data));
//    data[3][3] = 1;
//    data[4][4] = 1;
//    data[3][4] = 2;
//    data[4][3] = 2;

    data[0][3] = 1;
    data[0][4] = 2;
    data[7][3] = 2;
    data[7][4] = 1;
    data[3][3] = 2;

    code(data);
    player = 1;
    finished = 0;
}

void GameState::decode(int data[8][8]) {  /// 1 - white ;; 2 - black
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            int t = i * 8 + j;
            if (((used >> t) & 1) == 1)
                data[i][j] = (((white >> t) & 1) == 1) ? 1 : 2;
            else
                data[i][j] = 0;
        }
}

void GameState::code(int data[8][8]) {
    used = white = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            int t = i * 8 + j;
            if (data[i][j] != 0) {
                used |= (1ll << t);
                if (data[i][j] == 1)
                    white |= (1ll << t);
            }
        }
}

bool GameState::makeMove(int x, int y, shared_ptr < GameState > result) {
    assert(check(x, y)); int data[8][8];
    decode(data);
    if (data[x][y] != 0) {
        return false;
    }
    int countFlip = 0;
    int myColor = player;
    int enemyColor = 3 - player;
    data[x][y] =  myColor;
    for (int dir = 0; dir < 8; dir++) {
        int x1 = x + dx[dir];
        int y1 = y + dy[dir];
        int cntEnemy = 0;
        for (; check(x1, y1) && data[x1][y1] == enemyColor; x1 += dx[dir], y1 += dy[dir], cntEnemy++);
        if (check(x1, y1) && data[x1][y1] == myColor) {
            countFlip += cntEnemy;
            x1 = x + dx[dir];
            y1 = y + dy[dir];
            for (; check(x1, y1) && data[x1][y1] == enemyColor; x1 += dx[dir], y1 += dy[dir])
                data[x1][y1] = myColor;
        }
    }
    if (countFlip == 0) {
        return false;
    }
    result->code(data);
    result->player = 3 - player;
    return true;
}


bool GameState::isPossibleMove() {
    int data[8][8];
    decode(data);
    int count = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            shared_ptr < GameState > tmp(new GameState());
            count += makeMove(i, j, tmp);
        }
    return count > 0;
}

bool GameState::check(int x, int y) {
    return 0 <= x && x < 8 && 0 <= y && y < 8;
}

void GameState::printBoard() {
    int data[8][8];
    decode(data);
    cout << ((player == 1)? "White": "Black") << endl;
    printf("#");
    for (int i = 0; i < 8; i++)
        printf("%d", i);
    printf("\n");
    for (int i = 0; i < 8; i++) {
        printf("%d", i);
        for (int j = 0; j < 8; j++) {
            if (data[i][j] == 0)
                printf(".");
            else if (data[i][j] == 1)
                printf("W");
            else
                printf("B");
        }
        printf("%d\n", i);
    }
    printf("#");
    for (int i = 0; i < 8; i++)
        printf("%d", i);
    printf("\n");
}




void GameState::nextTurn() {
    player = 3 - player;
}

string GameState::toJSArray() {
    int data[8][8];
    decode(data);
    string result = "[";
    for (int i = 0; i < 8; i++) {
        result += "[";
        for (int j = 0; j < 8; j++) {
            result += '0' + data[i][j];
            if (j + 1 != 8)
                result += ", ";
        }
        result += "]";
        if (i + 1 != 8)
            result += ", ";
    }
    result += "]";
    return result;
}


