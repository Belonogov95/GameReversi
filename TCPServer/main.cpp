#include <iostream>
#include <assert.h>
#include <thread>
#include <fstream>
#include "MyEpoll.h"
#include "debug.h"
#include "HttpWorker.h"
#include "MyClient.h"
#include "reversiServer/GameState.h"

using namespace std;

struct GamePointer {
    int gameId;
    int enemyId;
    int color;

    GamePointer() { }

    GamePointer(int gameId, int enemyId, int color) : gameId(gameId), enemyId(enemyId), color(color) {
        assert(color == 1 || color == 2);
    }
};


map<int, shared_ptr<HttpWorker>> workers;
map<int, string> loginById;

map<string, int> idByLogin;
map<int, set<int> > edges;
map<int, GamePointer> currentGame;
map<int, GameState> gameById;
int curPlayerId;
int curGame;

set<string> myFiles;


void onAC2(shared_ptr<MyClient> client) {
    //assert(workers.count(client->getSocketDescriptor()) == 0);
    //workers[client->getSocketDescriptor()] = shared_ptr<HttpWorker>(new HttpWorker());
}


string arrayJSFromStrings(vector<string> data) {
    string res = "[";
    for (int i = 0; i < (int) data.size(); i++) {
        res += "\"" + data[i] + "\"";
        if (i + 1 != (int) data.size())
            res += ", ";
    }
    res += "]";
    return res;
}


string generatePlayersList(int id, pair<int, int> &match) {
    vector<string> data;
    for (auto x: loginById)
        if (x.first != id)
            data.push_back(x.second);
    string s0 = arrayJSFromStrings(data);
    vector<string> dataF;
    for (auto x: edges[id])
        dataF.push_back(loginById[x]);
    string s1 = arrayJSFromStrings(dataF);
    int competitor = -1;
    for (auto x: edges[id]) {
        if (edges[x].count(id) == 1) {
            competitor = x;
            break;
        }
    }
    if (competitor != -1) {
        match = make_pair(competitor, id);
        return "[" + s0 + ", " + s1 + ", " + "'" + loginById[competitor] + "'" + "]";
    }
    else {
        return "[" + s0 + ", " + s1 + "]";
    }
}


void boardQuery(Message message, shared_ptr < MyClient > client, shared_ptr < HttpWorker > worker) {
    int id = idByLogin[message.URL];
    int gameId = currentGame[id].gameId;
    GameState state = gameById[gameId];
    string board = state.toJSArray();
    vector < string > tmp(3);
    tmp[0] = board;
    tmp[1] = loginById[id];
    tmp[2] = loginById[currentGame[id].enemyId];
    if (currentGame[id].color != 1)
        swap(tmp[1], tmp[2]);
    worker->sendString(arrayJSFromStrings(tmp), client);
}

void onRC2(shared_ptr<MyClient> client) {
    int descriptor = client->getSocketDescriptor();
    if (workers.count(descriptor) == 0)
        workers[descriptor] = shared_ptr<HttpWorker>(new HttpWorker());
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
            string login = message.get("login");
            bool exist = 0;
            for (auto x: loginById)
                if (x.second == login)
                    exist = 1;
            if (exist) {
                worker->sendString("unsuccess", client);
            }
            else {
                loginById[curPlayerId] = login;
                idByLogin[login] = curPlayerId;
                curPlayerId++;
                worker->sendString("success", client);
            }
        }
        else if (message.URL == "/players") {
            string login = message.get("login");
            assert(idByLogin.count(login) == 1);
            int id = idByLogin[login];
            pair<int, int> match = make_pair(-1, -1);
            string tmp = generatePlayersList(id, match);
            if (match.first != -1) {
                if (rand() % 2 == 1)
                    swap(match.first, match.second);
                int x = match.first;
                int y = match.second;
                /// clear invite list
                for (int i = 0; i < curPlayerId; i++) {
                    edges[i].erase(x);
                    edges[i].erase(y);
                }
                GameState state;
                db2(x, y);
                assert(currentGame.count(x) == 0);
                assert(currentGame.count(y) == 0);

                currentGame[x] = GamePointer(curGame, y, 1);
                currentGame[y] = GamePointer(curGame, x, 2);
                gameById[curGame] = state;
                curGame++;
            }
            if (currentGame.count(id) != 0) {
                db2("game already exist", id + " " + loginById[id]);
                tmp = "[ \"\", \"\", \"" + loginById[currentGame[id].enemyId] + "\"]";
            }
            db(tmp);
            worker->sendString(tmp, client);
        }
        else if (message.URL == "/move") {
            int id = idByLogin[message.get("login")];
            int x = stoi(message.get("x"));
            int y = stoi(message.get("y"));

            int gameId = currentGame[id].gameId;
            int color = currentGame[id].color;
            GameState state = gameById[gameId];
            if (state.player == color) {
                GameState result;
                if (state.go(x, y, result)) {
                    gameById[gameId] = result;
                }
                else {
                    db("===========incorrect move");
                }
            }
            else {
                db("incorrect color");
            }
        }
        else if (message.URL == "/board") {
            //TODO
            boardQuery(message, client, worker);

        }
        else if (message.URL == "/invite") {
            string login = message.get("login");
            assert(idByLogin.count(login) == 1);
            int id = idByLogin[login];
            int invitedId = idByLogin[message.get("target")];
            edges[invitedId].insert(id);
        }
        else if (myFiles.count(message.URL) == 1) {
            if (message.URL == "/") message.URL = "/index.html";
            worker->sendFile(message.URL, client);
        }
        else {
            db(message.URL);
            assert(false);
        }
    }

}


void test2() {
    myFiles.insert("/");
    myFiles.insert("/index.html");
    myFiles.insert("/favicon.ico");
    myFiles.insert("/cat.jpg");
    myFiles.insert("/script.js");
    myFiles.insert("/mystyle.css");

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

