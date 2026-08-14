#pragma once

#include <glut.h>
#include <cmath>
#include <cstdlib>

// --- Helper Functions from Bicycle (Modified for Motorbike) ---

// Modified to be thicker and without spokes (like modern motorcycle wheels)
void drawMotorbikeWheel() {
    // Tire
    glColor3f(0.1, 0.1, 0.1); // Dark Gray/Black
    glutSolidTorus(0.3, 1.8, 50, 50); // Thicker tire

    // Solid Hub (No spokes needed for a modern look)
    glPushMatrix();
    glColor3f(0.5, 0.5, 0.5); // Metal Hub
    glScaled(0.5, 0.5, 0.5);
    glutSolidSphere(1.0, 20, 20);
    glPopMatrix();
}

// Simplified Fork/Frame for Motorbike
void drawMotorbikeFrame(float colorR, float colorG, float colorB) {
    GLUquadric* quad = gluNewQuadric();
    glColor3f(colorR, colorG, colorB);

    // Main Body/Tank (Box-like shape)
    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.5f);
    glScalef(0.6f, 1.0f, 2.0f); // Thicker body
    glutSolidCube(2.0f);
    glPopMatrix();

    // Seat
    glPushMatrix();
    glColor3f(0.1f, 0.1f, 0.1f);
    glTranslatef(0.0f, 1.3f, 1.8f);
    glScalef(0.8f, 0.2f, 1.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Front Fork connection
    glPushMatrix();
    glColor3f(0.5f, 0.5f, 0.5f);
    glTranslated(0, 1.0, -0.5);
    glRotated(110, 1, 0, 0);
    gluCylinder(quad, 0.2, 0.2, 2.5, 10, 1);
    glPopMatrix();

    // Handlebar (simplified)
    glPushMatrix();
    glColor3f(0.8f, 0.8f, 0.8f);
    glTranslated(0, 1.5, -2.5);
    glRotated(90, 0, 1, 0);
    glScalef(0.2, 0.2, 0.8);
    glutSolidCube(2.0);
    glPopMatrix();

    gluDeleteQuadric(quad);
}

// --- Motorbike Class ---
class Motorbike {
public:
    float x, z, yaw;
    float speed;
    float colorR, colorG, colorB;

    Motorbike(float startX, float startZ, float startYaw, float r, float g, float b)
        : x(startX), z(startZ), yaw(startYaw), speed(35.0f), colorR(r), colorG(g), colorB(b) {
    }

    void Draw() {
        glPushMatrix();
        glTranslatef(x, 1.8f, z); // Adjust Y to sit on the road
        glRotatef(yaw, 0, 1, 0);
        glScalef(0.8f, 0.8f, 0.8f); // Scale down

        // Frame and Body
        drawMotorbikeFrame(colorR, colorG, colorB);

        // Front Wheel
        glPushMatrix();
        glTranslatef(0, -1.0, -1.8);
        glRotatef(90, 0, 1, 0);
        drawMotorbikeWheel();
        glPopMatrix();

        // Back Wheel
        glPushMatrix();
        glTranslatef(0, -1.0, 1.8);
        glRotatef(90, 0, 1, 0);
        drawMotorbikeWheel();
        glPopMatrix();

        glPopMatrix();
    }
};


// --- Bus Drawing Logic ---
void drawBusBody(float colorR, float colorG, float colorB) {
    // Main Body
    glColor3f(colorR, colorG, colorB);
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, 0.0f);
    glScalef(10.0f, 7.0f, 3.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Windows (Side - Darker color)
    glColor3f(0.3f, 0.3f, 0.4f);
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, 1.55f);
    glScalef(9.8f, 2.5f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 4.0f, -1.55f);
    glScalef(9.8f, 2.5f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Windshield (Front)
    glColor3f(0.3f, 0.3f, 0.4f);
    glPushMatrix();
    glTranslatef(5.05f, 4.0f, 0.0f);
    glScalef(0.1f, 3.0f, 2.5f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Headlights (Front)
    glColor3f(1.0f, 1.0f, 0.8f);
    glPushMatrix();
    glTranslatef(5.1f, 2.0f, 1.0f);
    glutSolidSphere(0.2f, 10, 10);
    glTranslatef(0.0f, 0.0f, -2.0f);
    glutSolidSphere(0.2f, 10, 10);
    glPopMatrix();

    // Tail lights (Back)
    glColor3f(1.0f, 0.0f, 0.0f);
    glPushMatrix();
    glTranslatef(-5.1f, 2.0f, 1.0f);
    glutSolidCube(0.2f);
    glTranslatef(0.0f, 0.0f, -2.0f);
    glutSolidCube(0.2f);
    glPopMatrix();
}

void drawBusWheel() {
    glPushMatrix();
    // Tire
    glColor3f(0.1, 0.1, 0.1); // Dark Gray/Black
    glutSolidTorus(0.4, 1.2, 50, 50);

    // Hub
    glPushMatrix();
    glColor3f(0.6, 0.6, 0.6); // Silver
    glutSolidSphere(0.8, 20, 20);
    glPopMatrix();
    glPopMatrix();
}

// --- Bus Class ---
class Bus {
public:
    float x, z, yaw;
    float speed;
    float colorR, colorG, colorB;

    Bus(float startX, float startZ, float startYaw, float r, float g, float b)
        : x(startX), z(startZ), yaw(startYaw), speed(20.0f), colorR(r), colorG(g), colorB(b) {
    }

    void Draw() {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        glRotatef(yaw, 0, 1, 0);

        // Draw Body (centered on Z-axis, length 10)
        drawBusBody(colorR, colorG, colorB);

        // Wheels
        float wheelOffsetX = 4.0f;
        float wheelOffsetZ = 1.6f;

        // Front Left
        glPushMatrix();
        glTranslatef(wheelOffsetX, 1.2f, wheelOffsetZ);
        drawBusWheel();
        glPopMatrix();

        // Front Right
        glPushMatrix();
        glTranslatef(wheelOffsetX, 1.2f, -wheelOffsetZ);
        drawBusWheel();
        glPopMatrix();

        // Rear Left
        glPushMatrix();
        glTranslatef(-wheelOffsetX, 1.2f, wheelOffsetZ);
        drawBusWheel();
        glPopMatrix();

        // Rear Right
        glPushMatrix();
        glTranslatef(-wheelOffsetX, 1.2f, -wheelOffsetZ);
        drawBusWheel();
        glPopMatrix();

        glPopMatrix();
    }
};


// --- Vehicle Manager and Base Class ---
#define ROAD_TILE 1
#define DEG2RAD 0.0174532925f

class Vehicle {
public:
    float x, z, yaw, speed, maxSpeed;
    float sizeScale;

    Vehicle(float startX, float startZ, float startYaw, float s, float ms, float scale)
        : x(startX), z(startZ), yaw(startYaw), speed(s), maxSpeed(ms), sizeScale(scale) {
    }

    virtual void Draw() = 0;
    virtual void Update(float dt, int** cityLayout, int mapWidth, int mapHeight, float tileSize);

    // Virtual destructor is important for base classes
    virtual ~Vehicle() {}
};

void Vehicle::Update(float dt, int** cityLayout, int mapWidth, int mapHeight, float tileSize) {

    // --- Road Check ---
    int tileX = (int)floor(x / tileSize);
    int tileZ = (int)floor(z / tileSize);

    bool onRoad = false;
    if (tileX >= 0 && tileX < mapWidth && tileZ >= 0 && tileZ < mapHeight) {
        if (cityLayout[tileZ][tileX] == ROAD_TILE) {
            onRoad = true;
        }
    }

    // --- Movement Logic ---
    if (!onRoad) {
        // If off-road, force a turn. The city map ensures vehicles spawn on roads.
        yaw += 90.0f; // Snap turn
        float rad = yaw * DEG2RAD;
        // Back up slightly and re-orient (simple)
        x -= sinf(rad) * 5.0f;
        z += cosf(rad) * 5.0f;
    }

    // Simple random movement logic for traffic:
    // 1. Random slow down (friction/braking)
    speed *= 0.99f;

    // 2. Random acceleration up to maxSpeed
    if (rand() % 100 < 5) { // 5% chance per update cycle to accelerate
        speed += (float)(rand() % 10 + 1) * dt * 20.0f;
        if (speed > maxSpeed) speed = maxSpeed;
    }

    // 3. Random turning (to navigate corners and adjust lane)
    if (rand() % 100 < 3) { // 3% chance to turn slightly
        yaw += (rand() % 41) - 20; // Random angle between -20 and +20 degrees
        if (yaw > 360.0f) yaw -= 360.0f;
        if (yaw < 0.0f) yaw += 360.0f;
    }

    // --- Apply Movement ---
    float rad = yaw * DEG2RAD;
    x += sinf(rad) * speed * dt;
    z -= cosf(rad) * speed * dt;
}


class BusVehicle : public Bus, public Vehicle {
public:
    BusVehicle(float startX, float startZ, float startYaw, float r, float g, float b)
        : Bus(startX, startZ, startYaw, r, g, b), Vehicle(startX, startZ, startYaw, 20.0f, 30.0f, 1.0f) {
        // Overwrite Bus properties with Vehicle properties to keep state unified
        Bus::speed = Vehicle::speed;
        Bus::yaw = Vehicle::yaw;
        Bus::x = Vehicle::x;
        Bus::z = Vehicle::z;
    }

    void Draw() override {
        // Sync Vehicle position/rotation to Bus drawing
        Bus::x = Vehicle::x;
        Bus::z = Vehicle::z;
        Bus::yaw = Vehicle::yaw;
        Bus::Draw();
    }

    void Update(float dt, int** cityLayout, int mapWidth, int mapHeight, float tileSize) override {
        Vehicle::Update(dt, cityLayout, mapWidth, mapHeight, tileSize);
    }
};

class MotorbikeVehicle : public Motorbike, public Vehicle {
public:
    MotorbikeVehicle(float startX, float startZ, float startYaw, float r, float g, float b)
        : Motorbike(startX, startZ, startYaw, r, g, b), Vehicle(startX, startZ, startYaw, 35.0f, 60.0f, 0.8f) {
        // Overwrite Motorbike properties with Vehicle properties
        Motorbike::speed = Vehicle::speed;
        Motorbike::yaw = Vehicle::yaw;
        Motorbike::x = Vehicle::x;
        Motorbike::z = Vehicle::z;
    }

    void Draw() override {
        // Sync Vehicle position/rotation to Motorbike drawing
        Motorbike::x = Vehicle::x;
        Motorbike::z = Vehicle::z;
        Motorbike::yaw = Vehicle::yaw;
        Motorbike::Draw();
    }

    void Update(float dt, int** cityLayout, int mapWidth, int mapHeight, float tileSize) override {
        Vehicle::Update(dt, cityLayout, mapWidth, mapHeight, tileSize);
    }
};

// Simple utility function to convert vector<vector<int>> to int**
// This is necessary because the base Vehicle::Update requires a C-style array for simplicity
// in the intersection checks, and the CityMap stores it as vector<vector>.
int** getLayoutPointer(std::vector<std::vector<int>>& layout) {
    int rows = layout.size();
    int** arr = new int* [rows];
    for (int i = 0; i < rows; ++i) {
        arr[i] = layout[i].data();
    }
    return arr;
}