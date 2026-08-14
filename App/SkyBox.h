#pragma once
#include <glut.h>
#include "Texture.h"

class Skybox {
private:
    Texture skyTex;
    float radius;

public:
    Skybox(float r) {
        radius = r;
    }

    // Load just one texture for the whole sphere
    void LoadTexture(const char* fname) {
        skyTex.Load(fname);
    }

    // Helper to keep your existing init() code working (ignores extra params)
    void LoadTextures(const char* f1, const char* f2, const char* f3, const char* f4, const char* f5, const char* f6) {
        // We only use the first image for the sphere
        skyTex.Load(f1);
    }

    void Draw(float camX, float camY, float camZ) {
        glPushMatrix();

        // 1. Move Skybox with Camera so you never reach the edge
        glTranslatef(camX, camY, camZ);

        // 2. Disable Lighting/Depth so sky is always bright and behind everything
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);

        skyTex.Use();
        glColor3f(1.0f, 1.0f, 1.0f);

        // 3. Fix Rotation (Spheres are often drawn sideways by default)
        glRotatef(90, 1, 0, 0);

        // 4. Draw the Sphere
        GLUquadric* q = gluNewQuadric();
        gluQuadricTexture(q, GL_TRUE);      // Enable Texture Coords
        gluQuadricOrientation(q, GLU_INSIDE); // Draw texture on INSIDE of sphere

        // Radius, Slices, Stacks (Higher numbers = smoother sphere)
        gluSphere(q, radius, 64, 64);

        gluDeleteQuadric(q);

        // 5. Restore settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glPopMatrix();
    }
};