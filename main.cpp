#include <iostream>

#include "server.h"

Server server;

int main(int argc, char const *argv[])
{
    server.init();

    while(true)
    {
        server.loop();
    }
    return 0;
}
