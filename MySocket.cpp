//
// Created by vanya on 02.06.15.
//

#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include "MySocket.h"
#include <bits/stdc++.h>

using namespace std;

MySocket::MySocket(int port) : port(port) {
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


MySocket::MySocket(int port, int socketDescriptor) : port(port), socketDescriptor(socketDescriptor) { }

MySocket::MySocket(): port(-1), socketDescriptor(-1) { }

int MySocket::getPort() const {
    return port;
}

int MySocket::getSocketDescriptor() const {
    return socketDescriptor;
}

void MySocket::makeSocketNonBlocking() {
    int flags = fcntl(socketDescriptor, F_GETFL, 0);
    assert(flags >= 0);
    flags |= O_NONBLOCK;
    assert(fcntl(socketDescriptor, F_SETFL, flags) >= 0);
}


MySocket MySocket::operator=(MySocket other) {
    swap(*this, other);
    return *this;
}


