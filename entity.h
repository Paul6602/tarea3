#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

class Entity {
    std::string name;
    int x, y;
    float energy;
public:
    Entity(std::string n, int startX, int startY)
        : name(n), x(startX), y(startY), energy(100.0) {}

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }

    void heal(float valor) {
        energy += valor;
        if (energy > 100.0)
            energy = 100;
    }

    void damage(float valor) {
        energy -= valor;
        if (energy < 0.0)
            energy = 0.0;
    }

    void reset() {
        x = 0;
        y = 0;
        energy = 100.0;
    }

    void printStatus() {
        std::cout<<"entity: "<< name << "" << getStatus() << std::endl;
    }

    std::string getStatus() {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << energy;

        return " Posicion:(" + std::to_string(x) + "," + std::to_string(y) +
               ") Energia:" + oss.str();
    }
};
#endif