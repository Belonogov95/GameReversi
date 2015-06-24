//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_TCPSOCKETCLIENT_H
#define HWW2_TCPSOCKETCLIENT_H

#include <bits/stdc++.h>
#include "Executor.h"
#include "FileDescriptor.h"
#include "SmartSocket.h"

using namespace std;
typedef function<void(int, u_int32_t )> OnReceive;

const int TEMP_SIZE1 = 1000;

class TcpSocketClient {
public:

    TcpSocketClient(int serverDescriptor, Executor *executor, OnReceive onReceive);

    deque < char > buffer;
    FileDescriptor socketDescriptor;
    SmartSocket smartSocket;
    Executor * executor;
    u_int32_t flagMask;


    int read(string &buffer);

    void write(const string & data);

    void setWrite(int flag);

    void writeFromEpoll();

    int readyToWrite();

};

void makeSocketNonBlocking(int socketDescriptor);

#endif //HWW2_TCPSOCKETCLIENT_H
