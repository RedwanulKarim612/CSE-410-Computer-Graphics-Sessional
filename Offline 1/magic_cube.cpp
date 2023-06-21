#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;
GLfloat triangleScale = 1;
/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(-1,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,-1,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,-1);
        glVertex3f(0,0,1);
    glEnd();
}

void drawOctHSurface(){
    glPushMatrix();
        // glTranslatef(sqrt(2.0)/4,sqrt(3)/6,sqrt(2.0)/4);  
        // glScalef(triangleScale, triangleScale,triangleScale);
        // glTranslatef(-sqrt(2.0)/4,-sqrt(3)/6,-sqrt(2.0)/4);  
         

        glTranslatef(1.0/3,1.0/3,1.0/3);  
        glScalef(triangleScale, triangleScale,triangleScale);
        glTranslatef(-1.0/3,-1.0/3,-1.0/3);  
        glBegin(GL_TRIANGLES);
            glVertex3f(1,0,0);
            glVertex3f(0,1,0);
            glVertex3f(0,0,1);
        glEnd();

    glPopMatrix();
}

void drawHalfOctH(){
    glPushMatrix();
        glColor3f(0.52, 0.16, 0.6);
        drawOctHSurface();
        glRotatef(90,0,1,0);
        glColor3f(1, 0.57, 0.01);
        drawOctHSurface();
        glRotatef(90,0,1,0);
        glColor3f(0.52, 0.16, 0.6);
        drawOctHSurface();
        glRotatef(90,0,1,0);
        glColor3f(1, 0.57, 0.01);
        drawOctHSurface();
    glPopMatrix();
}

void drawOctahedron(){
    drawHalfOctH();
    glPushMatrix();
        glRotatef(270,0,1,0);
        glRotatef(180,1,0,1);
        drawHalfOctH();
    glPopMatrix();
}

void drawCylinderUtil(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments/360.0*72; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}


void drawCylinder(){
    float thetta = acos(-1.0/3.0)/2.0;
    GLfloat d = (1-triangleScale)/sqrt(2);
    GLfloat radius = d*sin(thetta);
    GLfloat len = sqrt(2);
    glPushMatrix();
        glTranslatef(0.5,0,0.5);
        glRotatef(-45,0,1,0);
        glPushMatrix();
        glTranslatef(-d,0,0);

        glRotatef(-36, 0, 0, 1);
        drawCylinderUtil(len*triangleScale, radius, 100);
        glPopMatrix();
    glPopMatrix();
}

void drawFourCylindersOnPlane(){
    glPushMatrix();
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
    glPopMatrix();
}

void drawAllCylinders(){
    glColor3f(0.13, 0.56, 0.35);
    drawFourCylindersOnPlane();
    glPushMatrix();
        glRotatef(90,1,0,0);
        drawFourCylindersOnPlane();
    glPopMatrix();
    glPushMatrix();
        glRotatef(90,0,0,1);
        drawFourCylindersOnPlane();
    glPopMatrix();
}

void buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }
    for(int i=0;i<(vertices.size()/3)-pointsPerRow - 1;i+=1){
        glPointSize(5);
        glBegin(GL_QUADS);
            glVertex3f(vertices[3*i],vertices[3*i+1],vertices[3*i+2]);
            glVertex3f(vertices[3*i + 3],vertices[3*i+4],vertices[3*i+5]);
            glVertex3f(vertices[3*(i + pointsPerRow) + 3],vertices[3*(i + pointsPerRow) + 4],vertices[3*(i + pointsPerRow) + 5]);
            glVertex3f(vertices[3*(i + pointsPerRow)],vertices[3*(i + pointsPerRow) + 1],vertices[3*(i + pointsPerRow) + 2]);   
        glEnd();
    }
}

void drawSphereSegment(){
    float thetta = acos(-1.0/3.0)/2.0;
    GLfloat d = (1.0-triangleScale)/sqrt(2.0);
    GLfloat radius = d*sin(thetta);
    glPushMatrix();
        glTranslatef(1-sqrt(2)*d,0,0);
        glScalef(radius,radius,radius);
        buildUnitPositiveX(4);
    glPopMatrix();
}

void drawTwoSphereSegments(){
    drawSphereSegment();
    glRotatef(180,0,1,0);
    drawSphereSegment();
}

void drawAllSphereSegments(){
    glPushMatrix();
        glColor3d(1,0,0);
        drawTwoSphereSegments();
        glColor3d(0,0,1);
        glRotatef(90,0,1,0);
        drawTwoSphereSegments();
        glRotatef(90,0,0,1);
        glColor3d(0,1,0);
        drawTwoSphereSegments();
    glPopMatrix();
}


void drawCentroid(){
    glPointSize(10);
    glColor3d(1,1,1);
    glBegin(GL_POINTS);
        glVertex3f(0.3,0.3,0.3);
    glEnd();
}

void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    gluLookAt(eyex,eyey,eyez,
              centerx,centery,centerz,
              upx,upy,upz);
    // draw
    glScalef(1.5,1.5,1.5);
    if (isAxes) drawAxes();

    drawOctahedron();
    drawAllCylinders();
    drawAllSphereSegments();
    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.1;
    switch (key) {
    // Control eye (location of the eye)
    // control eyex
    case ',':
        if(triangleScale>=0) triangleScale-=0.05;
        break;
    case '.':
        if(triangleScale<1) triangleScale+=0.05;
        break;
    case '1':
        eyex += v;
        break;
    case '2':
        eyex -= v;
        break;
    // control eyey
    case '3':
        eyey += v;
        break;
    case '4':
        eyey -= v;
        break;
    // control eyez
    case '5':
        eyez += v;
        break;
    case '6':
        eyez -= v;
        break;

    // Control center (location where the eye is looking at)
    // control centerx
    case 'q':
        centerx += v;
        break;
    case 'w':
        centerx -= v;
        break;
    // control centery
    case 'e':
        centery += v;
        break;
    case 'r':
        centery -= v;
        break;
    // control centerz
    case 't':
        centerz += v;
        break;
    case 'y':
        centerz -= v;
        break;

    // Control what is shown
    case 'a':
        isAxes = !isAxes;   // show/hide Axes if 'a' is pressed
        break;
    case 'c':
        isCube = !isCube;   // show/hide Cube if 'c' is pressed
        break;
    case 'p':
        isPyramid = !isPyramid; // show/hide Pyramid if 'p' is pressed
        break;

    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    case GLUT_KEY_LEFT:
        eyex += v * (upy*lz);
        eyez += v * (-lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_RIGHT:
        eyex += v * (-upy*lz);
        eyez += v * (lx*upy);
        s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
        eyex /= s;
        eyez /= s;
        break;
    case GLUT_KEY_UP:
        eyey += v;
        break;
    case GLUT_KEY_DOWN:
        eyey -= v;
        break;
    
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
