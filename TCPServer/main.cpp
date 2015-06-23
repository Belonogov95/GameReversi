#include <iostream>
#include <assert.h>
#include <thread>
#include <fstream>
#include "MyEpoll.h"
#include "debug.h"
#include "HttpWorker.h"
#include "reversiServer/GameState.h"
#include "Executor.h"
#include "TcpSocketClient.h"
#include "TcpSocketServer.h"
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/timerfd.h>


using namespace std;

struct GamePointer {
    int gameId;
    int enemyId;
    int color;

    GamePointer() { }

    GamePointer(int gameId, int enemyId, int color) : gameId(gameId), enemyId(enemyId), color(color) {
        assertMy(color == 1 || color == 2);
    }
};


map<string, int> idByLogin;

map<int, string> loginById;
map<int, set<int> > edges;
map<int, GamePointer> currentGame;

map<int, GameState> gameById;
int curPlayerId;
int curGame;



set<string> myFiles;


string zip(string s, char quote) {
    return quote + s + quote;
}

string arrayJSFromStrings(vector<string> data, char quote = '\'') {
    string res = "[";
    for (int i = 0; i < (int) data.size(); i++) {
        res += zip(data[i], quote);
        if (i + 1 != (int) data.size())
            res += ", ";
    }
    res += "]";
    return res;
}

vector<string> generatePlayersList(int id, pair<int, int> &match) {
    vector<string> data;
    for (auto x: loginById)
        if (x.first != id)
            data.push_back(x.second);
    string s0 = arrayJSFromStrings(data, '\"');
    vector<string> dataF;
    for (auto x: edges[id])
        dataF.push_back(loginById[x]);
    string s1 = arrayJSFromStrings(dataF, '\"');
    for (auto x: edges[id]) {
        if (edges[x].count(id) == 1) {
            match = make_pair(id, x);
            break;
        }
    }
    vector<string> tmp;
    tmp.push_back(s0);
    tmp.push_back(s1);
    return tmp;
}


void dump() {
    db2(curPlayerId, curGame);
    db("login - id");
    for (auto x: loginById)
        cerr << x.second << " " << x.first << endl;
    db("currentGame");
    for (auto x: currentGame)
        cerr << "id  enemyId color, gameId: " << x.first << "    " << x.second.enemyId << " " <<
        x.second.color << " " << x.second.gameId << endl;
    db("===========");
}


void boardQuery(Message message, shared_ptr<TcpSocketClient> client, shared_ptr<HttpWorker> worker) {
    int id = idByLogin[message.get("login")];
    int gameId = currentGame[id].gameId;
    GameState state = gameById[gameId];
    string board = state.toJSArray();
    vector<string> tmp(6);
    tmp[0] = board;
    tmp[1] = to_string(state.getCntWhite());
    tmp[2] = to_string(state.getCntBlack());
    tmp[3] = to_string(state.player);
    tmp[4] = to_string(state.finished);
    if (state.finished) {
        if (state.getCntBlack() == state.getCntWhite())
            tmp[5] = "Draw";
        else
            tmp[5] = (((state.getCntWhite() > state.getCntBlack()) == (currentGame[id].color == 1)) ? loginById[id] :
                      loginById[currentGame[id].enemyId]) + " Win!";
    }
    worker->sendString(arrayJSFromStrings(tmp), client);
}


void onAccept(shared_ptr<TcpSocketClient> client, map<int, shared_ptr < TcpSocketClient > > & dataByDescriptor) {
    db2("main on Accept", client->socketDescriptor);
    dataByDescriptor[client->socketDescriptor] = client;
}

//map<int, shared_ptr < TcpSocketClient > > dataByDescriptor;
//map<int, shared_ptr<HttpWorker>> workers; // by socket descriptor


void onReceive(int descriptor, u_int32_t flagMask, map < int, shared_ptr < TcpSocketClient > > & dataByDescriptor,
               map<int, shared_ptr<HttpWorker>> &workers) {

    assertMy(dataByDescriptor.count(descriptor) == 1);
    shared_ptr < TcpSocketClient > client = dataByDescriptor[descriptor];
    client->writeFromEpoll();
    if (workers.count(descriptor) == 0)
        workers[descriptor] = shared_ptr<HttpWorker>(new HttpWorker());

    shared_ptr<HttpWorker> worker = workers[descriptor];
    while (true) {
        auto prAddress = worker->readMessage(client);
        auto message = prAddress.second;
        if (prAddress.first == 0) {
            workers.erase(descriptor);
            dataByDescriptor.erase(descriptor);
            return;
        }
        if (prAddress.first == -1)
            return;
        if (message.URL == "/login") {
            assertMy((int) message.body.size() == 1);
            string login = message.get("login");
            bool exist = 0;
            for (auto x: loginById)
                if (x.second == login)
                    exist = 1;
            exist |= login.size() == 0;
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
            assertMy(idByLogin.count(login) == 1);
            int id = idByLogin[login];
            pair<int, int> match = make_pair(-1, -1);
            vector < string > tmp = generatePlayersList(id, match);
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
                db2(x, y);
                assertMy(currentGame.count(x) == 0);
                assertMy(currentGame.count(y) == 0);

                currentGame[x] = GamePointer(curGame, y, 1);
                currentGame[y] = GamePointer(curGame, x, 2);
                gameById[curGame] = GameState();
                curGame++;
            }
            if (currentGame.count(id) != 0) {
                tmp.push_back(loginById[currentGame[id].enemyId]);
                tmp.push_back(to_string(currentGame[id].color));
            }
            string qq = arrayJSFromStrings(tmp);
            worker->sendString(qq, client);
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
                if (state.makeMove(x, y, result)) {
                    gameById[gameId] = result;
                    if (!gameById[gameId].isPossibleMove()) {
                        gameById[gameId].nextTurn();
                    }
                    if (!gameById[gameId].isPossibleMove()) {
                        gameById[gameId].finished = true;
                    }
                }
            }
            worker->sendString("OK!MOVE", client);
        }
        else if (message.URL == "/board") {
            boardQuery(message, client, worker);
        }
        else if (message.URL == "/invite") {
            string login = message.get("login");
            assertMy(idByLogin.count(login) == 1);
            int id = idByLogin[login];
            int invitedId = idByLogin[message.get("target")];
            edges[invitedId].insert(id);
            worker->sendString("OK!", client);
        }
        else if (myFiles.count(message.URL) == 1) {
            if (message.URL == "/") message.URL = "/index.html";
            worker->sendFile(message.URL, client);
        }
        else {
            assertMy(false);
        }
    }

}


struct MyPipe {
    int pipeIn, pipeOut;
    MyPipe() {
        int p[2];
        assertMy(pipe(p) == 0);
        pipeIn = p[0];
        pipeOut = p[1];
    }
    ~MyPipe() {
        close(pipeIn);
        close(pipeOut);
    }
};
//
//struct MyTimer {
//    int fd;
//
//    MyTimer() {
//        fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
//        struct itimerspec tmr;
//        memset(&tmr, 0, sizeof(tmr));
//        tmr.it_interval.tv_sec = 1;
//        assertMy(timerfd_settime(fd, TFD_TIMER_ABSTIME, &tmr, NULL) != -1);
//    }
//
//    ~MyTimer() {
//        close(fd);
//    }

//
//       int timerfd_settime(int fd, int flags,
//                           const struct itimerspec *new_value,
//                           struct itimerspec *old_value);
//
//       int timerfd_gettime(int fd, struct itimerspec *curr_value);
//        }

//
//};

int fdFromEpoll;

void handl(int signum) {
    char buffer[10];
    sprintf(buffer, "wake up!");
    assertMy(write(fdFromEpoll, buffer, strlen(buffer)) != -1);
}

int main() {
    db2(EPOLLIN, EPOLLOUT);
    myFiles.insert("/");
    myFiles.insert("/index.html");
    myFiles.insert("/favicon.ico");
    myFiles.insert("/cat.jpg");
    myFiles.insert("/script.js");
    myFiles.insert("/mystyle.css");

    freopen("config.txt", "r", stdin);
    int port;
    string ipAddress;
    cin >> port;
    cin >> ipAddress;
    db2(port, ipAddress);

    MyPipe myPipe;
    db2(myPipe.pipeOut, myPipe.pipeIn);
    fdFromEpoll = myPipe.pipeOut;

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handl;
    sigaction(SIGINT, &sa, NULL);

    Executor executor;
    map<int, shared_ptr < TcpSocketClient > > dataByDescriptor;
    map<int, shared_ptr<HttpWorker>> workers; // by socket descriptor

    function < void (u_int32_t ) > fBreak = [&](u_int32_t y) {
        cerr << "false";
        executor.running = false;
    };
//    function < void (int, u_int32_t) > fTimer = [] (int fd, u_int32_t y) {
//        cerr << "timer\n";
//    };

//    MyTimer myTimer;

//    executor.add(myTimer.fd, fTimer, EPOLLIN);
    executor.add(myPipe.pipeIn, fBreak, EPOLLIN);

    function < void (shared_ptr < TcpSocketClient > ) > alfAC = [&](shared_ptr < TcpSocketClient > x) {
        onAccept(x, dataByDescriptor);
    };
    function < void (int, u_int32_t) > alfRC= [&](int x, u_int32_t y) {
        onReceive(x, y, dataByDescriptor, workers);
    } ;

    TcpSocketServer tcpServer(port, ipAddress, alfAC, alfRC, &executor);

    executor.run();



    dataByDescriptor.clear();
    workers.clear();

    return 0;
}


