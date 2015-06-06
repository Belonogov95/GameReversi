//
// Created by vanya on 02.06.15.
//

#include <sys/socket.h>
#include <assert.h>
#include <sys/epoll.h>
#include <string.h>
#include <unistd.h>
#include "MyClient.h"

MyClient::MyClient(MySocket mySocket, int epollDescriptor):mySocket(mySocket), epollDescriptor(epollDescriptor), flagMask(0),
                                                           bufferCursor(0), closed(0) {  }

MyClient::MyClient(const MyClient & myClient): mySocket(myClient.mySocket), epollDescriptor(myClient.epollDescriptor),
                        flagMask(myClient.flagMask), bufferCursor(myClient.bufferCursor), closed(myClient.closed){ }

MyClient::MyClient() { }



int MyClient::read(vector < char > & buffer) {
    int readLen = (int) recv(mySocket.getSocketDescriptor(), &buffer[0], buffer.size(), 0);
    //db(errno);
    //db(strerror(errno));
    assert(readLen != -1 || errno == 11);
    return readLen;
}

int MyClient::write(vector < char > data) {
    int oldLen = (int) buffer.size();
    buffer.resize(buffer.size() + data.size());
    for (int i = 0; i < (int)data.size(); i++)
        buffer[oldLen + i] = data[i];
    if (bufferCursor * 2 > (int)buffer.size()) {
        for (int i = bufferCursor; i < (int)buffer.size(); i++)
            buffer[i - bufferCursor] = buffer[i];
    }
    buffer.resize(buffer.size() - bufferCursor);
    bufferCursor = 0;

    setWrite(1);
}

int MyClient::readyToWrite() {
    assert(bufferCursor <= (int)buffer.size());
    return (int)buffer.size() - bufferCursor;
}

void MyClient ::setRead(int flag) {
    assert(flag == 0 || flag == 1);
    flagMask |= EPOLLIN;
    if (flag == 0) {
        flagMask ^= EPOLLIN;
    }
    epoll_event event;
    event.data.fd = mySocket.getSocketDescriptor();
    event.events = flagMask;
    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, mySocket.getSocketDescriptor(), &event) == 0);
}

void MyClient :: setWrite(int flag) {
    assert(flag == 0 || flag == 1);
    db(flagMask);
    flagMask |= EPOLLOUT;
    if (flag == 0)
        flagMask ^= EPOLLOUT;
    epoll_event event;
    event.data.fd = mySocket.getSocketDescriptor();
    event.events = flagMask;
    //db(flagMask);
    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, mySocket.getSocketDescriptor(), &event) == 0);
//    db(epollDescriptor);
//    db(mySocket.getSocketDescriptor());
//    (epoll_ctl(epollDescriptor, EPOLL_CTL_DEL, mySocket.getSocketDescriptor(), NULL) == 0);
//    db(errno);
//    cerr << strerror(errno) << endl;
//    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, mySocket.getSocketDescriptor(), &event) == 0);
//    //epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, mySocket.getSocketDescriptor(), &event);
}


void MyClient:: closeClient() {
    if(closed) return;
    epoll_ctl(epollDescriptor, EPOLL_CTL_DEL, mySocket.getSocketDescriptor(), 0);
    closed = true;
    assert(close(mySocket.getSocketDescriptor()) == 0);
}




