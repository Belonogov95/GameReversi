//
// Created by vanya on 02.06.15.
//

#include <assert.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "MyEpoll.h"
#include "debug.h"


MyEpoll::MyEpoll() {
    epollDescriptor = epoll_create(1);
    pipeFD = -1;
}

MyEpoll::~MyEpoll() {
    assert(close(epollDescriptor) == 0);
}

void MyEpoll::add(int port, string ipAddress, void (*onReceive)(shared_ptr<MyClient>)) {
    MyClient myClient(port, ipAddress);

    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = myClient.getSocketDescriptor();
    onReceiveMap[myClient.getSocketDescriptor()] = onReceive;
    socketDescriptorType[myClient.getSocketDescriptor()] = WAITING_ACCEPT;
    portFromDescriptor[myClient.getSocketDescriptor()] = myClient.getPort();

    epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, myClient.getSocketDescriptor(), &epollEvent);
}


void MyEpoll::start() {
    bool running = true;
    for (; running;) {
        epoll_event events[MAX_EVENTS];
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
        for (int i = 0; i < eventsSize; i++) {
            int socketDescriptor = events[i].data.fd;
            if (socketDescriptor == pipeFD)  {
                cerr << "here\n";
                for (auto x: clientFromDescriptor)
                    x.second->closeClient();
                //portFromDescriptor.clear();
                running = false;
                //db("close epoll");
                //int res = close(epollDescriptor);
                //assert(close(epollDescriptor) == 0);
                //TODO
            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_ACCEPT) {
                assert(events[i].events & EPOLLIN);
                sockaddr_storage sockAddrStorage;
                socklen_t sockLen = sizeof(sockAddrStorage);
                int newSocketDescriptor = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);

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
    assert(pipeFD == -1);
    int pipefd[2];
    pipe(pipefd);
    pipeFD = pipefd[0];
    epoll_event epollEvent;
    epollEvent.events = EPOLLIN | EPOLLOUT;
    epollEvent.data.fd = pipeFD;
    assert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, pipeFD, &epollEvent) == 0);
    cerr << "add\n";
    return pipefd[1];
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



