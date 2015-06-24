//
// Created by vanya on 24.06.15.
//

#include <sys/epoll.h>
#include <signal.h>
#include "SignalHandler.h"

SignalHandler::SignalHandler(Executor &executor, SignalHandler::action_t action)
    : executor(executor)
    , smartSocket(pipe.pipeIn, [action] (int, uint32_t) { action(); }, EPOLLIN, &executor)
{
    myAssert(pipeFd == -1);
    pipeFd = pipe.pipeOut;
    struct sigaction sa = {};
    sa.sa_handler = handler;
    sigaction(SIGINT, &sa, NULL);
}

SignalHandler::~SignalHandler() {
    // TODO: unsubscribe
    myAssert(pipeFd == pipe.pipeOut);
    pipeFd = -1;
}

void SignalHandler::handler(int signum) {
    char buffer[1] = {0};
    myAssert(write(pipeFd, buffer, sizeof(buffer)) != -1);
}

int SignalHandler::pipeFd = -1;