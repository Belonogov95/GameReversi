//
// Created by vanya on 02.06.15.
//
#include <sys/epoll.h>
#include <map>
#include "MySocket.h"
#include "MyClient.h"

#ifndef HWW2_MYEPOLL_H
#define HWW2_MYEPOLL_H

const int MAX_EVENTS = 2;
const int WAITING_ACCEPT = 1;
const int WAITING_READ_OR_WRITE = 2;


using namespace std;

class MyEpoll {
    int epollDescriptor;
    bool closed;
    map < int, void (*) (MyClient) > onAcceptMap;
    map < int, void (*) (MyClient) > onReceiveMap;
    map < int, int > socketDescriptorType;
    MyEpoll();

public:

    void add(MySocket mySocket, void (*onAccept)(MyClient), void (*onReceive)(MyClient));

    void start();

};


#endif //HWW2_MYEPOLL_H
