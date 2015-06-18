#include <iostream>
#include <assert.h>
#include <thread>
#include <fstream>
#include "MyEpoll.h"
#include "debug.h"
#include "HttpWorker.h"

using namespace std;



map<int, shared_ptr < HttpWorker > > workers;


void onAC2(shared_ptr < MyClient > client) {
    assert(workers.count(client->getSocketDescriptor()) == 0);
    workers[client->getSocketDescriptor()] = shared_ptr < HttpWorker > (new HttpWorker());
}



void onRC2(shared_ptr < MyClient > client) {
    assert(workers.count(client->getSocketDescriptor()) != 0);
    shared_ptr < HttpWorker > worker = workers[client->getSocketDescriptor()];
    while (true) {
        auto prAddress = worker->readMessage(client);
        if (!prAddress.first) break;

        //auto question = worker->split(prAddress.second, '?');

        //if ((int)question.size() > 1)
            //prAddress.second = "/hello.html";

        worker->sendFile(prAddress.second.URL, client);
    }

}

void test2() {
    MyEpoll ep;
    ep.add(7772, onAC2, onRC2);
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

