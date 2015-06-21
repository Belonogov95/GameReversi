//
// Created by vanya on 02.06.15.
//

#include <assert.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "MyEpoll.h"
#include "debug.h"


MyEpoll::MyEpoll() {
    epollDescriptor = epoll_create(1);
    int pipefd[2];
    pipe(pipefd);
    pipeFD = pipefd[0];
    pipeOut = pipefd[1];
    epoll_event epollEvent;
    memset(&epollEvent, 0, sizeof(epollEvent));
    epollEvent.events = EPOLLIN | EPOLLOUT;
    epollEvent.data.fd = pipeFD;

    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, pipeFD, &epollEvent) == 0);
}

MyEpoll::~MyEpoll() {
//    assert(close(epollDescriptor) == 0);
}

void MyEpoll::add(int port, string ipAddress, void (*onReceive)(shared_ptr<MyClient>)) {
    addrinfo hints;
    addrinfo *result;
    int socketDescriptor;

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
    makeSocketNonBlocking(socketDescriptor);

    epoll_event epollEvent;
    memset(&epollEvent, 0, sizeof(epollEvent));
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = socketDescriptor;
    onReceiveMap[socketDescriptor] = onReceive;
    socketDescriptorType[socketDescriptor] = WAITING_ACCEPT;
    portFromDescriptor[socketDescriptor] = port;

    epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, socketDescriptor, &epollEvent);
}


void MyEpoll::start() {
    bool running = true;
    for (; running;) {
        epoll_event events[MAX_EVENTS];
//        db("before wait");
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
//        db("after wait");
        for (int i = 0; i < eventsSize; i++) {
            int socketDescriptor = events[i].data.fd;
//            db(socketDescriptor);
            if (socketDescriptor == pipeFD)  {
//                cerr << "here\n";
                for (auto x: clientFromDescriptor)
                    x.second->closeClient();
                running = false;
                assert(close(epollDescriptor) == 0);
                //TODO
            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_ACCEPT) {
                assert(events[i].events & EPOLLIN);
                sockaddr_storage sockAddrStorage;
                socklen_t sockLen = sizeof(sockAddrStorage);
                int newSocketDescriptor = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);
                db(newSocketDescriptor);

                int port = portFromDescriptor[socketDescriptor];

                shared_ptr<MyClient> newClient( new MyClient(port, newSocketDescriptor, epollDescriptor, this));

                onReceiveMap[newSocketDescriptor] = onReceiveMap[socketDescriptor];
                socketDescriptorType[newSocketDescriptor] = WAITING_READ_OR_WRITE;
                portFromDescriptor[newSocketDescriptor] = port;
                clientFromDescriptor[newSocketDescriptor] = newClient;

                epoll_event epollEvent;
                epollEvent.events = 0;
                epollEvent.data.fd = newSocketDescriptor;

                assert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, newSocketDescriptor, &epollEvent) == 0);
                newClient->setRead(1);

            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_READ_OR_WRITE) {
                shared_ptr<MyClient> myClient = clientFromDescriptor[socketDescriptor];
                write(myClient.get());
                onReceiveMap[socketDescriptor](myClient);
            }
            else
                assert(false);

        }
    }
}

int MyEpoll::getPipe() {
    return pipeOut;
}

void MyEpoll::write(MyClient * myClient) {
    assert(!myClient->closed);
    if (myClient->readyToWrite() == 0)
        myClient->setWrite(0);
    else {
        int len = (int) send(myClient->getSocketDescriptor(), (void *) &(myClient->buffer[myClient->bufferCursor]),
                             (size_t) myClient->readyToWrite(), 0);
        assert(len >= 0);
        myClient->bufferCursor += len;
    }
}



