//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_SMARTSOCKET_H
#define HWW2_SMARTSOCKET_H

#include <functional>
#include "Executor.h"

class SmartSocket {
public:

    SmartSocket(int descriptor, std::function<void(int, u_int32_t)> callBack, u_int32_t flagMask, Executor *executor);
    ~SmartSocket();

    int descriptor;
    Executor * executor;

};


#endif //HWW2_SMARTSOCKET_H
