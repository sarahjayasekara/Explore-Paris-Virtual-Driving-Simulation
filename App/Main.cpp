#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <math.h>
#include <glut.h>
#include <stdio.h>
#include <string>
#include "CityMap.h"
#include "Car.h"
#include "Skybox.h"


// ---------------- Configuration ----------------
int win_width = 800, win_height = 600;
static int lastTime = 0;

// ---------------- Objects ----------------
CityMap* parisMap;
Car myCar;
Skybox* mySky;

// ---------------- State ----------------
bool fpsMode = false;
bool showGrid = false;

// CAMERA MODES
// 0 = First Person (Driver)
// 1 = Third Person (Chase)
// 2 = Top Down
int cameraMode = 1;
float camFov = 60.0f;

// ---------------- Car Physics ----------------
float carX = 100.0f, carY = 1.0f, carZ = 100.0f;
float carYaw = 0.0f;
float carSpeed = 0.0f;

float carMaxSpeed = 10.0f;
float carAccel = 10.0f;
float carTurnSpeed =50.0f;

// ---------------- FPS Builder ----------------
float flyX = 100.0f, flyY = 30.0f, flyZ = 150.0f;
float flyYaw = 0.0f, flyPitch = -15.0f;

// ---------------- Input ----------------
int gKeyDown[256] = { 0 };
int gSpecialKeyDown[256] = { 0 };

#define DEG2RAD 0.0174532925f

// ---------------- Forward Decls ----------------
void update();
void drawHUD();
void reshape(int w, int h);


// ---------------- HELPER FUNCTIONS ----------------

// Helper: Get X, Z coordinates from the Grid Number (1-200)
void getGridPos(int id, float* outX, float* outZ) {
    int col = (id - 1) % 10;
    int row = (id - 1) / 10;
    *outX = (col * 20.0f) + 10.0f;
    *outZ = (row * 20.0f) + 10.0f;
}

// Helper: 3D Text (For Grid Numbers)
void renderBitmapString(float x, float y, float z, void* font, const char* s) {
    glRasterPos3f(x, y, z);
    while (*s) glutBitmapCharacter(font, *s++);
}

// Helper: 2D Text (For HUD Overlay)
void renderHUDString(float x, float y, const char* s, float r, float g, float b) {
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glColor3f(r, g, b);
    glRasterPos2f(x, y);
    while (*s) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *s++);
    glEnable(GL_LIGHTING);
}
// ---------------- DEBUG GRID ----------------
void drawGrid() {
    if (!showGrid) return;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    glLineWidth(1.0f);

    // 1. Draw Lines
    glBegin(GL_LINES);
    glColor3f(0.5f, 0.5f, 0.5f); // Grey lines
    // Vertical (X)
    for (int i = 0; i <= 10; i++) {
        float x = i * 20.0f;
        glVertex3f(x, 0.1f, 0.0f); glVertex3f(x, 0.1f, 400.0f);
    }
    // Horizontal (Z)
    for (int i = 0; i <= 20; i++) {
        float z = i * 20.0f;
        glVertex3f(0.0f, 0.1f, z); glVertex3f(200.0f, 0.1f, z);
    }
    glEnd();

    // 2. Draw Numbers (1 to 200)
    char buffer[10];
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow Numbers

    for (int i = 1; i <= 200; i++) {
        float x, z;
        getGridPos(i, &x, &z);
        sprintf_s(buffer, "%d", i);
        renderBitmapString(x - 5.0f, 0.5f, z + 5.0f, GLUT_BITMAP_HELVETICA_12, buffer);
    }

    glEnable(GL_LIGHTING);
}
// ---------------- INIT ----------------
void init() {
    glClearColor(0.53f, 0.81f, 0.92f, 1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    GLfloat lightPos[] = { 100,200,100,0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    parisMap = new CityMap(20.0f);
    parisMap->loadTextures();

    mySky = new Skybox(500.0f);
    mySky->LoadTextures(
        "../Assets/textures/sky33.jpg", // Front
        "../Assets/textures/sky33.jpg", // Back
        "../Assets/textures/sky33.jpg", // Left
        "../Assets/textures/sky33.jpg", // Right
        "../Assets/textures/sky33.jpg", // Top
        "../Assets/textures/sky33.jpg"  // Bottom
    );
}

// ---------------- INPUT ----------------
void keyboardDown(unsigned char key, int, int) {
    gKeyDown[key] = 1;

    if (key == 27) exit(0);

    if (key == 'g' || key == 'G')
        showGrid = !showGrid;

    if (key == 'f' || key == 'F') {
        fpsMode = !fpsMode;
        if (!fpsMode) {
            carX = flyX;
            carZ = flyZ;
            carYaw = flyYaw;
        }
    }

    // CAMERA SWITCH
    if (key == 'c' || key == 'C') {
        cameraMode = (cameraMode + 1) % 3;
    }

    // ZOOM
    if (key == 'z' || key == 'Z') {
        camFov -= 2;
        if (camFov < 20.0f) camFov = 20.0f; // Limit minimum zoom
    }
    if (key == 'x' || key == 'X') {
        camFov += 2;
        if (camFov > 120.0f) camFov = 120.0f; // Limit maximum zoom
    }

 
    reshape(win_width, win_height);
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int, int) {
    gKeyDown[key] = 0;
}

void specialDown(int key, int, int) {
    gSpecialKeyDown[key] = 1;
}

void specialUp(int key, int, int) {
    gSpecialKeyDown[key] = 0;
}

// ---------------- UPDATE ----------------
void update() {
    // --- 1. Time Calculation ---
    DWORD currentTime = GetTickCount();
    if (lastTime == 0) lastTime = currentTime;

    // Calculate deltaTime (dt) in seconds
    float dt = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;

    // --- 2. Update GlobalTime ---
    // GlobalTime is declared static in CityMap.h, so we must access it directly 
  
    extern float GlobalTime; // Declare it as external if defined in another file
    GlobalTime += dt;
    

    if (!fpsMode) {
        // ACCELERATION
        if (gKeyDown['w'] || gKeyDown['W'])
            carSpeed += carAccel * dt;
        if (gKeyDown['s'] || gKeyDown['S'])
            carSpeed -= carAccel * dt;

        // STEERING
        if (gKeyDown['a'] || gKeyDown['A'])
            carYaw -= carTurnSpeed * dt;
        if (gKeyDown['d'] || gKeyDown['D'])
            carYaw += carTurnSpeed * dt;

        // LIMIT SPEED
        if (carSpeed > carMaxSpeed) carSpeed = carMaxSpeed;
        if (carSpeed < -carMaxSpeed) carSpeed = -carMaxSpeed;

        // MOVE CAR
        float rad = carYaw * DEG2RAD;
        carX += sinf(rad) * carSpeed * dt;
        carZ -= cosf(rad) * carSpeed * dt;

        // FRICTION
        carSpeed *= 0.98f;
        glutPostRedisplay();
    }
}
void renderBitmapString(float x, float y, void* font, const char* string) {
    glRasterPos2f(x, y);
    while (*string) {
        glutBitmapCharacter(font, *string++);
    }
}

// ---------------- HUD ----------------
void drawHUD() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, win_width, win_height, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_DEPTH_TEST);

    glColor3f(1,1,0);
    glRasterPos2f(10,20);
    const char* title = "Explore Paris - Driving Simulation";
    while (*title) glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *title++);
   
    glColor3f(0.0f, 0.0f, 0.0f); // White Text
    renderBitmapString(10, 45, GLUT_BITMAP_8_BY_13, "Controls:");
    renderBitmapString(20, 65, GLUT_BITMAP_8_BY_13, "[W ]    Accelerate");
    renderBitmapString(20, 80, GLUT_BITMAP_8_BY_13, "[S]  Brake / Reverse");
    renderBitmapString(20, 95, GLUT_BITMAP_8_BY_13, "[A]     Turn Left/Right");
    renderBitmapString(20, 130, GLUT_BITMAP_9_BY_15, "[C]         Switch Camera Mode");


    glEnable(GL_DEPTH_TEST);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

// ---------------- DISPLAY ----------------
void display() {
    update();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float rad = carYaw * DEG2RAD;

    // 🎥 CAMERA SETUP
    if (!fpsMode) {
        if (cameraMode == 0) {
            // FIRST PERSON 
            float camY = carY + 1.7f;
            gluLookAt(
                carX, camY, carZ,
                carX + sinf(rad), camY, carZ - cosf(rad),
                0,1,0
            );
        }
        else if (cameraMode == 1) {
            // THIRD PERSON
            gluLookAt(
                carX - sinf(rad) * 15, carY + 8, carZ + cosf(rad) * 15,
                carX, carY + 2, carZ,
                0,1,0
            );
        }
        else {
            // TOP DOWN
            gluLookAt(
                carX, carY + 80, carZ,
                carX, carY, carZ,
                0,0,-1
            );
        }
    }
    if (mySky) {
        // Passing the current camera position so the sky moves with us
        if (fpsMode) {
            mySky->Draw(flyX, flyY, flyZ);
        }
        else {
            // If in car mode, center sky on car
            mySky->Draw(carX, carY, carZ);
        }
    }
    // DRAW MAP
    if (parisMap) parisMap->Draw();

    // DRAW CAR
    if (!fpsMode) {
        glPushMatrix();
        glTranslatef(carX, carY, carZ);
        glRotatef(carYaw, 0,1,0);
        if (cameraMode != 0)
            myCar.DrawExterior();
        glPopMatrix();
    }

    drawHUD();
    glutSwapBuffers();
    glutPostRedisplay();
}

// ---------------- RESHAPE ----------------
void reshape(int w, int h) {
    if (h == 0) h = 1;
    win_width = w;
    win_height = h;

    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(camFov, (float)w/h, 0.1f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

// ---------------- MAIN ----------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(win_width, win_height);
    glutCreateWindow("Explore Paris");

    init();

    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutSpecialFunc(specialDown);
    glutSpecialUpFunc(specialUp);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
    return 0;
}
