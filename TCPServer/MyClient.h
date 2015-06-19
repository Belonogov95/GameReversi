//
// Created by vanya on 02.06.15.
//

#ifndef HWW2_MYCLIENT_H
#define HWW2_MYCLIENT_H


#include <array>
#include <vector>
#include <iostream>
#include <memory>
#include "MyEpoll.h"

using namespace std;

const int BACK_LOG = 10;

class MyEpoll;

class MyClient {
private:


    int port;
    int socketDescriptor;
    int epollDescriptor;
    u_int32_t flagMask;
    vector<char> buffer;
    int bufferCursor;
    bool closed;
    MyEpoll * myEpoll;


    MyClient(int port, int socketDescriptor, int epollDescriptor, MyEpoll * myEpoll);

public:
    friend class MyEpoll;

    MyClient(int port);

    void setRead(int flag);

    void setWrite(int flag);

    int readyToWrite();

    void closeClient();

    void makeSocketNonBlocking();

    int getSocketDescriptor();

    int getPort();

    void write(string s);

    int read(string &buffer);
};


#endif //HWW2_MYCLIENT_H
