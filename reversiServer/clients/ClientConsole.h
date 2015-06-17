//
// Created by vanya on 10.06.15.
//

#ifndef REVERSISERVER_CLIENTCONSOLE_H
#define REVERSISERVER_CLIENTCONSOLE_H


#include "ClientInterface.h"

class ClientConsole : public ClientInterface{
public:
    virtual GameState makeTurn(GameState state);
};


#endif //REVERSISERVER_CLIENTCONSOLE_H
