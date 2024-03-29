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
    Controller::addHandler("bed", Functions::bed);
    Controller::addHandler("venster", Functions::venster);
    Controller::addHandler("stoel", Functions::stoel);
    Controller::addHandler("schemerlamp", Functions::schemerlamp);
    Controller::addHandler("zuil", Functions::zuil);

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
