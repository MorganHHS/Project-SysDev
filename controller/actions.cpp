#include <cstdint>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include "actions.hpp"

void Deur::deurOpen(std::vector<std::string> *vals, uint16_t fd)
{
    const char* message = "deuropen";
    send(fd, message, strlen(message), 0);
}

void brand(std::vector<std::string> *vals, uint16_t fd)
{ 
    const char* message = "brand";
    send(fd, message, strlen(message), 0);
}

void geenBrand(std::vector<std::string> *vals, uint16_t fd)

{
    const char* message = "geenbrand";
    send(fd, message, strlen(message), 0);
}
