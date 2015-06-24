#include <iostream>
#include <assert.h>
#include <thread>
#include <fstream>
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
const int MAX_WAIT = 5; // sec

typedef map<int, shared_ptr<HttpWorker> > MapWorkers;

struct GamePointer {
    shared_ptr < GameState > gameState;
    int enemyId;
    int color;
    GamePointer() : gameState(NULL), enemyId(-1), color(-1) { }

    GamePointer(shared_ptr < GameState > gameState, int enemyId, int color) : gameState(gameState), enemyId(enemyId), color(color) {
        myAssert(color == 1 || color == 2);
    }
};

struct Player {
    string login;
    set<int> edges;
    GamePointer game;
    int lastActivity;

    Player() {
        myAssert(false);
    }

    Player(string login, int lastActivity) : login(login), lastActivity(lastActivity) { }

};

map<string, int> idByLogin;
map<int, Player> data;

int curPlayerId;

set<string> myFiles;


bool checkOpponent(int id) {
    myCheck(data.count(id) != 0);
    myCheck(data[id].game.gameState != NULL);
    int enemy = data[id].game.enemyId;
    bool flag;
    if (data.count(enemy) == 0)
        flag = false;
    else
        flag = (data[enemy].game.gameState == data[id].game.gameState);
    return flag;
}


int getTime() {
    static struct timespec start;
    struct timespec curr;
    static int first_call = 1;
    int secs, nsecs;

    if (first_call) {
        first_call = 0;
        myAssert(clock_gettime(CLOCK_MONOTONIC, &start) != -1);
    }
    myAssert(clock_gettime(CLOCK_MONOTONIC, &curr) != -1);

    secs = curr.tv_sec - start.tv_sec;
    nsecs = curr.tv_nsec - start.tv_nsec;
    if (nsecs < 0) {
        secs--;
    }
    return secs;
}


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
    vector<string> playerList;
    for (auto &x: data)
        if (x.first != id && x.second.game.gameState == NULL)
            playerList.push_back(x.second.login);
    string s0 = arrayJSFromStrings(playerList, '\"');

    vector<string> dataF;
    for (auto x: data[id].edges)
        if (data[x].game.gameState == NULL)
            dataF.push_back(data[x].login);

    string s1 = arrayJSFromStrings(dataF, '\"');
    for (auto x: data[id].edges) {
        if (data[x].game.gameState == NULL && data[x].edges.count(id) == 1) {
            match = make_pair(id, x);
            break;
        }
    }
    vector<string> tmp;
    tmp.push_back(s0);
    tmp.push_back(s1);
    return tmp;
}

void myBoard(int id, shared_ptr<HttpWorker> worker, const Message &message) {
    if (data[id].game.gameState == NULL) {
        worker->sendString("fail", "400 Bad Request");
        return;
    }

    vector<string> tmp(7);
    if (!checkOpponent(id) && !data[id].game.gameState->finished) {
        tmp[6] = "leave";
        data[id].game = GamePointer();
    }
    else {
        shared_ptr < GameState > state = data[id].game.gameState;
        string board = state->toJSArray();
        tmp[0] = board;
        tmp[1] = to_string(state->getCntWhite());
        tmp[2] = to_string(state->getCntBlack());
        db2(state->player, data[id].game.color);
        tmp[3] = to_string(state->player == data[id].game.color);
        tmp[4] = to_string(state->finished);
        if (state->finished) {
            if (state->getCntBlack() == state->getCntWhite())
                tmp[5] = "Draw";
            else
                tmp[5] = (((state->getCntWhite() > state->getCntBlack()) == (data[id].game.color == 1)) ? data[id].login :
                          data[data[id].game.enemyId].login) + " Win!";
            data[id].game = GamePointer();
        }
    }
    worker->sendString(arrayJSFromStrings(tmp));
}


void myLogin(shared_ptr<HttpWorker> worker, const Message &message) {
    myCheck((int) message.body.size() == 1);
    string login = message.get("login");
    bool exist = 0;
    exist = idByLogin.count(login);
    exist |= login.size() == 0;
    if (exist) {
        worker->sendString("unsuccess");
    }
    else {
        data.insert(make_pair(curPlayerId, Player(login, getTime())));
        idByLogin[login] = curPlayerId;
        curPlayerId++;
        worker->sendString("success");
    }
}

void myPlayers(int id, shared_ptr<HttpWorker> worker, const Message &message) {
    pair<int, int> match = make_pair(-1, -1);
    vector<string> tmp = generatePlayersList(id, match);
    if (match.first != -1) {
        if (rand() % 2 == 1)
            swap(match.first, match.second);
        int x = match.first;
        int y = match.second;
        //for (int i = 0; i < curPlayerId; i++) {
        for (auto & cur: data) {
            cur.second.edges.erase(x);
            cur.second.edges.erase(y);
        }

        //db2(x, y);
        myCheck(data[x].game.gameState == NULL);
        myCheck(data[y].game.gameState == NULL);

        shared_ptr < GameState > gameState(new GameState());
        data[x].game = GamePointer(gameState, y, 1);
        data[y].game = GamePointer(gameState, x, 2);
    }
    if (data[id].game.gameState != NULL) {
        tmp.push_back(data[data[id].game.enemyId].login);
        tmp.push_back(to_string(data[id].game.color));
    }
    string qq = arrayJSFromStrings(tmp);
    worker->sendString(qq);
}

void myMove(int id, shared_ptr<HttpWorker> worker, const Message &message) {
    int x = stoi(message.get("x"));
    int y = stoi(message.get("y"));


    myCheck(data[id].game.gameState != NULL);
    shared_ptr < GameState > gameState = data[id].game.gameState;

    int color = data[id].game.color;
    if (gameState->player == color) {
        shared_ptr < GameState > result(new GameState());
        if (gameState->makeMove(x, y, result)) {
            myCheck(data[id].game.gameState->player != result->player);
            *(data[id].game.gameState) = *(result);
            if (!data[id].game.gameState->isPossibleMove()) {
                data[id].game.gameState->nextTurn();
            }
            if (!data[id].game.gameState->isPossibleMove()) {
                data[id].game.gameState->finished = true;
            }
        }
    }
    worker->sendString("OK!MOVE");
}

void myInvite(int id, shared_ptr<HttpWorker> worker, const Message &message) {
    if (data[id].game.gameState == NULL) {
        int invitedId = idByLogin[message.get("target")];
        data[invitedId].edges.insert(id);
    }
    worker->sendString("OK!");
}


void onAccept(shared_ptr<TcpSocketClient> client, MapWorkers &workers) {
    myCheck(workers.count(client->socketDescriptor.get()) == 0);
    workers.insert(make_pair(client->socketDescriptor.get(), shared_ptr<HttpWorker>(new HttpWorker(client))));
}

void onReceive(int descriptor, u_int32_t flagMask, MapWorkers &workers) {
    myCheck(workers.count(descriptor) == 1);
    shared_ptr<HttpWorker> worker = workers[descriptor];
    worker->writeBuffer(flagMask);

    while (true) {
        auto prAddress = worker->readMessage();
        auto message = prAddress.second;
        if (prAddress.first == 0) {
            workers.erase(descriptor);
            return;
        }
        if (prAddress.first == -1)
            return;

        db(message.URL);
        if (myFiles.count(message.URL) == 1) {
            if (message.URL == "/") message.URL = "/index.html";
            worker->sendFile(message.URL);
        }
        else if (message.URL == "/login") {
            myLogin(worker, message);
        }
        else {
            string login = message.get("login");
            if (login == "" || idByLogin.count(login) != 1) {
                worker->sendString("Error: incorrect login", "404 Page Not Found");
                return;
            }
            int id = idByLogin[login];
            data[id].lastActivity = getTime();
            if (message.URL == "/players") {
                myPlayers(id, worker, message);
            }
            else if (message.URL == "/move") {
                myMove(id, worker, message);
            }
            else if (message.URL == "/board") {
                myBoard(id, worker, message);
            }
            else if (message.URL == "/invite") {
                myInvite(id, worker, message);
            }
            else {
                myAssert(false);
            }
        }
    }

}

struct MyPipe {
    int pipeIn, pipeOut;

    MyPipe() {
        int p[2];
        myAssert(pipe(p) == 0);
        pipeIn = p[0];
        pipeOut = p[1];
    }

    ~MyPipe() {
        close(pipeIn);
        close(pipeOut);
    }
};

struct MyTimer {
    int fd;

    MyTimer() {
        fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
        db(fd);
        struct itimerspec tmr;
        memset(&tmr, 0, sizeof(tmr));
        tmr.it_interval.tv_sec = 1;
        tmr.it_value.tv_sec = 2;
        myAssert(timerfd_settime(fd, 0, &tmr, NULL) != -1);
        /// TODO rai
    }

    ~MyTimer() {
        close(fd);
    }
};

int fdFromEpoll;

void handl(int signum) {
    char buffer[10];
    sprintf(buffer, "wake up!");
    myAssert(write(fdFromEpoll, buffer, strlen(buffer)) != -1);
}

void read(int &port, string &ipAddress) {
    freopen("config.txt", "r", stdin);
    cin >> port;
    cin >> ipAddress;
    db2(port, ipAddress);

}

void initFiles() {
    myFiles.insert("/");
    myFiles.insert("/index.html");
    myFiles.insert("/favicon.ico");
    myFiles.insert("/mountains.jpg");
    myFiles.insert("/script.js");
    myFiles.insert("/mystyle.css");
}

void initSignal() {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handl;
    sigaction(SIGINT, &sa, NULL);
}


void clearOldUsers() {
    int currentTime = getTime();
    vector<int> forDelete;
    for (auto &x: data) {
        if (x.second.lastActivity + MAX_WAIT < currentTime) {
            forDelete.push_back(x.first);
        }
    }
    for (auto id: forDelete) {
        idByLogin.erase(data[id].login);
        data.erase(id);
    }
}


int main() {
//    assert(false);
    MyPipe myPipe;
    fdFromEpoll = myPipe.pipeOut;
    int port;
    string ipAddress;
    read(port, ipAddress);
    initFiles();
    initSignal();

    Executor executor;
    map<int, shared_ptr<HttpWorker> > workers;

    function<void(u_int32_t)> fBreak = [&](u_int32_t y) {
        executor.running = false;
    };
    executor.add(myPipe.pipeIn, fBreak, EPOLLIN);


    MyTimer myTimer;

    int timerFd = myTimer.fd;
    function<void(uint)> fTimer = [=](uint flagMask) {
        u_int64_t buffer;
        myAssert(read(timerFd, &buffer, sizeof(u_int64_t)) == 8);
        clearOldUsers();
    };

    executor.add(myTimer.fd, fTimer, EPOLLIN);

    function<void(shared_ptr<TcpSocketClient>)> alfAC = [&](shared_ptr<TcpSocketClient> x) {
        onAccept(x, workers);
    };
    function<void(int, uint)> alfRC = [&](int x, uint y) {
        onReceive(x, y, workers);
    };

    TcpSocketServer tcpServer(port, ipAddress, alfAC, alfRC, &executor);

    executor.run();
    db("after run");

    return 0;
}


