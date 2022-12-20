#include <cstdint>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include "actions.hpp"

void Deur::deurOpen(std::vector<std::string> *vals, uint16_t fd)
{
    const char* message = "1";
    send(fd, message, strlen(message), 0);
}
