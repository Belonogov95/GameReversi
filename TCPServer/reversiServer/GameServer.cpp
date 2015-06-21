//
// Created by vanya on 10.06.15.
//


#include "GameServer.h"
#include "debug.h"

bool GameServer::checkNoMoves(GameState state) {
    GameState stateR = state;
    stateR.reverseColor();
    return !stateR.isPossibleMove() && !state.isPossibleMove();
}

string GameServer :: calcResult(GameState state) {
    int white = state.getCntWhite();
    int black = state.getCntUsed() - state.getCntWhite();
    string s = "(White : Black) = (" + to_string(white) + " : " + to_string(black) + ")\n";
    if (white > black)
        s += "White win\n";
    else if (white < black)
        s += "Black win\n";
    else
        s += "Draw\n";
    return s;
}



void GameServer::play(shared_ptr < ClientInterface >  a, shared_ptr < ClientInterface > b) {
    GameState state;

    for (int turn = 1; ; turn++) {
//        state.printBoard();
//        GameState tmp;
//        db("uurra");
//        cout << state.go(2, 4, tmp) << endl;
//        return;
        if (checkNoMoves(state) || state.getCntUsed() == 64) {
            cout << calcResult(state) << endl;
            return;
        }
        db(state.player);
        state = a->makeTurn(state);
        db(state.player);
        state = b->makeTurn(state);
    }

}



