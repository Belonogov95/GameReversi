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
#include "MyClient.h"


MyClient::MyClient(int port) : port(port), epollDescriptor(-1), flagMask(0) {
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address;
    memset(&address, 0, sizeof(address));

    address.sin_family = AF_INET;
    address.sin_port = htons((uint16_t) port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    assert(bind(socketDescriptor, (const sockaddr *) &address, sizeof(address)) == 0);
    assert(listen(socketDescriptor, BACK_LOG) == 0);
    makeSocketNonBlocking();
}

MyClient::MyClient(int port, int socketDescriptor,
                   int epollDescriptor, MyEpoll * myEpoll) : port(port),
                                                                       socketDescriptor(socketDescriptor),
                                                                       epollDescriptor(epollDescriptor),
                                                                       flagMask(0),
                                                                       bufferCursor(0),
                                                                       closed(0),
                                                                       myEpoll(myEpoll) {
    makeSocketNonBlocking();
}


int MyClient::read(vector<char> &buffer) {
    cerr << "b\n";
    int readLen = (int) recv(socketDescriptor, &buffer[0], buffer.size(), 0);
    cerr << "a\n";
    assert(readLen != -1 || errno == 11);
    return readLen;
}


void MyClient::write(vector<char> data) {
    int oldLen = (int) buffer.size();
    buffer.resize(buffer.size() + data.size());
    for (int i = 0; i < (int) data.size(); i++)
        buffer[oldLen + i] = data[i];
    if (bufferCursor * 2 > (int) buffer.size()) {
        for (int i = bufferCursor; i < (int) buffer.size(); i++)
            buffer[i - bufferCursor] = buffer[i];
        buffer.resize(buffer.size() - bufferCursor);
        bufferCursor = 0;
    }
    setWrite(1);
    myEpoll->write(this);
}

int MyClient::readyToWrite() {
    assert(bufferCursor <= (int) buffer.size());
    return (int) buffer.size() - bufferCursor;
}

void MyClient::setRead(int flag) {
    assert(flag == 0 || flag == 1);
    flagMask |= EPOLLIN;
    if (flag == 0) {
        flagMask ^= EPOLLIN;
    }
    epoll_event event;
    event.data.fd = socketDescriptor;
    event.events = flagMask;
    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, socketDescriptor, &event) == 0);
}

void MyClient::setWrite(int flag) {
    assert(flag == 0 || flag == 1);
    //db(flagMask);
    flagMask |= EPOLLOUT;
    if (flag == 0)
        flagMask ^= EPOLLOUT;
    epoll_event event;
    event.data.fd = socketDescriptor;
    event.events = flagMask;
    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, socketDescriptor, &event) == 0);
}


void MyClient::closeClient() {
    cerr << "close client\n";
    assert(!closed);
    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_DEL, socketDescriptor, 0) == 0);
    assert(myEpoll->onReceiveMap.count(socketDescriptor) == 1);
    myEpoll->onReceiveMap.erase(socketDescriptor);
    assert(myEpoll->onAcceptMap.count(socketDescriptor) == 1);
    myEpoll->onAcceptMap.erase(socketDescriptor);
    assert(myEpoll->clientFromDescriptor.count(socketDescriptor) == 1);
    myEpoll->clientFromDescriptor.erase(socketDescriptor);

    closed = true;
    assert(close(socketDescriptor) == 0);
}


//void MyClient::makeSocketNonBlocking() {
//    int flags = fcntl(socketDescriptor, F_GETFL, 0);
//    assert(flags >= 0);
//    flags |= O_NONBLOCK;
//    assert(fcntl(socketDescriptor, F_SETFL, flags) >= 0);
//}
//
//int MyClient::getSocketDescriptor() {
//    return socketDescriptor;
//}
//
//int MyClient::getPort() {
//    return port;
//}



void MyClient::makeSocketNonBlocking() {
    int flags = fcntl(socketDescriptor, F_GETFL, 0);
    assert(flags >= 0);
    flags |= O_NONBLOCK;
    assert(fcntl(socketDescriptor, F_SETFL, flags) >= 0);
}

int MyClient::getSocketDescriptor() {
    return socketDescriptor;
}

int MyClient::getPort() {
    return port;
}

void MyClient::write(string s) {
    vector<char> buf;
    for (auto x: s)
        buf.push_back(x);
    write(buf);
}


