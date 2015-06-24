//
// Created by vanya on 21.06.15.
//

#include "Executor.h"
#include "debug.h"
#include <sys/epoll.h>
#include <unistd.h>

using namespace std;

namespace
{
    const int MAX_EVENTS = 1;

    size_t epollWait(int fd, epoll_event (&events)[MAX_EVENTS])
    {
        for (;;)
        {
            int result = epoll_wait(fd, events, MAX_EVENTS, -1);
            if (result < 0)
            {
                int err = errno;
                if (err == EINTR)
                    continue;
                myAssert(false);
                std::abort();
            }
            return (size_t)result;
        }
    }
}

Executor::Executor():running(true) {
    epollDescriptor = epoll_create(1);
    myAssert(epollDescriptor != -1);
}

Executor::~Executor() {
    int r = close(epollDescriptor);
    myAssert(r == 0);
}

void Executor::add(int fd, function < void (uint32_t ) > action, uint32_t flagMask) {
    actionByFD[fd] = action;
    epoll_event epollEvent = {};
    epollEvent.events = flagMask;
    epollEvent.data.fd = fd;
//    db2(epollDescriptor, fd);
    myAssert(epoll_ctl(epollDescriptor, EPOLL_CTL_ADD, fd, &epollEvent) == 0);
}

void Executor::changeFlags(int fd, uint32_t flagMask) {
    epoll_event event = {};
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
    while (running) {
        epoll_event events[MAX_EVENTS];
        size_t eventsReady = epollWait(epollDescriptor, events);
        for (size_t i = 0; i < eventsReady; i++) {
            int fd = events[i].data.fd;
            actionByFD[fd](events[i].events);
        }
    }
}

void Executor::stop() {
    running = false;
}
