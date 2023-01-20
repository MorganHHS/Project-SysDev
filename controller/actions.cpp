#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include "actions.hpp"

Apartment mary;
Apartment *currentApartment = &mary;

Venster::Venster(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    mary.venster = this;
    mary.brandP->subscribe(subscriber);
    mary.geenBrandP->subscribe(subscriber);
}

Stoel::Stoel(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    mary.stoel = this;
    mary.brandP->subscribe(subscriber);
    mary.geenBrandP->subscribe(subscriber);
}

Bed::Bed(uint16_t fileDescriptor) { 
    printf("Bed Created\n");
    fd = fileDescriptor;
    subscriber = new Subscriber;
    mary.bed = this;
    mary.brandP->subscribe(subscriber);
    mary.geenBrandP->subscribe(subscriber);
}

Deur::Deur(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    mary.deur = this;
    mary.brandP->subscribe(subscriber);
    mary.geenBrandP->subscribe(subscriber);
}

Zuil::Zuil(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    mary.zuil = this;
}

Schemerlamp::Schemerlamp(uint16_t fileDescriptor) {
    fd = fileDescriptor;
    subscriber = new Subscriber;
    mary.schemerlamp = this;
    mary.brandP->subscribe(subscriber);
    mary.geenBrandP->subscribe(subscriber);
}

Apartment::Apartment() {
    bed = new Bed(0);
    brandP = new Publisher;
    geenBrandP = new Publisher;
}

void Functions::bed(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->bed = new Bed(fd);
}

void Functions::venster(std::vector<std::string> *vals, uint16_t fd) {
    currentApartment->venster = new Venster(fd);
}
void Functions::stoel(std::vector<std::string> *vals, uint16_t fd) {
    Stoel stoel(fd);
}
