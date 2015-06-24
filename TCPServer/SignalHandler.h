//
// Created by vanya on 24.06.15.
//

#ifndef HWW2_SIGNALHANDLER_H
#define HWW2_SIGNALHANDLER_H

#include <functional>
#include <unistd.h>
#include "Executor.h"
#include "debug.h"
#include "SmartSocket.h"

struct MyPipe {
    int pipeIn, pipeOut;

    MyPipe() {
        int p[2];
        myAssert(pipe(p) == 0);
        pipeIn = p[0];
        pipeOut = p[1];
    }

    ~MyPipe() {
        close(pipeIn);
        close(pipeOut);
    }
};

struct SignalHandler {
    using action_t = std::function<void ()>;

    SignalHandler(Executor&, action_t);
    ~SignalHandler();

    SignalHandler(SignalHandler const&) = delete;
    SignalHandler& operator=(SignalHandler const&) = delete;

private:
    static void handler(int signum);

private:
    Executor& executor;
    MyPipe pipe;
    SmartSocket smartSocket;
    static int pipeFd;
};


#endif //HWW2_SIGNALHANDLER_H
