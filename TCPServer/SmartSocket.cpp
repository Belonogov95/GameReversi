//
// Created by vanya on 21.06.15.
//

#include "SmartSocket.h"


SmartSocket::SmartSocket(int descriptor, function<void(int, u_int32_t)> callBack,
                         u_int32_t flagMask, Executor * executor):executor(executor) {
    executor->add(descriptor, [=](u_int32_t mask) { callBack(descriptor, mask); }, flagMask);
}

SmartSocket::~SmartSocket() {
    executor->del(descriptor);
}
