//
// Created by vanya on 18.06.15.
//

#include "HttpWorker.h"
#include "MyClient.h"
#include "debug.h"
#include "Message.h"
#include "Tools.h"


void HttpWorker::sendFile(string path, shared_ptr < MyClient > client) {
    path = "site" + path;
    db(path);
    ifstream in(path, ios::binary);
    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg);
    string message(length, 0);
    in.read((char *)message.data(), length);

    db(message);
    string header = "HTTP/1.1 200 OK" + LINE_BREAK;
    header += "Content-Length: " + to_string(message.size()) + LINE_BREAK;
    string res = header + LINE_BREAK + message;
    client->write(res);
}

void HttpWorker::sendString(string message, shared_ptr < MyClient > client) {
    db(message);
    string header = "HTTP/1.1 200 OK" + LINE_BREAK;
    header += "Content-Length: " + to_string(message.size()) + LINE_BREAK;
    string res = header + LINE_BREAK + message;
    client->write(res);
}


bool isEmpty(string s) {
    for (auto ch: s)
        if (!isspace(ch))
            return false;
    return true;
}


pair < int, Message > HttpWorker::readMessage(shared_ptr < MyClient > client) {
    string buffer(BUF_SZ, 0);
    int len = client->read(buffer);

    if (len == 0)  {
        client->closeClient();
        //TODO something with HttpWorker
        return make_pair(0, Message());
    }

    if (len <= 0) return make_pair(-1, Message());
    buffer.resize(len);

    for (auto x: buffer) {
        if (innerBuffer.empty())
            innerBuffer.push_back(string());
        innerBuffer.back().push_back(x);
        if (x == '\n')
            innerBuffer.push_back(string());
    }

    printBuff();

    for (;!innerBuffer.empty() && innerBuffer[0].empty(); innerBuffer.pop_front());

    assert(!innerBuffer.empty());
    auto tmp = tools::split(innerBuffer[0], ' ');
//    db("here");

    Message message;
    message.type = tmp[0];
    assert(message.type == "POST" || message.type == "GET");
    message.URL = tmp[1];
    int emptyLine = -1;
    int textLen = -1;
    //db("here");
    for (int i = 0; i < (int)innerBuffer.size() - 1; i++) {
        auto tmp = tools::split(innerBuffer[i], ':');
        if (tmp[0] == "Content-Length") {
            assert(tmp.size() == 2u);
            textLen = stoi(tmp[1]);
        }
        if (isEmpty(innerBuffer[i])) {
            emptyLine = i;
            break;
        }
    }
//    db(emptyLine);
//    db(innerBuffer.size());
    for (int i = 0; i < (int)innerBuffer.size(); i++)
//        db2(i, innerBuffer[i].size());
    if (emptyLine == -1)  {
        return make_pair(-1, Message());
    }
    int cur = emptyLine + 1;
    if (message.type == "POST") {
        assert(textLen >= 0);
        for (;cur < (int)innerBuffer.size() && textLen > 0; cur++) {
            textLen -= innerBuffer[cur].size();
        }
        if (textLen > 0) {
            return make_pair(-1, Message());
        }
        for (int j = emptyLine + 1; j < cur; j++)
            message.body.push_back(innerBuffer[j]);
    }
    for (int j = 0; j < cur; j++)
        innerBuffer.pop_front();
    db("success finish");

    return make_pair(1, message);
}



void HttpWorker::printBuff() {
    for (auto x: innerBuffer)
        cerr << x;
}





