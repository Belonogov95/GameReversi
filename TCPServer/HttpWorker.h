//
// Created by vanya on 18.06.15.
//
#include <bits/stdc++.h>
#include "MyClient.h"
#include "Message.h"
#include "TcpSocketClient.h"

#ifndef HWW2_HTTPWORKER_H
#define HWW2_HTTPWORKER_H

using namespace std;
const string LINE_BREAK = "\r\n";
const int BUF_SZ = 10000;

class HttpWorker {

private:
    deque < string > innerBuffer;

public:

    void printBuff();

    void sendFile(string path, shared_ptr<TcpSocketClient> client);

    pair<int, Message > readMessage(shared_ptr<TcpSocketClient> client);

    void sendString(string data, shared_ptr<TcpSocketClient> client);
};




#endif //HWW2_HTTPWORKER_H
