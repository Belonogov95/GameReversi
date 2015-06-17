#include <iostream>
#include <assert.h>
#include <thread>
#include <fstream>
#include "MyEpoll.h"
#include "debug.h"

using namespace std;


const string LINE_BREAK = "\r\n";

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


const int BUF_SZ = 10000;

map < int, vector < string > > mainBuf;

void onAC2(shared_ptr < MyClient > client) {
    db("onAC2");
    assert(mainBuf.count(client->getSocketDescriptor()) == 0);
    mainBuf[client->getSocketDescriptor()] = vector < string > ();
}

vector < string > split(string s, char ch) {
    vector < string > res;
    //cerr << "debug\n";
    //cerr << "s:" << s << ":\n";;
    //cerr << "ch:" << ch << ":\n";
    //db2(s, ch);
    for (int i = 0; i < (int)s.size(); i++) {
        for (; i < (int)s.size() && s[i] == ch; i++);
        int j = i;
        for (; i < (int)s.size() && s[i] != ch; i++);
        if (i - j > 0) {
            res.push_back(s.substr(j, i - j));
        }
    }
    return res;
}

string trim(string s) {
    int l= 0;
    for (; l < (int)s.size() && s[l] == ' '; l++);
    int r = (int)s.size() - 1;
    for (; r > 0 && s[r] == ' '; r--);
    if (r < l) return string();
    return s.substr(l, (r - l) + 1);
}

void sendFile(string path, shared_ptr < MyClient > client) {
    path = "../site" + path;
    db(path);
    ifstream in(path, ios::binary);
    in.seekg(0, in.end);
    int length = in.tellg();
    in.seekg(0, in.beg); 
    string message(length, 0);
    in.read((char *)message.data(), length);

    string header = "HTTP/1.1 200 OK" + LINE_BREAK;
    header += "Content-Length: " + to_string(message.size()) + LINE_BREAK;

    string res = header + LINE_BREAK + message;
    client->write(res);
}

void printBuff(vector < string > data) {
    cerr << "===========: buffer\n";
    for (auto x: data)
        cerr << x << endl;  
}

pair < bool, string > readMessage(shared_ptr < MyClient > client) {
   string buffer(BUF_SZ, 0);
    int len = client->read(buffer);
    db(len);
    if (len == 0)  {
        client->closeClient();
        mainBuf.erase(client->getSocketDescriptor());
    }
    if (len <= 0) return make_pair(false, string());
    assert(len > 0);
    buffer.resize(len);
//    cerr << "buffer:" << buffer << ":\n";
    vector < string >  & buff = mainBuf[client->getSocketDescriptor()];
    for (auto x: buffer) {
        if (x == '\n' || buff.empty())
            buff.push_back(string());
        if (x != '\n')
            buff.back().push_back(x);
    }
    /// TODO message doesn't in one packet
    /// try read
    printBuff(buff);

    assert(!buff.empty());
    auto tmp = split(buff[0], ' ');
    db(tmp[0]);
    assert(tmp[0] == "GET");
    string address = tmp[1];
    buff.clear();
    return make_pair(1, address);
}

void onRC2(shared_ptr < MyClient > client) {
    auto prAddress = readMessage(client);
    if (!prAddress.first) return;
    sendFile(prAddress.second, client);
//    int textLen = -1;
//    int i = 1;
//    for (; i < (int)buff.size(); i++) {
//        if (buff[i].empty()) break;
//        auto tmp = split(buff[i], ':');
//        assert((int)tmp.size() >= 2);
//        if (tmp[0] == "Content-Length")
//            textLen = stoi(trim(tmp[1]));
//    }
    //assert(textLen != -1);
}

void test2() {
    MyEpoll ep;
    ep.add(7770, onAC2, onRC2);
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

