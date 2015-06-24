//
// Created by vanya on 21.06.15.
//

#include "SmartSocket.h"
#include "debug.h"

SmartSocket::SmartSocket(int descriptor, function<void(int, u_int32_t)> callBack, u_int32_t flagMask, Executor * executor)
        : descriptor(descriptor)
        , executor(executor) {

    executor->add(descriptor, [=](u_int32_t mask) { callBack(descriptor, mask); }, flagMask);
}

SmartSocket::~SmartSocket() {
//    myCheck(descriptor >= 1);
    executor->del(descriptor);
}
