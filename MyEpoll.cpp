//
// Created by vanya on 02.06.15.
//

#include <assert.h>
#include <sys/socket.h>
#include "MyEpoll.h"

MyEpoll::MyEpoll() {
    epollDescriptor = epoll_create(1);
    closed = false;
}

void MyEpoll::add(MySocket mySocket, void (*onAccept)(MyClient), void (*onReceive)(MyClient)) {
    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = mySocket.getSocketDescriptor();
    onAcceptMap[mySocket.getSocketDescriptor()] = onAccept;
    onReceiveMap[mySocket.getSocketDescriptor()] = onReceive;
    socketDescriptorType[mySocket.getSocketDescriptor()] = WAITING_ACCEPT;
    epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, mySocket.getSocketDescriptor(), &epollEvent);

}

void MyEpoll::start() {
    for (; !closed;) {
        epoll_event events[MAX_EVENTS];
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
        for (int i = 0; i < eventsSize; i++) {
            int socketDescriptor = events[i].data.fd;
            if (socketDescriptorType[socketDescriptor] == WAITING_ACCEPT) {
                assert(events[i].events & EPOLLIN);
                sockaddr_storage sockAddrStorage;
                socklen_t sockLen = sizeof(sockAddrStorage);
                int newSocketDescriptor = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);

                MyClient newClient();


                onAcceptMap[newSocketDescriptor] = onAcceptMap[socketDescriptor];
                onReceiveMap[newSocketDescriptor] = onReceiveMap[socketDescriptor];
            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_READ_OR_WRITE) {


            }
            else
                assert(false);

        }
    }

}




