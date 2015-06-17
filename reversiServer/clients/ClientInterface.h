//
// Created by vanya on 09.06.15.
//

#ifndef REVERSISERVER_CLIENTINTERFACE_H
#define REVERSISERVER_CLIENTINTERFACE_H


#include "../GameState.h"
#include <memory>

class ClientInterface {
public:
    virtual GameState makeTurn(GameState state) = 0;
};


#endif //REVERSISERVER_CLIENTINTERFACE_H
