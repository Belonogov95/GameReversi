#include <iostream>
#include <assert.h>
#include <thread>
#include "MyEpoll.h"

using namespace std;

#define db(x) cerr << #x << " = " << x << endl

//vector<char> strToVector(string s) {
//    vector<char> g;
//    for (auto x: s)
//        g.push_back(x);
//    return g;
//}

map<int, int> cnt;

void onAC(shared_ptr<MyClient> cl) {
    db("onAC");
    cnt[cl->getSocketDescriptor()] = 0;

    //vector < char > data = strToVector("abacaba2");
//    cl->write(data);
}


string toString(vector<char> y) {
    string t;
    for (int i = 0; i < (int) y.size(); i++)
        t += y[i];
    return t;
}


void onRC(shared_ptr<MyClient> cl) {
//    db2("onRC", cl->getSocketDescriptor());

    vector<char> buffer(10);
    int sz = cl->read(buffer);
    sz = max(sz, 0);
    if (sz == 0) return;
//    db2("--------------", sz);
//    db("after read");
    buffer.resize((unsigned long) sz);
    cout << toString(buffer) << endl;
    cl->write("hello, " + toString(buffer));
    cnt[cl->getSocketDescriptor()]++;
    if (cnt[cl->getSocketDescriptor()] == 3)
        cl->closeClient();
    assert(cnt[cl->getSocketDescriptor()] <= 3);
//    db("=aaaaaa");
}


void test() {
    int shift = 1;
    MyEpoll ep;

    ep.add(7770 + shift, onAC, onRC);
    ep.add(8880 + shift, onAC, onRC);

    ep.start();
    exit(0);
}

int main() {
    test();


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

