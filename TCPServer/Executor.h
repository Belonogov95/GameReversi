//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_EXECUTOR_H
#define HWW2_EXECUTOR_H

#include <bits/stdc++.h>
#include <sys/epoll.h>

using namespace std;

//typedef std::function < void (int, u_int32_t) > executorCallBackType;
//typedef unsigned long long ull;


class Executor {
public:
    int epollDescriptor;
    map < int, function < void (u_int32_t) >  > actionByFD;
    bool running;

    ~Executor();

    Executor();

    void add(int fd, function < void (u_int32_t ) > action, u_int32_t flagMask);

    void changeFlags(int fd, u_int32_t flagMask);
    void del(int fd);
    void run();

};


#endif //HWW2_EXECUTOR_H
