#include <iostream>

#include "server/server.h"
#include "controller/controller.hpp"
#include "controller/actions.hpp"

Server server;

void addHandlers()
{
    Controller::addHandler("knop",Actions::deurOpen);
}
int main(int argc, char const *argv[])
{
    server.onConnect(&Controller::handleServerConnection);
    server.onDisconnect(&Controller::handleServerDisconnect);
    server.onInput(&Controller::handleServerInput);
    server.init();

    while(true)
    {
        server.loop();
    }
    return 0;
}
