//
// Created by vanya on 02.06.15.
//

#ifndef HWW2_MYSOCKET_H
#define HWW2_MYSOCKET_H

const int BACK_LOG = 10;

class MySocket {
    int socketDescriptor;
    int port;

public:
    int getSocketDescriptor() const ;
    int getPort() const ;
    void makeSocketNonBlocking();


    MySocket();
    MySocket(int port);
    MySocket(int port, int socketDescriptor);
    MySocket operator = (MySocket other);
};


#endif //HWW2_MYSOCKET_H
