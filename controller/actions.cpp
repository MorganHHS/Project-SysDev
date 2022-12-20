#include <cstdint>
#include <iostream>
#include <sys/socket.h>
#include "actions.hpp"

void Deur::deurOpen(std::vector<std::string> *vals, uint16_t fd)
{
    std::string message = "1";
    send(fd, &message, message.length(), 0);
}
