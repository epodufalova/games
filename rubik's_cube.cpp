#include <GL/glut.h>
#include <iostream>
#include <algorithm>

const int CUBE_SIZE = 3;

float colors[6][3] = {
    {1.0, 0.0, 0.0},   // red(left)
    {0.0, 1.0, 0.0},   // green(right)
    {0.0, 0.0, 1.0},   // blue(bottom)
    {1.0, 1.0, 0.0},   // yellow(top)
    {1.0, 0.5, 0.0},   // orange(back)
    {1.0, 1.0, 1.0}    // white(front)
};

struct Cubelet {
    int x, y, z;
    int faceColors[6]; // 0 left, 1 right, 2 bottom, 3 top, 4 back, 5 front

    void rotateAroundX(bool clockwise) {
        if (clockwise)
            std::swap(faceColors[2], faceColors[5]),
            std::swap(faceColors[5], faceColors[3]),
            std::swap(faceColors[3], faceColors[4]);
        else
            std::swap(faceColors[2], faceColors[4]),
            std::swap(faceColors[4], faceColors[3]),
            std::swap(faceColors[3], faceColors[5]);
    }

    void rotateAroundY(bool clockwise) {
        if (clockwise)
            std::swap(faceColors[0], faceColors[4]),
            std::swap(faceColors[4], faceColors[1]),
            std::swap(faceColors[1], faceColors[5]);
        else
            std::swap(faceColors[0], faceColors[5]),
            std::swap(faceColors[5], faceColors[1]),
            std::swap(faceColors[1], faceColors[4]);
    }

    void rotateAroundZ(bool clockwise) {
        if (clockwise)
            std::swap(faceColors[0], faceColors[2]),
            std::swap(faceColors[2], faceColors[1]),
            std::swap(faceColors[1], faceColors[3]);
        else
            std::swap(faceColors[0], faceColors[3]),
            std::swap(faceColors[3], faceColors[1]),
            std::swap(faceColors[1], faceColors[2]);
    }
};

Cubelet cube[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];
float angleX = 20.0f, angleY = 30.0f;

void initCube() {
    for (int x = 0; x < CUBE_SIZE; ++x)
        for (int y = 0; y < CUBE_SIZE; ++y)
            for (int z = 0; z < CUBE_SIZE; ++z) {
                Cubelet& c = cube[x][y][z];
                std::fill(c.faceColors, c.faceColors + 6, -1);
                if (x == 0) c.faceColors[0] = 0;
                if (x == CUBE_SIZE - 1) c.faceColors[1] = 1;
                if (y == 0) c.faceColors[2] = 2;
                if (y == CUBE_SIZE - 1) c.faceColors[3] = 3;
                if (z == 0) c.faceColors[4] = 4;
                if (z == CUBE_SIZE - 1) c.faceColors[5] = 5;
                c.x = x; c.y = y; c.z = z;
            }
}

void drawCubelet(int x, int y, int z) {
    float size = 0.9f;
    float offset = 1.0f;
    Cubelet& c = cube[x][y][z];

    glPushMatrix();
    glTranslatef((x - 1) * offset, (y - 1) * offset, (z - 1) * offset);
    float s = size / 2.0f;

    for (int i = 0; i < 6; ++i) {
        if (c.faceColors[i] != -1) {
            glColor3fv(colors[c.faceColors[i]]);
        }
        else {
            glColor3f(0.1f, 0.1f, 0.1f); 
        }
        glBegin(GL_QUADS);
        switch (i) {
        case 0: glVertex3f(-s, -s, -s); glVertex3f(-s, -s, s); glVertex3f(-s, s, s); glVertex3f(-s, s, -s); break;
        case 1: glVertex3f(s, -s, -s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(s, s, -s); break;
        case 2: glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s); glVertex3f(s, -s, s); glVertex3f(-s, -s, s); break;
        case 3: glVertex3f(-s, s, -s); glVertex3f(s, s, -s); glVertex3f(s, s, s); glVertex3f(-s, s, s); break;
        case 4: glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s); glVertex3f(s, s, -s); glVertex3f(-s, s, -s); break;
        case 5: glVertex3f(-s, -s, s); glVertex3f(s, -s, s); glVertex3f(s, s, s); glVertex3f(-s, s, s); break;
        }
        glEnd();
    }

    glColor3f(0.0f, 0.0f, 0.0f);
    glLineWidth(2.0);
    glBegin(GL_LINES);

    glVertex3f(-s, -s, -s); glVertex3f(s, -s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, -s, s);
    glVertex3f(s, -s, s); glVertex3f(-s, -s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, -s, -s);
 
    glVertex3f(-s, s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, s, -s); glVertex3f(s, s, s);
    glVertex3f(s, s, s); glVertex3f(-s, s, s);
    glVertex3f(-s, s, s); glVertex3f(-s, s, -s);
    
    glVertex3f(-s, -s, -s); glVertex3f(-s, s, -s);
    glVertex3f(s, -s, -s); glVertex3f(s, s, -s);
    glVertex3f(s, -s, s); glVertex3f(s, s, s);
    glVertex3f(-s, -s, s); glVertex3f(-s, s, s);
    glEnd();
    glPopMatrix();
}

void drawCube() {
    for (int x = 0; x < CUBE_SIZE; ++x)
        for (int y = 0; y < CUBE_SIZE; ++y)
            for (int z = 0; z < CUBE_SIZE; ++z)
                drawCubelet(x, y, z);
}

void rotateLayerX(int layer, bool cw) {
    Cubelet tmp[3][3];
    for (int y = 0; y < 3; ++y)
        for (int z = 0; z < 3; ++z) {
            tmp[y][z] = cube[layer][y][z];
            tmp[y][z].rotateAroundX(cw);
        }
    if (cw) {
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
                cube[layer][y][z] = tmp[z][2 - y];
    }
    else {
        for (int y = 0; y < 3; ++y)
            for (int z = 0; z < 3; ++z)
                cube[layer][y][z] = tmp[2 - z][y];
    }
}

void rotateLayerY(int layer, bool cw) {
    Cubelet tmp[3][3];
    for (int x = 0; x < 3; ++x)
        for (int z = 0; z < 3; ++z) {
            tmp[x][z] = cube[x][layer][z];
            tmp[x][z].rotateAroundY(cw);
        }
    for (int x = 0; x < 3; ++x)
        for (int z = 0; z < 3; ++z) {
            cube[x][layer][z] = cw ? tmp[2 - z][x] : tmp[z][2 - x];
        }
}

void rotateLayerZ(int layer, bool cw) {
    Cubelet tmp[3][3];
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y) {
            tmp[x][y] = cube[x][y][layer];
            tmp[x][y].rotateAroundZ(cw);
        }
    for (int x = 0; x < 3; ++x)
        for (int y = 0; y < 3; ++y) {
            cube[x][y][layer] = cw ? tmp[2 - y][x] : tmp[y][2 - x];
        }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.0, 0.0, -10.0);
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    drawCube();
    glutSwapBuffers();
}

void keyboard(unsigned char key, int, int) {
    switch (key) {
    case '1': rotateLayerX(0, true); break;
    case '2': rotateLayerX(1, true); break;
    case '3': rotateLayerX(2, true); break;
    case '4': rotateLayerX(0, false); break;
    case '5': rotateLayerX(1, false); break;
    case '6': rotateLayerX(2, false); break;
    case 'q': rotateLayerY(0, true); break;
    case 'w': rotateLayerY(1, true); break;
    case 'e': rotateLayerY(2, true); break;
    case 'a': rotateLayerY(0, false); break;
    case 's': rotateLayerY(1, false); break;
    case 'd': rotateLayerY(2, false); break;
    case 'z': rotateLayerZ(0, true); break;
    case 'x': rotateLayerZ(1, true); break;
    case 'c': rotateLayerZ(2, true); break;
    case 'v': rotateLayerZ(0, false); break;
    case 'b': rotateLayerZ(1, false); break;
    case 'n': rotateLayerZ(2, false); break;
    }
    glutPostRedisplay();
}

void specialKeys(int key, int, int) {
    if (key == GLUT_KEY_LEFT) angleY -= 5;
    if (key == GLUT_KEY_RIGHT) angleY += 5;
    if (key == GLUT_KEY_UP) angleX -= 5;
    if (key == GLUT_KEY_DOWN) angleX += 5;
    glutPostRedisplay();
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Rubik's Cube");
    initOpenGL();
    initCube();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    std::cout << "\nControls:\n";
    std::cout << "Arrow Keys: Rotate the entire cube view\n";
    std::cout << "X Axis Rotation: 1/2/3 (forward), 4/5/6 (back)\n";
    std::cout << "Y Axis Rotation: q/w/e (forward), a/s/d (back)\n";
    std::cout << "Z Axis Rotation: z/x/c (forward), v/b/n (back)\n";

    glutMainLoop();
    return 0;
}
