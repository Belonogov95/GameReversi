//
// Created by vanya on 02.06.15.
//

#ifndef HWW2_MYCLIENT_H
#define HWW2_MYCLIENT_H


#include <array>
#include <vector>
#include "MySocket.h"
#include <iostream>

using namespace std;

#define db(x) cerr << #x << " = " << x << endl

class MyClient {
private:

    int epollDescriptor;
    u_int32_t flagMask;

public:
    MySocket mySocket;
    vector < char > buffer;
    int bufferCursor;
    bool closed;
    MyClient();
    MyClient(MySocket mySocket, int epollDescriptor);
    MyClient(const MyClient & myClient);


    void setRead(int flag);

    void setWrite(int flag);

    int write(vector<char> data);

    int readyToWrite();

    int read(vector<char> &buffer);


    void closeClient();
};


#endif //HWW2_MYCLIENT_H
