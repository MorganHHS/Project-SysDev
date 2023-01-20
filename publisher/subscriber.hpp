#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

#include <cstdio>
#include <string>

class Subscriber {
    public:
        void onMessage(std::string message);
        Subscriber() {printf("Subscriber made\n");};
};

#endif // !SUBSCRIBER_H
