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
    subscriber = new Subscriber;
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Stoel::Stoel(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    currentApartment->stoel = this;
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Bed::Bed(uint16_t fileDescriptor) { 

    fd = fileDescriptor;
    subscriber = new Subscriber;
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Deur::Deur(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    currentApartment->brandP->subscribe(subscriber);
    currentApartment->geenBrandP->subscribe(subscriber);
}

Zuil::Zuil(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
}

Schemerlamp::Schemerlamp(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
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
    } else if (strcmp(function.c_str(), "vensterOmlaag")) {
        currentApartment->venster->vensterOmlaag(vals, fd);
    } else if (strcmp(function.c_str(), "vensterOmhoog")) {
        currentApartment->venster->vensterOmhoog(vals, fd);
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
    } else if (strcmp(function.c_str(), "deurDicht")) {
        currentApartment->zuil->brand(vals, fd);
    } else if(strcmp(function.c_str(), "brand")) {
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