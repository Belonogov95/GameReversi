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


MyClient::MyClient(int port, string ipAddress) : port(port), epollDescriptor(-1), flagMask(0) {
    addrinfo hints;
    addrinfo *result;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */

    assert(getaddrinfo(ipAddress.data(), to_string(port).data(), &hints, &result) == 0);

    assert(result != NULL);

    socketDescriptor = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    assert(socketDescriptor != -1);

    int one = 1;
    assert(setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) == 0);

    if (bind(socketDescriptor, result->ai_addr, result->ai_addrlen) != 0) {
        perror("");
        exit(0);
    }
    assert(listen(socketDescriptor, BACK_LOG) == 0);
    freeaddrinfo(result);           /* No longer needed */
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


int MyClient::read(string &buffer) {
    int readLen = (int) recv(socketDescriptor, &buffer[0], buffer.size(), 0);
    assert(readLen != -1 || errno == 11);
    return readLen;
}


void MyClient::write(string data) {
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

    assert(myEpoll->clientFromDescriptor.count(socketDescriptor) == 1);
    myEpoll->clientFromDescriptor.erase(socketDescriptor);
    myEpoll->portFromDescriptor.erase(socketDescriptor);
    myEpoll->socketDescriptorType.erase(socketDescriptor);
    closed = true;
    assert(close(socketDescriptor) == 0);
}


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

