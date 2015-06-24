//
// Created by vanya on 21.06.15.
//

#include "Executor.h"
#include "debug.h"
#include <bits/stdc++.h>
#include <sys/epoll.h>
#include <unistd.h>

using namespace std;


Executor::Executor():running(1) {
    epollDescriptor = epoll_create(1);
    myAssert(epollDescriptor != -1);
}

Executor::~Executor() {
    int r = close(epollDescriptor);
    myAssert(r == 0);
}

void Executor::add(int fd, function < void (u_int32_t ) > action, u_int32_t flagMask) {
    actionByFD[fd] = action;
    epoll_event epollEvent;
    memset(&epollEvent, 0, sizeof(epollEvent));
    epollEvent.events = flagMask;
    epollEvent.data.fd = fd;
//    db2(epollDescriptor, fd);
    myAssert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, fd, &epollEvent) == 0);
}

void Executor::changeFlags(int fd, u_int32_t flagMask) {
    epoll_event event;
    memset(&event, 0, sizeof(event));
    event.data.fd = fd;
    event.events = flagMask;
    myAssert(epoll_ctl(epollDescriptor, EPOLL_CTL_MOD, fd, &event) == 0);
}

void Executor::del (int fd) {
    db2(epollDescriptor, fd);
    int r = epoll_ctl(epollDescriptor, EPOLL_CTL_DEL, fd, 0);
    if (r != 0) {
        perror("");
        assert(false);
    }

    actionByFD.erase(fd);
}

void Executor::run() {
    for (; running;) {
        epoll_event events[MAX_EVENTS];
        int eventsSize = epoll_wait(epollDescriptor, events, MAX_EVENTS, -1);
        for (int i = 0; i < eventsSize; i++) {
            int fd = events[i].data.fd;
            actionByFD[fd](events[i].events);
        }
    }
}





