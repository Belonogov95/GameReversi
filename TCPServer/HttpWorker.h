//
// Created by vanya on 18.06.15.
//
#include <bits/stdc++.h>
#include "MyClient.h"
#include "Message.h"

#ifndef HWW2_HTTPWORKER_H
#define HWW2_HTTPWORKER_H

using namespace std;
const string LINE_BREAK = "\r\n";
const int BUF_SZ = 10000;

class HttpWorker {

private:
    deque < string > innerBuffer;

public:

    vector<string> split(string s, char ch);

    string trim(string s);

    void printBuff();

    void sendFile(string path, shared_ptr<MyClient> client);

    pair<bool, Message > readMessage(shared_ptr<MyClient> client);

};


#endif //HWW2_HTTPWORKER_H
