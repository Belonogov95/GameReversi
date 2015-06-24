//
// Created by vanya on 18.06.15.
//
#include <bits/stdc++.h>
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
    shared_ptr < TcpSocketClient > client;

public:

    HttpWorker(const shared_ptr<TcpSocketClient> &client);

    void printBuff();

    void sendFile(string path);

    pair<int, Message > readMessage();

    void writeBuffer(u_int32_t flagMask);

    void sendString(string message, string status = "200 OK");
};




#endif //HWW2_HTTPWORKER_H
