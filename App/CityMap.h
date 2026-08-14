#pragma once

#include <glut.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <cmath> 

#include "Texture.h"
#include "Cube.h"
#include "Eiffel.h"

// Tile Types
#define TILE_GRASS 0
#define TILE_ROAD 1
#define TILE_BUILDING 3

// --- GLOBAL STATE ---
static float GlobalTime = 0.0f;

class CityMap {
private:
    float tileSize;
    int mapWidth, mapHeight;
    std::vector<std::vector<int>> cityLayout;

    // --- GRID NUMBERING STATE ---
    bool showGridNumbers = false;

    // Resources
    Texture texGrass, texRoad, texBuilding1, texBuilding2;
    Texture texEiffel;
    Texture texTrunk;
    Cube buildingBlock;
    EiffelTower myEiffel;

    // Function to render text for grid numbering
    void renderText(float x, float y, float z, const char* text) {
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.0f, 0.0f, 0.0f); // Black color for text
        glRasterPos3f(x, y, z);
        while (*text) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *text++);
        }
        glColor3f(1.0f, 1.0f, 1.0f);
        glEnable(GL_TEXTURE_2D);
    }

    // --- Procedural Tree Function (Existing) ---
    void drawProceduralTree(float x, float z) {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        glEnable(GL_TEXTURE_2D);

        // Trunk
        texTrunk.Use();
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        GLUquadric* trunkQuad = gluNewQuadric();
        gluQuadricTexture(trunkQuad, GL_TRUE);
        gluCylinder(trunkQuad, 0.8f, 0.6f, 10.0f, 12, 5);
        gluDeleteQuadric(trunkQuad);
        glPopMatrix();

        // Leaves 
        glDisable(GL_TEXTURE_2D);
        glColor3f(0, 1.0f, 0);
        glPushMatrix();
        glTranslatef(0.0f, 12.0f, 0.0f);
        glScalef(4.0f, 4.0f, 4.0f);
        glutSolidDodecahedron();
        glPopMatrix();
        glPushMatrix(); glTranslatef(3.0f, 9.0f, 0.0f); glScalef(2.5f, 2.5f, 2.5f); glutSolidDodecahedron(); glPopMatrix();
        glPushMatrix(); glTranslatef(-3.0f, 10.0f, 2.0f); glScalef(2.8f, 2.8f, 2.8f); glutSolidDodecahedron(); glPopMatrix();
        glPushMatrix(); glTranslatef(0.0f, 11.0f, -3.0f); glScalef(2.2f, 2.2f, 2.2f); glutSolidDodecahedron(); glPopMatrix();
        glPushMatrix(); glTranslatef(2.0f, 13.0f, 2.0f); glScalef(2.0f, 2.0f, 2.0f); glutSolidDodecahedron(); glPopMatrix();
        glPushMatrix(); glTranslatef(-2.0f, 12.5f, -2.0f); glScalef(2.3f, 2.3f, 2.3f); glutSolidDodecahedron(); glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }

    // --- NEW: Traffic Light Drawing Logic 
    void drawTrafficLight(float x, float z, float rotationY) {
        glPushMatrix();
        glTranslatef(x, 0.0f, z);
        glRotatef(rotationY, 0.0f, 1.0f, 0.0f);

        // 1. Draw the Pole (Simple cylinder)
        glColor3f(0.3f, 0.3f, 0.3f); // Dark gray pole
        glDisable(GL_TEXTURE_2D);

        glPushMatrix();
        // Base of cylinder is at Y=0. Cylinder length is 10.0f.
        glTranslatef(0.0f, 0.0f, -0.1f);
        glRotatef(-90, 1, 0, 0);
        GLUquadric* poleQuad = gluNewQuadric();
        gluCylinder(poleQuad, 0.2f, 0.2f, 10.0f, 8, 2);
        gluDeleteQuadric(poleQuad);
        glPopMatrix();

        // 2. Draw the Light Housing (Simple black box)
        glColor3f(0.1f, 0.1f, 0.1f); // Black housing
        glPushMatrix();
        // Positioned above the pole (pole is 10.0f high)
        glTranslatef(0.0f, 10.5f, 0.1f);
        glScalef(1.0f, 3.0f, 0.5f);
        glutSolidCube(1.0f);
        glPopMatrix();

        // 3. Draw the Lights (Bulbs)
        // NEW Traffic light cycle (10.0 seconds total)
        // Red (5s): 0.0-5.0 | Green (3s): 5.0-8.0 | Yellow (2s): 8.0-10.0
        float timeInCycle = fmod(GlobalTime, 10.0f); // Total cycle reduced to 10 seconds
        float bulbRadius = 0.35f;

        // --- Red Light (Top) ---
        if (timeInCycle < 5.0f) { // Red phase 0.0 to 5.0 seconds
            glColor3f(1.0f, 0.0f, 0.0f); // Bright Red
        }
        else {
            glColor3f(0.1f, 0.0f, 0.0f); // Dim Red
        }
        glPushMatrix();
        glTranslatef(0.0f, 11.5f, 0.3f);
        glutSolidSphere(bulbRadius, 10, 10);
        glPopMatrix();

        // --- Yellow Light (Middle) ---
        if (timeInCycle >= 8.0f && timeInCycle < 10.0f) { // Yellow phase 8.0 to 10.0 seconds
            glColor3f(1.0f, 1.0f, 0.0f); // Bright Yellow
        }
        else {
            glColor3f(0.1f, 0.1f, 0.0f); // Dim Yellow
        }
        glPushMatrix();
        glTranslatef(0.0f, 10.5f, 0.3f);
        glutSolidSphere(bulbRadius, 10, 10);
        glPopMatrix();

        // --- Green Light (Bottom) ---
        if (timeInCycle >= 5.0f && timeInCycle < 8.0f) { // Green phase 5.0 to 8.0 seconds
            glColor3f(0.0f, 1.0f, 0.0f); // Bright Green
        }
        else {
            glColor3f(0.0f, 0.1f, 0.0f); // Dim Green
        }
        glPushMatrix();
        glTranslatef(0.0f, 9.5f, 0.3f);
        glutSolidSphere(bulbRadius, 10, 10);
        glPopMatrix();

        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPopMatrix();
    }


public:
    CityMap(float size) {
        tileSize = size;
        initMapLayout();
    }

    // Public function to toggle the flag
    void toggleGridNumbers() {
        showGridNumbers = !showGridNumbers;
    }

    void initMapLayout() {
        cityLayout = {
            // Updated map layout with extended roads
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1,    1, 1, 1, 1},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},

            {1, 1, 1, 1, 1, 1,    1, 1, 1, 1},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1,    1, 1, 1, 1},

            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1,    1, 1, 1, 1},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},

            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {1, 1, 1, 1, 1, 1,    1, 1, 1, 1},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
            {3, 3, 1, 3, 3, 1,    0, 0, 0, 0},
        };
        mapHeight = cityLayout.size();
        mapWidth = cityLayout[0].size();
    }

    void loadTextures() {
        texGrass.Load("../Assets/textures/grass.jpg");
        texRoad.Load("../Assets/textures/road4.jpg");
        texBuilding1.Load("../Assets/textures/building22.jpg");
        texBuilding2.Load("../Assets/textures/building3.jpg");
        texTrunk.Load("../Assets/textures/trunk2.jpg");
        texEiffel.Load("../Assets/textures/eiffel4.jpg");
    }

    void drawGroundTile(float x, float y, float z) {
        float s = tileSize / 2.0f;
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(x - s, y, z - s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(x - s, y, z + s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(x + s, y, z + s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(x + s, y, z - s);
        glEnd();
    }

    void Draw() {
        glColor3f(1.0f, 1.0f, 1.0f);

        // 1. HUGE GROUND PLANE 
        texGrass.Use();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        float bigSize = 2000.0f;
        float repetitions = 100.0f;

        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-bigSize, -0.1f, -bigSize);
        glTexCoord2f(0.0f, repetitions); glVertex3f(-bigSize, -0.1f, bigSize);
        glTexCoord2f(repetitions, repetitions); glVertex3f(bigSize, -0.1f, bigSize);
        glTexCoord2f(repetitions, 0.0f); glVertex3f(bigSize, -0.1f, -bigSize);
        glEnd();

        // 2. CITY GRID
        int gridCounter = 1;

        for (int z = 0; z < mapHeight; z++) {
            for (int x = 0; x < mapWidth; x++) {
                int tileType = cityLayout[z][x];
                float worldX = (x * tileSize);
                float worldZ = (z * tileSize);

                // --- DRAW GROUND TILES ---
                if (tileType == TILE_ROAD) {
                    texRoad.Use();
                    drawGroundTile(worldX, 0.0f, worldZ);
                }

                // --- DRAW GRID NUMBERING (CONDITIONAL) ---
                if (showGridNumbers) {
                    char buffer[5];
                    // Use safer sprintf_s or snprintf
#ifdef _MSC_VER 
                    sprintf_s(buffer, 5, "%d", gridCounter);
#else
                    snprintf(buffer, 5, "%d", gridCounter);
#endif

                    renderText(worldX - (tileSize / 2.0f) + 1.0f, 0.1f, worldZ + (tileSize / 2.0f) - 1.0f, buffer);
                }

                // --- DRAW OBJECTS ---

                // Eiffel Tower 
                if (gridCounter == 100) {
                    myEiffel.SetTexture(texEiffel.textureID);
                    myEiffel.Draw(worldX, 0.0f, worldZ, 4.0f);
                }

                // BUILDINGS
                else if (tileType == TILE_BUILDING) {
                    int seed = (x * 11) + (z * 17);
                    if (seed % 2 == 0) texBuilding1.Use(); else texBuilding2.Use();

                    float heightScale = (seed % 3 == 0) ? 5.5f : 2.5f;
                    float bSize = tileSize * (0.5f + ((seed % 3) * 0.1f));

                    glPushMatrix();
                    glTranslatef(worldX, 0.0f, worldZ);
                    glScalef(1.0f, heightScale, 1.0f);
                    buildingBlock.Draw(0.0f, bSize / 2.0f, 0.0f, bSize);
                    glPopMatrix();
                }

                // TRAFFIC LIGHTS (only at specified grid numbers)
                else if (tileType == TILE_ROAD) {
                    // Check only the specified grid counters: 
                    if (gridCounter == 16 || gridCounter == 86 || gridCounter == 156) {
                        float halfTile = tileSize / 2.0f;
                        float offset = halfTile - 1.5f;

                        // Place 4 lights at the corners of the intersection tile
                        // North-East corner (Facing South: rotation 180)
                        drawTrafficLight(worldX + offset, worldZ - offset, 180.0f);
                        // South-East corner (Facing West: rotation 270)
                        drawTrafficLight(worldX + offset, worldZ + offset, 270.0f);
                        // South-West corner (Facing North: rotation 0)
                        drawTrafficLight(worldX - offset, worldZ + offset, 0.0f);
                        // North-West corner (Facing East: rotation 90)
                        drawTrafficLight(worldX - offset, worldZ - offset, 90.0f);
                    }
                }

                // TREES (on grass tiles only, 1 in 3 chance)
                else if (tileType == TILE_GRASS) {
                    // Check if the tile is within the exclusion zone (e.g., Grid 90 to Grid 120, which surrounds 100)
                    if (gridCounter < 90 || gridCounter > 120) {
                        int seed = (x * 13) + (z * 7);

                        if (gridCounter == 41 || (seed % 3 == 0)) {
                            drawProceduralTree(worldX, worldZ);
                        }
                    }
                }

                gridCounter++;
            }
        }
    }
};