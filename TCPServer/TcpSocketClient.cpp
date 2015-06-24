//
// Created by vanya on 21.06.15.
//

#include "TcpSocketClient.h"
#include "debug.h"
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <unistd.h>

using namespace std;

TcpSocketClient::TcpSocketClient(int socketDescriptor, Executor * executor): socketDescriptor(socketDescriptor),
                                                                              executor(executor), flagMask(EPOLLIN){
    makeSocketNonBlocking(socketDescriptor);
}

TcpSocketClient::~TcpSocketClient() {
    executor->del(socketDescriptor);
    int r = close(socketDescriptor);
    myAssert(r == 0);
}

int TcpSocketClient::read(string &buffer) {
    int readLen = (int) recv(socketDescriptor, &buffer[0], buffer.size(), 0);
    myAssert(readLen != -1 || errno == 11);
    return readLen;
}

void TcpSocketClient::write(string data) {
    for (auto x: data)
        buffer.push_back(x);
    setWrite(1);
    writeFromEpoll();
}

int TcpSocketClient::readyToWrite() {
    return buffer.size();
}

void TcpSocketClient::setWrite(int flag) {
//    db(flag);
    flagMask |= EPOLLOUT;
    if (flag == 0)
       flagMask ^= EPOLLOUT;
//    db(flagMask);
    executor->changeFlags(socketDescriptor, flagMask);
}

void makeSocketNonBlocking(int socketDescriptor) {
    int flags = fcntl(socketDescriptor, F_GETFL, 0);
    myAssert(flags >= 0);
    flags |= O_NONBLOCK;
    myAssert(fcntl(socketDescriptor, F_SETFL, flags) >= 0);
}



void TcpSocketClient::writeFromEpoll() {
    //TODO check closed
//    db(readyToWrite());
    if (readyToWrite() == 0)
        setWrite(0);
    else {
        int len = 1;
        for (; len > 0;) {
            vector<char> temporaryBuffer;
            for (int i = 0; i < min(TEMP_SIZE1, (int) buffer.size()); i++)
                temporaryBuffer.push_back(buffer[i]);
            len = (int) send(socketDescriptor, (void *) temporaryBuffer.data(), temporaryBuffer.size(), 0);
//            db(len);
            myAssert(len >= 0);
            for (int i = 0; i < len; i++)
                buffer.pop_front();
        }
    }
}


