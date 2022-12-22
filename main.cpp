#include <iostream>

#include "server/server.hpp"
#include "controller/controller.hpp"
#include "controller/actions.hpp"

Server server;

void addHandlers()
{
    Controller::addHandler("deurKnop1",Deur::deurOpen);
    Controller::addHandler("brand", Deur::brand);
    Controller::addHandler("geenBrand", Deur::geenBrand);
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
        server.checkSocket();
    }
    return 0;
}
