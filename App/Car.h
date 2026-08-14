#pragma once
#include <glut.h>

class Car {
public:
    void DrawExterior();
    void DrawInterior();
    void DrawSteeringWheel(float steerAngle);
};
