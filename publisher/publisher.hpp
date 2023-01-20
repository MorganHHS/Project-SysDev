#ifndef PUBLISHER_H
#define PUBLISHER_H

#include <string>
#include <memory>
#include <vector>

#include "subscriber.hpp"

class Publisher
{
    public:
        void publish(std::string message) {
            for (const auto& subscriber : subscribers_) {
                subscriber->onMessage(message);
            }
        }

        void subscribe(Subscriber* subscriber) {
            subscribers_.push_back(subscriber);
        }

        void unSubscribe(std::shared_ptr<Subscriber> subscriber) {
            
        }


    private:
        std::vector<Subscriber*> subscribers_;
};

#endif // !PUBLISHER_H
