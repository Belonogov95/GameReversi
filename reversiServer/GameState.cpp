//
// Created by vanya on 09.06.15.
//

#include "GameState.h"
#include "debug.h"

int GameState::getCntUsed() {
    return __builtin_popcountll(used);
}

int GameState::getCntWhite() {
    return __builtin_popcountll(white);
}

void GameState::reverseColor() {
    ull newMask = 0;
    for (int i = 0; i < 64; i++) {
        if (((used >> i) & 1) == 1 && ((white >> i) & 1) == 0)
            newMask |= 1ull << i;
    }
    white = newMask;
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

bool GameState::go(int x, int y, GameState &res, bool FLAG) {
    assert(check(x, y));
    int data[8][8];
    decode(data);
//
//    if (FLAG) {
//        db2(x, y);
//        for (int i = 0; i < 8; i++, cerr << endl)
//            for (int j = 0; j < 8; j++)
//                cout << data[i][j] << " ";
//    }
    if (data[x][y] != 0) return false;
    int cntFlip = 0;
    data[x][y] = 1;
    for (int dir = 0; dir < 8; dir++) {
        int x1 = x + dx[dir];
        int y1 = y + dy[dir];
        int cntBlack = 0;
        for (; check(x1, y1) && data[x1][y1] == 2; x1 += dx[dir], y1 += dy[dir], cntBlack++);
//        db(cntBlack);
        if (check(x1, y1) && data[x1][y1] == 1) {
//            cerr << "here " << cntBlack << endl;
            cntFlip += cntBlack;
            x1 = x + dx[dir];
            y1 = y + dy[dir];
            for (; check(x1, y1) && data[x1][y1] == 2; x1 += dx[dir], y1 += dy[dir])
                data[x1][y1] = 1;
        }
    }
//    db(cntFlip);
    if (cntFlip == 0) return false;

    res.code(data);
    res.player = player;
    return true;
}

//void checkState() {
//assert(getCntWhite() * 2 == getCntUsed());
//}
bool GameState::isPossibleMove() {
    int data[8][8];
    decode(data);
    int cnt = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            GameState tmp;
            cnt += go(i, j, tmp);
        }
    return cnt > 0;
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
//    printf("^\n");
//    printf("|\n");
//    printf("x\n");

}



GameState::GameState() {
    int data[8][8];
    memset(data, 0, sizeof(data));
    data[3][3] = 1;
    data[4][4] = 1;
    data[3][4] = 2;
    data[4][3] = 2;

    code(data);
    player = 1;
}

void GameState::nextTurn() {
    player = 3 - player;
}


