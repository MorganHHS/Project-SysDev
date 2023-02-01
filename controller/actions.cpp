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
    currentApartment->patientHulpP->subscribe(subscriber);
}

Zuil::Zuil(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber(fd);
    currentApartment->deurbelP->subscribe(subscriber);
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
    deurbelP = new Publisher;
    patientHulpP = new Publisher;
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
    if (strcmp(function.c_str(), "regelLed") == 0) {
        currentApartment->bed->regelLed(vals, fd);
    } else if (strcmp(function.c_str(), "hulpNodig") == 0) {
        currentApartment->bed->hulpNodig(vals, fd);
    } else if(strcmp(function.c_str(), "afwezigheidBed") == 0) {
        currentApartment->bed->afwezigheidBed(vals, fd);
    } 
}

void Functions::venster(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "ldrRead") == 0) {
        currentApartment->venster->ldrRead(vals, fd);
    } else if (strcmp(function.c_str(), "potRead") == 0) {
        currentApartment->venster->potRead(vals, fd);
    } else if (strcmp(function.c_str(), "regelLcdPanel") == 0) {
        currentApartment->venster->regelLcdPanel(vals, fd);
    } else if (strcmp(function.c_str(), "regelVenster") == 0) {
        currentApartment->venster->regelVenster(vals, fd);
    } 
}

void Functions::stoel(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "trillenAan") == 0) {
        currentApartment->stoel->trillenAan(vals, fd);
    } else if (strcmp(function.c_str(), "trillenUit") == 0) {
        currentApartment->stoel->trillenUit(vals, fd);
    } else if(strcmp(function.c_str(), "ledAan") == 0) {
        currentApartment->stoel->ledAan(vals, fd);
    } else if(strcmp(function.c_str(), "stoelTrilCyclus") == 0) {
        currentApartment->stoel->stoelTrilCyclus(vals, fd);
    } else if(strcmp(function.c_str(), "stoelKnopTril") == 0) {
        currentApartment->stoel->stoelKnopTril(vals, fd);
    } 
}

void Functions::deur(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "deurOpenBinnen") == 0) {
        currentApartment->deur->deurOpenBinnen(vals, fd);
    } else if (strcmp(function.c_str(), "deurOpenBuiten") == 0) {
        currentApartment->deur->deurOpenBuiten(vals, fd);
    } else if(strcmp(function.c_str(), "brand") == 0) {
        currentApartment->deur->brand(vals, fd);
    }
}

void Functions::zuil(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    std::cout << "Zuil Actie " << function << std::endl;
    if (strcmp(function.c_str(), "deurBel") == 0) {
        currentApartment->zuil->deurBel(vals, fd);
    } else if (strcmp(function.c_str(), "zuilBrand") == 0) {
        currentApartment->zuil->zuilBrand(vals, fd);
    } else if(strcmp(function.c_str(), "geenBrand") == 0) {
        currentApartment->zuil->geenBrand(vals, fd);
    }
}

void Functions::schemerlamp(std::vector<std::string> *vals, uint16_t fd) {
    std::string function = vals->at(2);
    if (strcmp(function.c_str(), "ledOn") == 0) {
        currentApartment->schemerlamp->ledOn(vals, fd);
    } else if (strcmp(function.c_str(), "ledOff") == 0) {
        currentApartment->schemerlamp->ledOff(vals, fd);
    } else if(strcmp(function.c_str(), "brand") == 0) {
        currentApartment->schemerlamp->brand(vals, fd);
    } else if(strcmp(function.c_str(), "leesBeweging") == 0) {
        currentApartment->schemerlamp->leesBeweging(vals, fd);
    }
}

void Zuil::zuilBrand(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->brandP->publish("8");
    const char *message = "8";
    send(this->fd, message, strlen(message), 0);
}

void Bed::regelLed(std::vector<std::string> *vals, uint16_t fd) {
    if (statusLed) {
        const char *message = "2"; 
        send(fd, message, strlen(message), 0);
        statusLed = false;
        return;
       }
        if (!statusLed) {
            const char* message = "1"; 
            send(fd, message, strlen(message), 0);
            statusLed = true;
        return;
       }
}

void Bed::hulpNodig(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->patientHulpP->publish("4");
    const char *message = "3";
    send(fd, message, strlen(message), 0);
    hulp = !hulp;
}

void Bed::afwezigheidBed(std::vector<std::string> *vals, uint16_t fd) {
    std::cout << "Mary is uit bed gevallen"  << std::endl;
}

void Functions::sendMessage(uint16_t fd, const char* message) {
    send(fd, message, strlen(message), 0);
}
