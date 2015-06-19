#include <iostream>
#include <assert.h>
#include <thread>
#include <fstream>
#include "MyEpoll.h"
#include "debug.h"
#include "HttpWorker.h"
#include "MyClient.h"

using namespace std;


map <int, shared_ptr<HttpWorker>> workers;
map <int, string> names;
set <string> myFiles;
map < int, set < int > > edges;

void onAC2(shared_ptr<MyClient> client) {
    assert(workers.count(client->getSocketDescriptor()) == 0);
    workers[client->getSocketDescriptor()] = shared_ptr<HttpWorker>(new HttpWorker());
}


string arrayJSFromStrings(vector < string > data) {
    string res = "[" ;
    for (int i = 0; i < (int)data.size(); i++) {
        res += "\"" + data[i] + "\"";
        if (i + 1 != (int)data.size())
            res += ", ";
    }
    res += "]";
    return res;
}


string generatePlayersList(shared_ptr < MyClient > client) {
    int descriptor = client->getSocketDescriptor();
    vector < string > data;
    for (auto x: names)
        if (x.first != descriptor)
            data.push_back(x.second);
    string s0 = arrayJSFromStrings(data);
    vector < string > dataF;
    for (auto x: edges[descriptor])
        dataF.push_back(names[x]);
    string s1 = arrayJSFromStrings(dataF);
    vector < string > tmp;
    return "[" + s0 + ", " + s1 + "]";
}

void onRC2(shared_ptr<MyClient> client) {
    int descriptor = client->getSocketDescriptor();
    assert(workers.count(descriptor) != 0);
    shared_ptr<HttpWorker> worker = workers[descriptor];
    while (true) {
        auto prAddress = worker->readMessage(client);
        auto message = prAddress.second;
        if (prAddress.first == 0) {
            workers.erase(client->getSocketDescriptor());
            return;
        }
        if (prAddress.first == -1)
            return;

        if (message.URL == "/login") {
            assert((int) message.body.size() == 1);
            string name = message.body[0];
            bool exist = 0;
            for (auto x: names)
                if (x.second == name)
                    exist = 1;
            if (exist) {
                worker->sendString("unsuccess", client);
            }
            else {
                names[descriptor] = name;
                worker->sendString("success", client);
            }
        }
        else if (message.URL == "/players") {
            string tmp = generatePlayersList(client);
            db(tmp);
            worker->sendString(tmp, client);
//            worker->sendString(generatePlayersList(client), client);
        }
        else if (myFiles.count(message.URL) == 1) {
            worker->sendFile(prAddress.second.URL, client);
        }
        else {
            db(message.URL);
            assert(false);
        }
    }

}

void test2() {
    myFiles.insert("/index.html");
    myFiles.insert("/favicon.ico");
    myFiles.insert("/cat.jpg");
    myFiles.insert("/script.js");
    myFiles.insert("/mystyle.css");

    MyEpoll ep;
    ep.add(7773, onAC2, onRC2);
    ep.start();
}

int main() {
    //test();
    //ifstream in("../site/index.html");
    //string s;
    //while (getline(in, s))
    //cerr << s << endl;


    test2();
    //cerr << char(69) << endl;
//    int sd = socket(AF_INET, SOCK_STREAM, 0);
//    db(sd);
//    sockaddr_in address;
//    memset(&address, 0, sizeof(address));
//    address.sin_family = AF_INET;
//    address.sin_port = htons(39005);
//    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
//
//    assert(bind(sd, (const sockaddr *) &address, sizeof(address)) == 0);
//    int res = listen(sd, 5);
//    db(res);
//    assert(res == 0);
//    sockaddr_storage outAddr;
//    socklen_t len = sizeof outAddr;
//
//    int new_sd = accept(sd, (sockaddr*)&outAddr, &len);
//    //char *s = (char *) string("8888").c_str();
//    //int bytes_sent = (int) send(new_sd, s, strlen(s), 0);
//
//    int L = 10;
//    char buff[L];
//    for (int i = 0; i < L; i++)
//        buff[i] = '_';
//    db("before recv");
//
//    int mask = fcntl(new_sd, F_GETFL, 0);
//    //fcntl(new_sd, F_SETFL, mask | O_NONBLOCK);
//    int sz = (int) recv(new_sd, buff, (size_t) L, 0);
//    db(sz);
//    cerr << "|";
//    for (int i = 0; i < sz; i++)
//        cerr << buff[i];
//    cerr << "|";
//    cerr << endl;
//    cerr << ":" << buff << ":" << endl;
//    close(new_sd);
//    close(sd);


    //db(errno);
    //perror("sdf");
    //strerror(errno);
    //while (true);


    return 0;
}

//inet_ntop(PF_INET, (struct in_addr*)&(address->sin_addr.s_addr), ip, sizeof(ip)-1);

//struct sockaddr_in {
//   short   sin_family;
//   u_short sin_port;
//   struct  in_addr sin_addr;
//   char    sin_zero[8];
//};

