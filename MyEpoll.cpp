//
// Created by vanya on 02.06.15.
//

#include <assert.h>
#include <sys/socket.h>
#include <iostream>
#include "MyEpoll.h"



MyEpoll::MyEpoll() {
    epollDescriptor = epoll_create(1);
    closed = false;
}

void MyEpoll::add(MySocket mySocket, void (*onAccept)(shared_ptr < MyClient > ), void (*onReceive)(shared_ptr < MyClient > )) {
    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = mySocket.getSocketDescriptor();
    onAcceptMap[mySocket.getSocketDescriptor()] = onAccept;
    onReceiveMap[mySocket.getSocketDescriptor()] = onReceive;
    socketDescriptorType[mySocket.getSocketDescriptor()] = WAITING_ACCEPT;
    portFromDescriptor[mySocket.getSocketDescriptor()] = mySocket.getPort();


    epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, mySocket.getSocketDescriptor(), &epollEvent);

}

void MyEpoll::start() {
    for (; !closed;) {
        epoll_event events[MAX_EVENTS];
        //cerr << "before wait\n";
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
        //cerr << "after wait\n";
        //db(eventsSize);
        for (int i = 0; i < eventsSize; i++) {
            int socketDescriptor = events[i].data.fd;
            //db(socketDescriptor);
            if (socketDescriptorType[socketDescriptor] == WAITING_ACCEPT) {
                assert(events[i].events & EPOLLIN);
                sockaddr_storage sockAddrStorage;
                socklen_t sockLen = sizeof(sockAddrStorage);
                int newSocketDescriptor = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);

                int port = portFromDescriptor[socketDescriptor];
                shared_ptr < MyClient > newClient(new MyClient(MySocket(port, newSocketDescriptor), epollDescriptor));
                //newClient.get

                onAcceptMap[newSocketDescriptor] = NULL;
                onReceiveMap[newSocketDescriptor] = onReceiveMap[socketDescriptor];
                socketDescriptorType[newSocketDescriptor] = WAITING_READ_OR_WRITE;
                portFromDescriptor[newSocketDescriptor] = port;
                //clientFromDescriptor[newSocketDescriptor] = newClient;

                onAcceptMap[socketDescriptor](newClient); /// function call

                epoll_event epollEvent;
                epollEvent.events = EPOLLIN;
                epollEvent.data.fd = newSocketDescriptor;

                epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, newSocketDescriptor, &epollEvent);

            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_READ_OR_WRITE) {
                shared_ptr < MyClient > myClient = clientFromDescriptor[socketDescriptor];
                write(myClient);

                onReceiveMap[socketDescriptor](myClient);
            }
            else
                assert(false);

        }
    }

}

void MyEpoll::write(shared_ptr < MyClient > myClient) {
    if (myClient->readyToWrite() == 0)
        myClient->setWrite(0);
    else {
        int len = (int) send(myClient->mySocket.getSocketDescriptor(), &(myClient->buffer[myClient->bufferCursor]),
                             (size_t) myClient->readyToWrite(), 0);
        assert(len >= 0);
        myClient->bufferCursor += len;
    }
}



