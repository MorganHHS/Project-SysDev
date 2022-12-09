#include "server.h"

#define DEBUG

Server::Server()
{
    setup(DEFAULT_PORT);
}

Server::Server(int port)
{
    setup(port);
}

Server::~Server()
{
    #ifdef DEBUG

    #endif
    close(server_fd);
}

void Server::setup(int port)
{
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket setup failed");
    }

    FD_ZERO(&masterfds);
    FD_ZERO(&tempfds);

    memset(&servaddr, 0, sizeof(servaddr)); //set servaddr struct to 0
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
}

void Server::initSocket()
{
    #ifdef DEBUG

    #endif
    int opt = 1;
    int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    #ifdef DEBUG
    printf("[SERVER] setsocketopt() return value: %d\n", ret);
    #endif // DEBUG
    if (ret < 0)
    {
        perror("setsocketopt() failed");
        exit(EXIT_FAILURE);
    }
}

void Server::bindSocket()
{
    #ifdef DEBUG
    std::cout << "[SERVER] binding\n";
    #endif // DEBUG
    int ret = bind(server_fd, (struct sockaddr*) &servaddr, sizeof(servaddr));
    #ifdef DEBUG
    printf("[SERVER] bind() return value: %d\n", ret);
    #endif // DEBUG
    if (ret < 0)
    {
        perror("bind() failed");
    }
    FD_SET(server_fd, &masterfds);
    max_sd = server_fd;
}

void Server::listenSocket()
{
    #ifdef DEBUG
    std::cout << "[SERVER] listen starting\n";
    #endif // DEBUG
    int ret = listen(server_fd, 3);
    #ifndef DEBUG
    printf("[SERVER] listen() return value: %d.\n", ret);
    #endif // !DEBUG
    if (ret < 0)
    {
        perror("listen() failed");
    }
}

void Server::shutdown()
{
    int ret = close(server_fd);
    #ifdef DEBUG
    printf("Closing server_fd return value: %d\n", ret);
    #endif // DEBUG
}

void Server::newConnection()
{
    #ifdef DEBUG
    std::cout << "[SERVER] [CONNECTION] new connection\n";
    #endif // DEBUG
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t addrlen = sizeof(client_addr);
    new_socket = accept(server_fd, (struct sockaddr*) &client_addr, &addrlen);

    if (new_socket < 0)
    {
        perror("[SERVER] accept() failed");
    } else
    {
        FD_SET(new_socket, &masterfds);

        if (server_fd > max_sd)
        {
            max_sd = server_fd;
            #ifdef DEBUG
            std::cout << "[SERVER] incrementing max_sd to " << max_sd << std::endl;
            #endif // DEBUG
        }
        #ifdef DEBUG
        printf("[SERVER] New connection on socket '%d'.\n", new_socket);
        #endif // DEBUG
    }
}

void Server::recvInputFromConnection(int fd)
{
    int checkrecv = recv(fd, buffer, BUFFER_SIZE, 0);
    if (checkrecv <= 0)
    {
        if (0 == checkrecv)
        {
            close(fd);
            FD_CLR(fd, &masterfds);
            return;
        } else
        {
            perror("[SERVER] [ERROR] recv() failed");
        }
        close(fd);
        FD_CLR(fd, &masterfds);
        return;
    }
    #ifdef DEBUG
    printf("[SERVER] [RECV] REcieved '%s' from cluent.\n", buffer);
    #endif // DEBUG
    printf("%s", buffer);
    bzero(&buffer, BUFFER_SIZE);
}

void Server::loop()
{
    tempfds = masterfds;
    #ifdef DEBUG
    printf("[SERVER] max_sd = '%hu' \n", max_sd);
    std::cout << "[SERVER] calling select()\n";
    #endif // DEBUG
    int sel = select(max_sd + 1, &tempfds, NULL, NULL, NULL);
    if (sel < 0 )
    {
        perror("[SERVER] [ERROR] select() failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i <= max_sd; i++)
    {
        if (FD_ISSET(i, &tempfds))
        {
            if (server_fd == i)
            {
                newConnection();
            } else
            {
                recvInputFromConnection(i);
            }
        }
    }
}

void Server::init()
{
    initSocket();
    bindSocket();
    listenSocket();
}