#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstring>
#include <cstdint>
#include <vector>

namespace Controller
{
    typedef void (*handler)(std::vector<std::string> *TokenArray);

    void handleServerConnection(uint16_t fd);
    void handleServerDisconnect(uint16_t fd);
    void handleServerInput(uint16_t fd, char *buffer);
    
    void addHandler(std::string key, handler h);
}

#endif // !CONTROLLER_H