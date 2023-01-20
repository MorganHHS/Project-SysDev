#include <iostream>

#include "server/server.hpp"
#include "controller/controller.hpp"
#include "controller/actions.hpp"

Server server;

void addHandlers()
{
    Controller::addHandler("setBed", Functions::setBed);
    Controller::addHandler("setVenster", Functions::setVenster);
    Controller::addHandler("setStoel", Functions::setStoel);
    Controller::addHandler("setDeur", Functions::setDeur);
    Controller::addHandler("setZuil", Functions::setZuil);
    Controller::addHandler("setSchemerlamp", Functions::setSchemerlamp);
    Controller::addHandler("deur", Functions::deur);

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
