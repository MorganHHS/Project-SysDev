/*
* File:   server.h
* author M0rga-n
*
* Created on December 9, 2022, 11:16 AM
*/

#ifndef SERVER_H
#define SERVER_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#define BUFFER_SIZE 100 //Budder size for output
#define DEFAULT_PORT 8080 // Default port

class Server
{
public:
    Server(); //Start server
    Server(int port); //Start server with custom port

    ~Server();

    void init();
    void shutdown();
    void loop();

private:

    fd_set masterfds;
    fd_set tempfds;

    uint16_t max_sd;

    int server_fd; //socket that recieves connections
    int new_socket;

    //server adderess details
    struct sockaddr_in servaddr;

    char buffer[BUFFER_SIZE];

    //Prototypes
    void setup(int port);
    void recvInputFromConnection(int fd);
    void initSocket();
    void bindSocket();
    void listenSocket();
    void newConnection();

    struct sockaddr_storage client_addr;
};

#endif // !SERVER_H
