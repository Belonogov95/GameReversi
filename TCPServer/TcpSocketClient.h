//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_TCPSOCKETCLIENT_H
#define HWW2_TCPSOCKETCLIENT_H

#include <bits/stdc++.h>
#include "Executor.h"

using namespace std;

const int TEMP_SIZE1 = 1000;

class TcpSocketClient {
public:
    TcpSocketClient(int socketDescriptor, Executor *executor);
    ~TcpSocketClient();

    deque < char > buffer;
    int socketDescriptor;
    Executor * executor;
    u_int32_t flagMask;


    int read(string &buffer);

    void write(string data);

    void setWrite(int flag);

    void writeFromEpoll();

    int readyToWrite();

};

void makeSocketNonBlocking(int socketDescriptor);

#endif //HWW2_TCPSOCKETCLIENT_H
