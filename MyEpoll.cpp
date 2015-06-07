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
    epoll_event event;
    event.data.fd = 0;
    event.events = EPOLLIN;
    epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, 0, &event);
    running = true;
}

MyEpoll::~MyEpoll() {
    assert(close(epollDescriptor) == 0);
}

void MyEpoll::add(int port, void (*onAccept)(shared_ptr < MyClient > ), void (*onReceive)(shared_ptr < MyClient > )) {
    MyClient myClient(port);

    epoll_event epollEvent;
    epollEvent.events = EPOLLIN;
    epollEvent.data.fd = myClient.getSocketDescriptor();
    onAcceptMap[myClient.getSocketDescriptor()] = onAccept;
    onReceiveMap[myClient.getSocketDescriptor()] = onReceive;
    socketDescriptorType[myClient.getSocketDescriptor()] = WAITING_ACCEPT;
    portFromDescriptor[myClient.getSocketDescriptor()] = myClient.getPort();

    epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, myClient.getSocketDescriptor(), &epollEvent);

}

void MyEpoll::start() {
    for (; running;) {
        epoll_event events[MAX_EVENTS];
        cerr << "before wait\n";
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
        cerr << "after wait\n";
        db(eventsSize);
        for (int i = 0; i < eventsSize; i++) {
            int socketDescriptor = events[i].data.fd;
            db(socketDescriptor);
            if (socketDescriptor == 0) {
                string s;
                getline(cin, s);
                if (s != "exit") {
                    cerr << "no such comand :" << s << ":\n";
                }
                else {
                    cerr << "success\n";
                }
            }
            else  if (socketDescriptorType[socketDescriptor] == WAITING_ACCEPT) {
//                db("new accept");
                assert(events[i].events & EPOLLIN);
                sockaddr_storage sockAddrStorage;
                socklen_t sockLen = sizeof(sockAddrStorage);
                int newSocketDescriptor = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);

                int port = portFromDescriptor[socketDescriptor];
//                db(newSocketDescriptor);
                shared_ptr < MyClient > newClient(new MyClient(port, newSocketDescriptor, epollDescriptor));
                //newClient.get

                onAcceptMap[newSocketDescriptor] = NULL;
                onReceiveMap[newSocketDescriptor] = onReceiveMap[socketDescriptor];
                socketDescriptorType[newSocketDescriptor] = WAITING_READ_OR_WRITE;
                portFromDescriptor[newSocketDescriptor] = port;
                clientFromDescriptor[newSocketDescriptor] = newClient;

                epoll_event epollEvent;
                epollEvent.events = 0;
                epollEvent.data.fd = newSocketDescriptor;

                assert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, newSocketDescriptor, &epollEvent) == 0);
                onAcceptMap[socketDescriptor](newClient); /// function call

                newClient->setRead(1);

            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_READ_OR_WRITE) {
//                cerr << "here\n";
                shared_ptr < MyClient > myClient = clientFromDescriptor[socketDescriptor];
//                db(myClient.use_count());
                //cerr << "here\n";
                write(myClient);
//                cerr << "here\n";

                onReceiveMap[socketDescriptor](myClient);
//                cerr << "here\n";
            }
            else
                assert(false);

        }
    }

}

void MyEpoll::write(shared_ptr < MyClient > myClient) {
    assert(!myClient->closed);
    if (myClient->readyToWrite() == 0)
        myClient->setWrite(0);
    else {
//        db2(myClient->buffer.size(), myClient->bufferCursor);
        int len = (int) send(myClient->getSocketDescriptor(), (void*)&(myClient->buffer[myClient->bufferCursor]),
                             (size_t) myClient->readyToWrite(), 0);
//        db(errno);
//        cerr << strerror(errno) << endl;
        assert(len >= 0);
        myClient->bufferCursor += len;
//        db2(myClient->buffer.size(), myClient->bufferCursor);
//        db(len);
    }
}



