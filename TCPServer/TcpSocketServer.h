//
// Created by vanya on 21.06.15.
//

#ifndef HWW2_TCPSOCKET_H
#define HWW2_TCPSOCKET_H
#include <bits/stdc++.h>
#include "Executor.h"
#include "TcpSocketClient.h"
#include "SmartSocket.h"
#include "FileDescriptor.h"

using namespace std;



class TcpSocketServer {
public:
    Executor * executor;
    FileDescriptor socketDescriptor;
    SmartSocket smartSocket;

    TcpSocketServer(int port, string ipAddress, function<void(shared_ptr < TcpSocketClient >)> onAccept,function<void(int, u_int32_t )> onReceive, Executor *executor);
};


#endif //HWW2_TCPSOCKET_H
