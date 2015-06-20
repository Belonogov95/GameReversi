//
// Created by vanya on 17.06.15.
//

#ifndef REVERSISERVER_CLIENTBOT_H
#define REVERSISERVER_CLIENTBOT_H


#include "ClientInterface.h"

class ClientBot : public ClientInterface {
public:
    virtual GameState makeTurn(GameState state);
};


#endif //REVERSISERVER_CLIENTBOT_H
