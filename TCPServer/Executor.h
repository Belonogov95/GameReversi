//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_EXECUTOR_H
#define HWW2_EXECUTOR_H

#include <bits/stdc++.h>
#include <sys/epoll.h>


using namespace std;

const int MAX_EVENTS = 1;

typedef u_int32_t uint;


class Executor {
public:
    int epollDescriptor;
    map < int, function < void (uint) >  > actionByFD;
    bool running;

    ~Executor();

    Executor();

    void add(int fd, function < void (uint) > action, uint flagMask);

    void changeFlags(int fd, uint flagMask);
    void del(int fd);
    void run();

};


#endif //HWW2_EXECUTOR_H
