#include <iostream>
#include <cstring>
#include <map>
#include <sys/socket.h>

#include "controller.hpp"
#include "../parser/parser.hpp"
#include "get_action.hpp"

Parser prs;
std::map<std::string, Controller::handler> handlers;

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
    std::cout << "Got input '" << buffer << "' from" << fd << std::endl;
    prs.parse(buffer);

    std::string key;
    try
    {
        key = prs.values.at(0);
    }
    catch(const std::out_of_range& e)
    {
        std::cerr << "no action" << std::endl;
    }
    
    Controller::handler h = NULL;
    if(get_action<std::string, Controller::handler>(&handlers, key, &h))
    {
        std::cout << "handler at: '" << h << "' Running action: '" << key << "'" << std::endl;

        h(&prs.values, fd); 
    } else
    {
        std::cerr << "The action was not found: " << key << std::endl;
    }

    prs.values.clear();

}

void Controller::addHandler(std::string key, Controller::handler h)
{
    handlers.insert(std::pair<std::string, Controller::handler>(key,h));
}
