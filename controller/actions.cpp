#include <cstdint>
#include <iostream>
#include <sys/socket.h>
#include "actions.hpp"

void Actions::deurOpen(std::vector<std::string> *vals, uint16_t fd)
{
    send(fd, "knop", 16, 0);
}
