#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <cstring>
#include <cstdint>

namespace Controller
{
    void handleServerConnection(uint16_t fd);
    void handleServerDisconnect(uint16_t fd);
    void handleServerInput(uint16_t fd, char *buffer);
}

#endif //