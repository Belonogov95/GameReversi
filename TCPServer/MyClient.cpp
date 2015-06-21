//
// Created by vanya on 02.06.15.
//

#include <sys/socket.h>
#include <assert.h>
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include "MyClient.h"
#include "debug.h"


MyClient::MyClient(int socketDescriptor, MyEpoll *myEpoll) :
        socketDescriptor( socketDescriptor),
        flagMask(0),
        closed(0),
        myEpoll(myEpoll) {
    makeSocketNonBlocking(socketDescriptor);
}



MyClient::~MyClient() {
    epoll_ctl(myEpoll->epollDescriptor, EPOLL_CTL_DEL, socketDescriptor, 0);
    myEpoll->onReceiveMap.count(socketDescriptor);
    myEpoll->onReceiveMap.erase(socketDescriptor);
    myEpoll->socketDescriptorType.erase(socketDescriptor);
    close(socketDescriptor);
}


int MyClient::read(string &buffer) {
    int readLen = (int) recv(socketDescriptor, &buffer[0], buffer.size(), 0);
    assertMy(readLen != -1 || errno == 11);
    return readLen;
}

void MyClient::write(string data) {

    for (auto x: data)
        buffer.push_back(x);

    setWrite(1);
    writeFromEpoll();
}


int MyClient::readyToWrite() {
    return buffer.size();
}

void MyClient::setRead(int flag) {
    assertMy(flag == 0 || flag == 1);
    flagMask |= EPOLLIN;
    if (flag == 0) {
        flagMask ^= EPOLLIN;
    }
    epoll_event event;
    event.data.fd = socketDescriptor;
    event.events = flagMask;
    assertMy(epoll_ctl(myEpoll->epollDescriptor, EPOLL_CTL_MOD, socketDescriptor, &event) == 0);
}

void MyClient::setWrite(int flag) {
    assertMy(flag == 0 || flag == 1);
    flagMask |= EPOLLOUT;
    if (flag == 0)
        flagMask ^= EPOLLOUT;
    epoll_event event;
    event.data.fd = socketDescriptor;
    event.events = flagMask;
    assertMy(epoll_ctl(myEpoll->epollDescriptor, EPOLL_CTL_MOD, socketDescriptor, &event) == 0);
}


void MyClient::closeClient() {
    db2("close client", socketDescriptor);
    assertMy(!closed);

    assertMy(epoll_ctl(myEpoll->epollDescriptor, EPOLL_CTL_DEL, socketDescriptor, 0) == 0);

    assertMy(myEpoll->clientFromDescriptor.count(socketDescriptor) == 1);
    assertMy(myEpoll->onReceiveMap.count(socketDescriptor) == 1);
    myEpoll->clientFromDescriptor.erase(socketDescriptor);
    myEpoll->onReceiveMap.erase(socketDescriptor);

    myEpoll->socketDescriptorType.erase(socketDescriptor);
    closed = true;

    assertMy(close(socketDescriptor) == 0);
}


void makeSocketNonBlocking(int socketDescriptor) {
    int flags = fcntl(socketDescriptor, F_GETFL, 0);
    assertMy(flags >= 0);
    flags |= O_NONBLOCK;
    assertMy(fcntl(socketDescriptor, F_SETFL, flags) >= 0);
}

int MyClient::getSocketDescriptor() {
    return socketDescriptor;
}


void MyClient::writeFromEpoll() {
    assertMy(!closed);
    if (readyToWrite() == 0)
        setWrite(0);
    else {
        int len = 1;
        for (; len > 0;) {
            vector<char> temporaryBuffer;
            for (int i = 0; i < min(TEMP_SIZE, (int) buffer.size()); i++)
                temporaryBuffer.push_back(buffer[i]);
            len = (int) send(getSocketDescriptor(), (void *) temporaryBuffer.data(), temporaryBuffer.size(), 0);
            assertMy(len >= 0);
            for (int i = 0; i < len; i++)
                buffer.pop_front();
        }
    }
}

