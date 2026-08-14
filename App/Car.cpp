#include "Car.h"
#include <glut.h>

// ---------- Helper ----------
void drawBox(float w, float h, float d) {
    glPushMatrix();
    glScalef(w, h, d);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// ---------- EXTERIOR ----------
void Car::DrawExterior() {
    glPushMatrix();

    // ================= BODY =================
    glColor3f(0.75f, 0.0f, 0.0f); // Red

    // Main body
    glPushMatrix();
    glTranslatef(0, 1.0f, 0);
    drawBox(4.6f, 1.0f, 9.0f);
    glPopMatrix();

    // Hood (slanted)
    glPushMatrix();
    glTranslatef(0, 1.2f, -3.2f);
    glRotatef(-8, 1, 0, 0);
    drawBox(4.2f, 0.6f, 2.5f);
    glPopMatrix();

    // Roof
    glPushMatrix();
    glTranslatef(0, 1.9f, 0.3f);
    drawBox(3.2f, 0.9f, 4.0f);
    glPopMatrix();

    // ================= WINDOWS =================
    glColor3f(0.05f, 0.05f, 0.05f); // Black glass

    // Front windshield
    glPushMatrix();
    glTranslatef(0, 2.0f, -1.8f);
    glRotatef(-25, 1, 0, 0);
    drawBox(3.0f, 0.6f, 0.1f);
    glPopMatrix();

    // Side windows
    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix();
        glTranslatef(1.65f * side, 1.9f, 0.3f);
        drawBox(0.1f, 0.7f, 3.5f);
        glPopMatrix();
    }

    // ================= BUMPER =================
    glColor3f(0.08f, 0.08f, 0.08f);
    glPushMatrix();
    glTranslatef(0, 0.7f, -4.7f);
    drawBox(4.8f, 0.5f, 0.8f);
    glPopMatrix();

    // ================= HEADLIGHTS =================
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int side = -1; side <= 1; side += 2) {
        glPushMatrix();
        glTranslatef(1.3f * side, 0.95f, -4.75f);
        drawBox(0.6f, 0.4f, 0.1f);
        glPopMatrix();
    }

    // ================= WHEELS =================
    glColor3f(0.15f, 0.15f, 0.15f);

    float wx[2] = { -2.1f, 2.1f };
    float wz[2] = { -3.0f, 3.0f };

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            glPushMatrix();
            glTranslatef(wx[i], 0.4f, wz[j]);
            glRotatef(90, 0, 1, 0);
            glutSolidTorus(0.15, 0.55, 16, 32);
            glPopMatrix();
        }
    }

    glPopMatrix();
}

void drawFixedCar() {
    // Car Material - Shiny Blue
    GLfloat mat_ambient[] = { 0.2f, 0.2f, 0.6f, 1.0f };
    GLfloat mat_diffuse[] = { 0.6f, 0.6f, 0.9f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
    // Scale slightly to fit your city scale
    glScalef(0.5f, 0.5f, 0.5f);

    glBegin(GL_QUADS);
    // HOOD
    glNormal3f(0.0f, 0.5f, -0.8f); // Angled normal for lighting
    glVertex3f(0.8f, 0.6f, -1.8f); glVertex3f(-0.8f, 0.6f, -1.8f);
    glVertex3f(-0.7f, 0.6f, -0.5f); glVertex3f(0.7f, 0.6f, -0.5f);

    // ROOF
    glNormal3f(0.0f, 1.0f, 0.0f); // Pointing straight up
    glVertex3f(0.6f, 1.3f, -0.2f); glVertex3f(-0.6f, 1.3f, -0.2f);
    glVertex3f(-0.6f, 1.3f, 1.2f); glVertex3f(0.6f, 1.3f, 1.2f);

    // WINDSHIELD (Darker)
    glColor3f(0.1f, 0.1f, 0.1f);
    glNormal3f(0.0f, 0.7f, -0.7f);
    glVertex3f(0.7f, 0.6f, -0.5f); glVertex3f(-0.7f, 0.6f, -0.5f);
    glVertex3f(-0.6f, 1.3f, -0.2f); glVertex3f(0.6f, 1.3f, -0.2f);

    // Reset color for body
    glColor3f(0.2f, 0.2f, 0.8f);

    // LEFT SIDE BODY
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.8f, 0.0f, -2.0f); glVertex3f(-0.9f, 0.0f, 2.0f);
    glVertex3f(-0.9f, 0.8f, 1.9f); glVertex3f(-0.8f, 0.6f, -1.8f);

    // RIGHT SIDE BODY
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.8f, 0.0f, -2.0f); glVertex3f(0.8f, 0.6f, -1.8f);
    glVertex3f(0.9f, 0.8f, 1.9f); glVertex3f(0.9f, 0.0f, 2.0f);

    // FRONT BUMPER
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.8f, 0.0f, -2.0f); glVertex3f(-0.8f, 0.0f, -2.0f);
    glVertex3f(-0.8f, 0.6f, -1.8f); glVertex3f(0.8f, 0.6f, -1.8f);

    // REAR BUMPER
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-0.9f, 0.0f, 2.0f); glVertex3f(0.9f, 0.0f, 2.0f);
    glVertex3f(0.9f, 0.8f, 1.9f); glVertex3f(-0.9f, 0.8f, 1.9f);
    glEnd();

    // Draw Wheels (Simple GLUT Torus or Cylinder)
    glColor3f(0.1f, 0.1f, 0.1f);

    // Front Left Wheel
    glPushMatrix();
    glTranslatef(-0.8f, 0.3f, -1.5f);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.1, 0.3, 10, 10);
    glPopMatrix();

    // Front Right Wheel
    glPushMatrix();
    glTranslatef(0.8f, 0.3f, -1.5f);
    glRotatef(90, 0, 1, 0);
    glutSolidTorus(0.1, 0.3, 10, 10);
    glPopMatrix();

    // ... Repeat for rear wheels ...

    glPopMatrix();
}

