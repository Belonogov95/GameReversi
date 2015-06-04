#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>
#include <thread>
#include <unistd.h>
#include <fcntl.h>
#include "MySocket.h"
#include "MyEpoll.h"
//#include <map>

using namespace std;

#define db(x) cerr << #x << " = " << x << endl

void onAC(MyClient cl) {
    db("onAC");
    char * s = (char *) string("abacaba").c_str();
}

void onRC(MyClient cl) {
    db("onRC");
}


void test() {
    MySocket sc(7777);
    MyEpoll ep;

    cerr << "before\n";
    //ep.add(sc, onAC, onRC);
    cerr << "added\n";
    ep.start();
    exit(0);
}

int main() {
    test();

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    db(sd);
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(39005);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    assert(bind(sd, (const sockaddr *) &address, sizeof(address)) == 0);
    int res = listen(sd, 5);
    db(res);
    assert(res == 0);
    sockaddr_storage outAddr;
    socklen_t len = sizeof outAddr;

    int new_sd = accept(sd, (sockaddr*)&outAddr, &len);
    //char *s = (char *) string("8888").c_str();
    //int bytes_sent = (int) send(new_sd, s, strlen(s), 0);

    int L = 10;
    char buff[L];
    for (int i = 0; i < L; i++)
        buff[i] = '_';
    db("before recv");

    int mask = fcntl(new_sd, F_GETFL, 0);
    //fcntl(new_sd, F_SETFL, mask | O_NONBLOCK);
    int sz = (int) recv(new_sd, buff, (size_t) L, 0);
    db(sz);
    cerr << "|";
    for (int i = 0; i < sz; i++)
        cerr << buff[i];
    cerr << "|";
    cerr << endl;
    cerr << ":" << buff << ":" << endl;
    close(new_sd);
    close(sd);


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

