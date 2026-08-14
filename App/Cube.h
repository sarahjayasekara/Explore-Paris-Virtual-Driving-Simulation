#pragma once
#include <glut.h> 
class Cube {
public:
    // x, y, z = Center Position; size = Width of the cube
    void Draw(float x, float y, float z, float size) {
        float s = size / 2.0f; // Half-size to draw from center

        glPushMatrix();
        glTranslatef(x, y, z); // Move the cube to its spot

        glBegin(GL_QUADS);
        // --- FRONT FACE (Textured) ---
        glNormal3f(0.0f, 0.0f, 1.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(s, -s, s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, s);

        // --- BACK FACE (Textured) ---
        glNormal3f(0.0f, 0.0f, -1.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(s, s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, -s);

        // --- TOP FACE (PLAIN GRAY - NO TEXTURE) ---
        // 1. End the current texture batch
        glEnd();

        // 2. Disable Texture & Set Color to Gray
        glDisable(GL_TEXTURE_2D);
        glColor3f(0.5f, 0.5f, 0.5f); // Gray Color

        // 3. Draw Top Face
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-s, s, -s);
        glVertex3f(-s, s, s);
        glVertex3f(s, s, s);
        glVertex3f(s, s, -s);
        glEnd();

        // 4. Re-enable Texture & Reset Color to White for the rest
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f, 1.0f, 1.0f);
        glBegin(GL_QUADS);
        // ------------------------------------------

        // --- BOTTOM FACE (Textured) ---
        glNormal3f(0.0f, -1.0f, 0.0f);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(s, -s, -s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, s);

        // --- RIGHT FACE (Textured) ---
        glNormal3f(1.0f, 0.0f, 0.0f);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(s, -s, -s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(s, s, -s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(s, s, s);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(s, -s, s);

        // --- LEFT FACE (Textured) ---
        glNormal3f(-1.0f, 0.0f, 0.0f);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(-s, -s, -s);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(-s, -s, s);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(-s, s, s);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(-s, s, -s);
        glEnd();

        glPopMatrix();
    }
};