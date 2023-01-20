#ifndef ACTIONS_H
#define ACTIONS_H

#include <vector>
#include <string>
#include "../publisher/subscriber.hpp"
#include "../publisher/publisher.hpp"


class Venster{
public:
    Venster(uint16_t fileDescriptor);
private:
    int fd;
    int ledVal;
    int potVal;
    Subscriber *subscriber;
};

class Stoel {
public:
    Stoel(uint16_t fileDescriptor);
private:
    int fd;
    uint16_t inputs;
    uint16_t drukSensor;
    Subscriber *subscriber;
};

class Bed {
public:
    Bed(uint16_t fileDescriptor);
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
private:
    int fd;
    int servoWaarde;
    Subscriber *subscriber;
};

class Zuil {
public:
    Zuil(uint16_t fileDescriptor);
private:
    int fd;
    bool brand;
    Subscriber *subscriber;
};

class Schemerlamp {
public:
    Schemerlamp(uint16_t fileDescriptor);
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
};

namespace Functions{
    void bed(std::vector<std::string> *vals, uint16_t fd);
    void venster(std::vector<std::string> *vals, uint16_t fd);
    void stoel(std::vector<std::string> *vals, uint16_t fd);
    void deur(std::vector<std::string> *vals, uint16_t fd);
    void zuil(std::vector<std::string> *vals, uint16_t fd);
    void ldrRead(std::vector<std::string> *vals, uint16_t fd);
    void potRead(std::vector<std::string> *vals, uint16_t fd);
    void vensterOmlaag(std::vector<std::string> *vals, uint16_t fd);
    void vensterOmhoog(std::vector<std::string> *vals, uint16_t fd);
    void regelLcdPanel(std::vector<std::string> *vals, uint16_t fd);
    void regelVenster(std::vector<std::string> *vals, uint16_t fd);
    void trillenAan(std::vector<std::string> *vals, uint16_t fd);
    void trillenUit(std::vector<std::string> *vals, uint16_t fd);
    void ledAan(std::vector<std::string> *vals, uint16_t fd);
    void regelLed(std::vector<std::string> *vals, uint16_t fd);
    void hulpNodig(std::vector<std::string> *vals, uint16_t fd);
    void afwezigheidBed(std::vector<std::string> *vals, uint16_t fd);
    void deurOpen(std::vector<std::string> *vals, uint16_t fd);
    void deurDicht(std::vector<std::string> *vals, uint16_t fd);
    void brand(std::vector<std::string> *vals, uint16_t fd);
    void geefServoWaarde(std::vector<std::string> *vals, uint16_t fd);
}
#endif // !ACTIONS_H
