//
// Created by vanya on 02.06.15.
//

#ifndef HWW2_MYSOCKET_H
#define HWW2_MYSOCKET_H


class MySocket {
    int socketDescriptor;
    int port;

public:
    int getSocketDescriptor() const {
        return socketDescriptor;
    }

    int getPort() const {
        return port;
    }

    MySocket(int port);
    MySocket(int port, int socketDescriptor);
};


#endif //HWW2_MYSOCKET_H
