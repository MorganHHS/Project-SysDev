#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <cstdio>
#include <string>
#include "../controller/controller.hpp"

class Subscriber {
    public:
        Subscriber(uint16_t fileDescriptor) {fd = fileDescriptor;}
        void onMessage(std::string message) {
            const char *messageC;
            messageC = message.c_str();
            send(fd, messageC, strlen(messageC), 0);
        };
        Subscriber() {printf("Subscriber made\n");};
    private:
        uint16_t fd;
};

#endif // !SUBSCRIBER_H
