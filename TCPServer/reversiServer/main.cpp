#include <iostream>
#include "GameServer.h"
#include "clients/ClientConsole.h"

using namespace std;

int main() {
    GameServer server;
    shared_ptr < ClientInterface > ptr1(new ClientConsole());
    shared_ptr < ClientInterface > ptr2(new ClientConsole());
    server.play(ptr1, ptr2);

}