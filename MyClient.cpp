//
// Created by vanya on 02.06.15.
//

#include <sys/socket.h>
#include <assert.h>
#include <sys/epoll.h>
#include "MyClient.h"

MyClient::MyClient(MySocket mySocket, int epollDescriptor):mySocket(mySocket), epollDescriptor(epollDescriptor), flagMask(0),  bufferCursor(0) {  }

MyClient::MyClient(const MyClient & myClient): mySocket(myClient.mySocket), epollDescriptor(myClient.epollDescriptor),
                        flagMask(myClient.flagMask), bufferCursor(myClient.bufferCursor){ }

MyClient::MyClient() { }



int MyClient::read(vector < char > & buffer) {
    int readLen = (int) recv(mySocket.getSocketDescriptor(), &buffer[0], buffer.size(), 0);
    assert(readLen != -1);
    return readLen;
}

int MyClient::write(vector < char > data) {
    buffer.resize(buffer.size() + data.size());
    buffer[0] = 1;
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
    epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, mySocket.getSocketDescriptor(), &event);
}

void MyClient :: setWrite(int flag) {
    assert(flag == 0 || flag == 1);
    flagMask |= EPOLLOUT;
    if (flag == 0)
        flagMask ^= EPOLLOUT;
    epoll_event event;
    event.data.fd = mySocket.getSocketDescriptor();
    event.events = flagMask;
    epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, mySocket.getSocketDescriptor(), &event);
}





