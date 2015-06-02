#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <assert.h>
#include <thread>
#include <unistd.h>
//#include <map>

using namespace std;

#define db(x) cerr << #x << " = " << x << endl

int main() {


    int sd = socket(AF_INET, SOCK_STREAM, 0);
    db(sd);
    sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(40001);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    assert(bind(sd, (const sockaddr *) &address, sizeof(address)) == 0);
    cerr << "sdf\n";
    assert(listen(sd, 5) == 0);
    cerr << "after \n";
    for (int i = 0; i < 1; i++) {
        cerr << "next step\n";
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    sockaddr_storage outAddr;
    socklen_t len = sizeof outAddr;
    db("before");
    int new_sd = accept(sd, (sockaddr*)&outAddr, &len);
    db(new_sd);
    char *s = "8888";
    int bytes_sent = (int) send(new_sd, s, strlen(s), 0);
    db(bytes_sent);

    for (int i = 0; i < 1; i++) {
        cerr << "next step\n";
        this_thread::sleep_for(chrono::milliseconds(1000));
    }


    int L = 10;
    char buff[L];
    for (int i = 0; i < L; i++)
        buff[i] = '_';
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
    while (true);


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

