#include "server.h"

// define DEBUG for debug infomation

Server::Server()
{
    setup(DEFAULT_PORT);  //Constructor
}

Server::Server(int port)
{
    setup(port);
}

Server::~Server()
{
    #ifdef DEBUG
    printf("[SERVER] [DESTRUCTOR] Destroying Server.");
    #endif
    close(server_fd);
}

void Server::setup(int port)//Create an endpoint for communication.
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

void Server::initSocket()//Set the socket options
{
    #ifdef DEBUG
    printf("[SERVER] Initiating socket.\n");
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

void Server::bindSocket()//Bind a name to a socket.
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

void Server::listenSocket()//Listen for socket communication and limit que of icoming connections.
{
    #ifdef DEBUG
    std::cout << "[SERVER] listen starting\n";
    #endif // DEBUG
    int ret = listen(server_fd, 3);
    #ifdef DEBUG
    printf("[SERVER] listen() return value: %d.\n", ret);
    #endif // DEBUG
    if (ret < 0)
    {
        perror("listen() failed");
    }
}

void Server::shutdown()//Close the file descriptor for the socket.
{
    int ret = close(server_fd);
    #ifdef DEBUG
    printf("Closing server_fd return value: %d\n", ret);
    #endif // DEBUG
}

void Server::newConnection()//Add new connection on the socket.
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

        if (new_socket > max_sd)
        {
            max_sd = new_socket;
            #ifdef DEBUG
            std::cout << "[SERVER] incrementing max_sd to " << max_sd << std::endl;
            #endif // DEBUG
        }
        #ifdef DEBUG
        printf("[SERVER] New connection on socket '%d'.\n", new_socket);
        #endif // DEBUG
    }
    newConnectionC(new_socket);
}

void Server::recvInputFromConnection(int fd)//Return buffer from connection.
{
    int checkrecv = recv(fd, buffer, BUFFER_SIZE, 0);
    if (checkrecv <= 0)
    {
        if (0 == checkrecv)
        {
            #ifdef DEBUG
            printf("[SERVER] [DISCONNECT] connection '%d' disconnected\n", fd);
            #endif // DEBUG
            disconnectedC(fd);
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
    printf("[SERVER] [RECV] Recieved '%s' from client.\n", buffer);
    #endif // DEBUG
    receiveC(fd, buffer);
    //printf("%s\n", buffer);
    bzero(&buffer, BUFFER_SIZE);
}

void Server::loop()//Check for change on socket connections
{
    tempfds = masterfds;
    #ifdef DEBUG
    printf("[SERVER] max sd = '%hu' \n", max_sd);
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

void Server::init()//initialise socket
{
    initSocket();
    bindSocket();
    listenSocket();
}

void Server::onConnect(void(*c)(uint16_t))
{
    newConnectionC = c;
}

void Server::onDisconnect(void(*dc)(uint16_t))
{
    disconnectedC = dc;
}

void Server::onInput(void(*in)(uint16_t, char *buffer))
{
    receiveC = in;
}