//
// Created by vanya on 02.06.15.
//
#include <sys/epoll.h>
#include <map>
//#include <bits/shared_ptr.h>
#include "MySocket.h"
#include "MyClient.h"
#include <memory>

#ifndef HWW2_MYEPOLL_H
#define HWW2_MYEPOLL_H
#define db(x) cerr << #x << " = " << x << endl

const int MAX_EVENTS = 2;
const int WAITING_ACCEPT = 1;
const int WAITING_READ_OR_WRITE = 2;


using namespace std;

class MyEpoll {
private:
    int epollDescriptor;
    bool running;
    map < int, void (*) (shared_ptr < MyClient > ) > onAcceptMap;
    map < int, void (*) (shared_ptr < MyClient > ) > onReceiveMap;
    map < int, int > socketDescriptorType;
    map < int, int > portFromDescriptor;
    map < int, shared_ptr < MyClient > > clientFromDescriptor;

public:


    void start();

    MyEpoll();
    ~MyEpoll();

    void write(shared_ptr<MyClient> myClient);

    void add(MySocket mySocket, void (*onAccept)(shared_ptr<MyClient>), void (*onReceive)(shared_ptr<MyClient>));
};


#endif //HWW2_MYEPOLL_H
