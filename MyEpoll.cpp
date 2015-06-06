//
// Created by vanya on 02.06.15.
//

#include <assert.h>
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include "MyEpoll.h"



MyEpoll::MyEpoll() {
    epollDescriptor = epoll_create(1);
    //db(epollDescriptor);
    running = true;
}

MyEpoll::~MyEpoll() {
    assert(close(epollDescriptor) == 0);
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
    for (; running;) {
        epoll_event events[MAX_EVENTS];
        cerr << "before wait\n";
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
//        cerr << "after wait\n";
        db(eventsSize);
        for (int i = 0; i < eventsSize; i++) {
            int socketDescriptor = events[i].data.fd;
            db(socketDescriptor);
            if (socketDescriptorType[socketDescriptor] == WAITING_ACCEPT) {
                db("new accept");
                assert(events[i].events & EPOLLIN);
                sockaddr_storage sockAddrStorage;
                socklen_t sockLen = sizeof(sockAddrStorage);
                int newSocketDescriptor = accept(socketDescriptor, (sockaddr *) &sockAddrStorage, &sockLen);

                int port = portFromDescriptor[socketDescriptor];
                db(newSocketDescriptor);
                shared_ptr < MyClient > newClient(new MyClient(MySocket(port, newSocketDescriptor), epollDescriptor));
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

                //epollEvent.events = EPOLLIN;

                //assert(epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, newSocketDescriptor, &epollEvent) == 0);
                //epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, newSocketDescriptor, &epollEvent);
                //db(errno);
                //cerr << strerror(errno) << endl;
                //assert(false);


                //assert(epoll_ctl(epollDescriptor, EPOLL_CTL_DEL, newSocketDescriptor, NULL) == 0);

            }
            else if (socketDescriptorType[socketDescriptor] == WAITING_READ_OR_WRITE) {
                cerr << "here\n";
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
//    db(myClient->readyToWrite());
    if (myClient->readyToWrite() == 0)
        myClient->setWrite(0);
    else {
//        void * ptr = &(myClient->buffer[myClient->bufferCursor]);
//        int l = myClient->readyToWrite();
//        db(l);
//        for (int i = 0; i < l; i++)
//            cerr << (int)((char *)ptr)[i] << " ";
//        cerr << endl;

        int len = (int) send(myClient->mySocket.getSocketDescriptor(), (void*)&(myClient->buffer[myClient->bufferCursor]),
                             (size_t) myClient->readyToWrite(), 0);
        //db(len);
        assert(len >= 0);
        myClient->bufferCursor += len;
    }
}



