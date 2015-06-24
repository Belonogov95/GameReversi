//
// Created by vanya on 09.06.15.
//

#ifndef REVERSISERVER_GAMESTATE_H
#define REVERSISERVER_GAMESTATE_H

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include <queue>
#include <ctime>
#include <cassert>
#include <cstdio>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <bitset>

using namespace std;

typedef unsigned long long ull;
const int dx[8] = {1, 1, 1, 0, 0, -1, -1, -1};
const int dy[8] = {1, -1, 0, 1, -1, 0, 1, -1};


class GameState {

public:
    GameState();

    ull used;
    ull white;
    int player;                   /// 1 - white; 2 - black
    bool finished;

    int getCntUsed();

    int getCntWhite();

    void decode(int data[8][8]);   /// 1 - white ;; 2 - black

    void code(int data[8][8]);

    bool check(int x, int y);

    void printBoard();

    void nextTurn();

    string toJSArray();

    bool isPossibleMove();

    int getCntBlack();

    bool makeMove(int x, int y, shared_ptr <GameState> result);
};


#endif //REVERSISERVER_GAMESTATE_H
