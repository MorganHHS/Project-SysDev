#include <iostream>

#include "server/server.hpp"
#include "controller/controller.hpp"
#include "controller/actions.hpp"
#include "controller/actions.hpp"

Server server;

void addHandlers()
{
}
int main(int argc, char const *argv[])
{
    addHandlers();
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
