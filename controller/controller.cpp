#include <iostream>
#include <cstring>

#include "controller.hpp"

void Controller::handleServerConnection(uint16_t fd)
{
    std::cout << "Controller: got connection from " << fd << std::endl;
}

void Controller::handleServerDisconnect(uint16_t fd)
{
    std::cout << "Controller: got disconnect from " << fd << std::endl;
}

void Controller::handleServerInput(uint16_t fd, char *buffer)
{
    std::cout << buffer << std::endl;
}