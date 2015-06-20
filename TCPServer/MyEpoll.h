//
// Created by vanya on 02.06.15.
//

#ifndef HWW2_MYEPOLL_H
#define HWW2_MYEPOLL_H

#include <sys/epoll.h>
#include <map>
#include "MyClient.h"
#include <memory>


const int MAX_EVENTS = 2;
const int WAITING_ACCEPT = 1;
const int WAITING_READ_OR_WRITE = 2;


using namespace std;

class MyClient;

class MyEpoll {
private:
    int epollDescriptor;
    int pipeFD;
    map < int, void (*) (shared_ptr < MyClient > ) > onReceiveMap;
    map < int, int > socketDescriptorType;
    map < int, int > portFromDescriptor;
    map < int, shared_ptr < MyClient > > clientFromDescriptor;

public:
    friend class MyClient;

    void start();

    MyEpoll();
    ~MyEpoll();

    void write(MyClient * myClient);
    void add(int port, string ipAddress, void (*onReceive)(shared_ptr<MyClient>));
    int getPipe();
};


#endif //HWW2_MYEPOLL_H
