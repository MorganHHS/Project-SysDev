#ifndef ACTIONS_H
#define ACTIONS_H

#include <vector>
#include <string>
#include "../publisher/subscriber.hpp"
#include "../publisher/publisher.hpp"

namespace Functions{
    void setBed(std::vector<std::string> *vals, uint16_t fd);
    void setVenster(std::vector<std::string> *vals, uint16_t fd);
    void setStoel(std::vector<std::string> *vals, uint16_t fd);
    void setDeur(std::vector<std::string> *vals, uint16_t fd);
    void setZuil(std::vector<std::string> *vals, uint16_t fd);
    void setSchemerlamp(std::vector<std::string> *vals, uint16_t fd);
    void bed(std::vector<std::string> *vals, uint16_t fd);
    void venster(std::vector<std::string> *vals, uint16_t fd);
    void stoel(std::vector<std::string> *vals, uint16_t fd);
    void deur(std::vector<std::string> *vals, uint16_t fd);
    void zuil(std::vector<std::string> *vals, uint16_t fd);
    void schemerlamp(std::vector<std::string> *vals, uint16_t fd);
    void sendMessage(uint16_t fd, const char* message);
}

class Venster{
public:
    Venster(uint16_t fileDescriptor);
    void ldrRead(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "1"); }
    void potRead(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "2"); }
    void regelLcdPanel(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "1"); }
    void regelVenster(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "2"); }
private:
    int fd;
    int ledVal;
    int potVal;
    bool venster;
    Subscriber *subscriber;
};

class Stoel {
public:
    Stoel(uint16_t fileDescriptor);
    void trillenAan(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "3"); }
    void trillenUit(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "4"); }
    void ledAan(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "1"); }
    void stoelTrilCyclus(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "5"); }
    void stoelKnopTril(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "2"); }
private:
    int fd;
    uint16_t inputs;
    uint16_t drukSensor;
    Subscriber *subscriber;
};

class Bed {
public:
    Bed(uint16_t fileDescriptor);
    void regelLed(std::vector<std::string> *vals, uint16_t fd);
    void hulpNodig(std::vector<std::string> *vals, uint16_t fd);
    void afwezigheidBed(std::vector<std::string> *vals, uint16_t fd);
private:
    int fd;
    bool statusLed;
    bool hulp;
    int druk;
    Subscriber *subscriber;
};

class Deur {
public:
    Deur(uint16_t fileDescriptor);
    void deurOpenBinnen(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "1"); }
    void deurOpenBuiten(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "2"); }
    void brand(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "8"); }
private:
    int fd;
    int servoWaarde;
    Subscriber *subscriber;
};

class Zuil {
public:
    Zuil(uint16_t fileDescriptor);
    void deurBel(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "1"); }
    void zuilBrand(std::vector<std::string> *vals, uint16_t fd);
    void geenBrand(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "9"); }
private:
    int fd;
    bool fire;
    Subscriber *subscriber;
};

class Schemerlamp {
public:
    Schemerlamp(uint16_t fileDescriptor);
    void ledOn(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "1"); }
    void ledOff(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "2"); }
    void brand(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "8"); }
    void leesBeweging(std::vector<std::string> *vals, uint16_t fd) { Functions::sendMessage(this->fd, "4"); }
private:
    int fd;
    Subscriber *subscriber;
};

class Apartment {
public:
    Apartment();
    Venster *venster;
    Stoel *stoel;
    Bed *bed;
    Deur *deur;
    Zuil *zuil;    
    Schemerlamp *schemerlamp;
    Publisher *brandP;
    Publisher *geenBrandP;
    Publisher *patientHulpP;
    Publisher *deurbelP;
};
#endif // !ACTIONS_H
