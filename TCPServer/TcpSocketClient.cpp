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


namespace {
    int acceptSocket(int socketDescriptor) {
        sockaddr_storage sockAddrStorage;
        socklen_t sockLen = sizeof(sockAddrStorage);
        int r = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);
        myCheck(r != -1);
        return r;
    }


};


TcpSocketClient::TcpSocketClient(int serverDescriptor, Executor *executor, OnReceive onReceive)
        : socketDescriptor(acceptSocket(serverDescriptor))
        , smartSocket(socketDescriptor.get(), [onReceive, this](int fd, uint32_t flags) {
            if (flags & EPOLLIN)
                onReceive(fd, EPOLLIN);
            if (flags & EPOLLOUT)
                writeFromEpoll();
        }, EPOLLIN, executor)
        , executor(executor)
        , flagMask(EPOLLIN) {
    makeSocketNonBlocking(socketDescriptor.get());
}

//executor->add(newSocketDescriptor, [=](u_int32_t mask) { onReceive(newSocketDescriptor, mask); }, EPOLLIN);


int TcpSocketClient::read(string &buffer) {
    int readLen = (int) recv(socketDescriptor.get(), &buffer[0], buffer.size(), 0);
    myCheck(readLen != -1 || errno == 11);
    return readLen;
}

void TcpSocketClient::write(const string & data) {
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
    executor->changeFlags(socketDescriptor.get(), flagMask);
}

void makeSocketNonBlocking(int socketDescriptor) {
    int flags = fcntl(socketDescriptor, F_GETFL, 0);
    myCheck(flags >= 0);
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
            len = (int) send(socketDescriptor.get(), (void *) temporaryBuffer.data(), temporaryBuffer.size(), 0);

            myCheck(len >= 0);
            for (int i = 0; i < len; i++)
                buffer.pop_front();
        }
    }
}


