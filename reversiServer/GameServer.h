//
// Created by vanya on 10.06.15.
//

#ifndef REVERSISERVER_GAMESERVER_H
#define REVERSISERVER_GAMESERVER_H


#include "clients/ClientInterface.h"
#include <memory>

class GameServer {


    bool checkNoMoves(GameState state);

    string calcResult(GameState state);

public:
    void play(shared_ptr<ClientInterface> a, shared_ptr<ClientInterface> b);
};


#endif //REVERSISERVER_GAMESERVER_H
