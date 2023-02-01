#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdio.h>
#include <sys/socket.h>
#include "actions.hpp"

Apartment mary;
Apartment *currentApartment = &mary;

Venster::Venster(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Stoel::Stoel(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Bed::Bed(uint16_t fileDescriptor) { 

    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Deur::Deur(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Zuil::Zuil(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
}

Schemerlamp::Schemerlamp(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Apartment::Apartment() {
    brandP = new Publisher;
    geenBrandP = new Publisher;
}

void Functions::setBed(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->bed = new Bed(fd);
}

void Functions::setVenster(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->venster = new Venster(fd);
}

void Functions::setStoel(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->stoel = new Stoel(fd);
}

void Functions::setDeur(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->deur = new Deur(fd);
}

void Functions::setZuil(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->zuil = new Zuil(fd);
}

void Functions::setSchemerlamp(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->schemerlamp = new Schemerlamp(fd);
}

void Functions::bed(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "regelLed")) {
        currentApartment->bed->regelLed(vals, fd);
    } else if (strcmp(function.c_str(), "hulpNodig")) {
        currentApartment->bed->hulpNodig(vals, fd);
    } else if(strcmp(function.c_str(), "afwezigheidBed")) {
        currentApartment->bed->afwezigheidBed(vals, fd);
    } 
}

void Functions::venster(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "ldrRead")) {
        currentApartment->venster->ldrRead(vals, fd);
    } else if (strcmp(function.c_str(), "potRead")) {
        currentApartment->venster->potRead(vals, fd);
    } else if (strcmp(function.c_str(), "regelLcdPanel")) {
        currentApartment->venster->regelLcdPanel(vals, fd);
    } else if (strcmp(function.c_str(), "regelVenster")) {
        currentApartment->venster->regelVenster(vals, fd);
    } 
}

void Functions::stoel(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "trillenAan")) {
        currentApartment->stoel->trillenAan(vals, fd);
    } else if (strcmp(function.c_str(), "trillenUit")) {
        currentApartment->stoel->trillenUit(vals, fd);
    } else if(strcmp(function.c_str(), "ledAan")) {
        currentApartment->stoel->ledAan(vals, fd);
    } else if(strcmp(function.c_str(), "stoelTrilCyclus")) {
        currentApartment->stoel->stoelTrilCyclus(vals, fd);
    } else if(strcmp(function.c_str(), "stoeKnopTril")) {
        currentApartment->stoel->stoeKnopTril(vals, fd);
    } 
}

void Functions::deur(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "deurOpenBinnen")) {
        currentApartment->deur->deurOpenBinnen(vals, fd);
    } else if (strcmp(function.c_str(), "deurOpenBuiten")) {
        currentApartment->deur->deurOpenBuiten(vals, fd);
    } else if(strcmp(function.c_str(), "brand")) {
        currentApartment->deur->brand(vals, fd);
    }
}

void Functions::zuil(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "deurBel")) {
        currentApartment->zuil->deurBel(vals, fd);
    } else if (strcmp(function.c_str(), "zuilBrand")) {
        currentApartment->zuil->zuilBrand(vals, fd);
    } else if(strcmp(function.c_str(), "geenBrand")) {
        currentApartment->zuil->geenBrand(vals, fd);
    }
}

void Functions::schemerlamp(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "ledOn")) {
        currentApartment->schemerlamp->ledOn(vals, fd);
    } else if (strcmp(function.c_str(), "ledOff")) {
        currentApartment->schemerlamp->ledOff(vals, fd);
    } else if(strcmp(function.c_str(), "brand")) {
        currentApartment->schemerlamp->brand(vals, fd);
    } else if(strcmp(function.c_str(), "leesBeweging")) {
        currentApartment->schemerlamp->leesBeweging(vals, fd);
    }
}

void Venster::ldrRead(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Venster::regelLcdPanel(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Venster::regelVenster(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}

void Venster::potRead(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}

void Deur::deurOpenBinnen(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Deur::deurOpenBuiten(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Deur::brand(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "8";
    send(fd, message, strlen(message), 0);
}

void Schemerlamp::ledOn(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Schemerlamp::ledOff(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "4";
    send(fd, message, strlen(message), 0);
}

void Schemerlamp::brand(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}

void Schemerlamp::leesBeweging(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "4";
    send(fd, message, strlen(message), 0);
}

void Zuil::deurBel(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Zuil::zuilBrand(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->brandP->publish("8");
    const char *message = "8";
    send(fd, message, strlen(message), 0);
}

void Zuil::geenBrand(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->brandP->publish("9");
}

void Stoel::trillenAan(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "3";
    send(fd, message, strlen(message), 0);
}

void Stoel::trillenUit(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "4";
    send(fd, message, strlen(message), 0);
}

void Stoel::ledAan(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "1";
    send(fd, message, strlen(message), 0);
}

void Stoel::stoelTrilCyclus(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "5";
    send(fd, message, strlen(message), 0);
}

void Stoel::stoeKnopTril(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}

void Bed::regelLed(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}

void Bed::hulpNodig(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}

void Bed::afwezigheidBed(std::vector<std::string> *vals, uint16_t fd) {
    const char *message = "2";
    send(fd, message, strlen(message), 0);
}
