#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>

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
        glColor3d(1,0,0);
        drawOctHSurface();
        glRotatef(90,0,1,0);
        glColor3d(0,0,1);
        drawOctHSurface();
        glRotatef(90,0,1,0);
        glColor3d(0,1,0);
        drawOctHSurface();
        glRotatef(90,0,1,0);
        glColor3d(1,1,0);
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
    glColor3d(1,0,1);
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
    cout << thetta << endl;
    GLfloat d = (1-triangleScale)/sqrt(2);
    GLfloat radius = d*sin(thetta);
    cout << radius << endl;
    GLfloat len = sqrt(2);
    glPushMatrix();
        glTranslatef(0.5,0,0.5);
        glRotatef(-45,0,1,0);

        // glScalef((1-triangleScale),  (1-triangleScale), triangleScale);
        
        // glColor3f(1,0,1);
        // for(float i = -len*triangleScale/2;i<=len*triangleScale/2;i+=0.01){
        //     glPushMatrix();
        //         glLineWidth(1);
        //         glTranslatef(-d,0,0);
        //         glBegin(GL_LINE_STRIP);
        //         GLfloat a = M_PI-acos(-1.0/3);
        //         for(float theta = -a/2; theta < a/2; theta+=0.01){
        //             glVertex3f(radius*cos(theta), radius*sin(theta), i);
        //         }
        //         glEnd();
        //     glPopMatrix();
        // }
        glPushMatrix();
        glTranslatef(-d,0,0);

        glRotatef(-36, 0, 0, 1);
        drawCylinderUtil(len*triangleScale, radius, 100);
        glPopMatrix();
    glPopMatrix();
}

void drawAllCylinders(){
    drawCylinder();
    glPushMatrix();
        glRotatef(90,0,1,0);
        drawCylinder();
    
        glRotatef(90,0,1,0);
        drawCylinder();
    
        glRotatef(90,0,1,0);
        drawCylinder();
    glPopMatrix();
    glPushMatrix();
        glRotatef(90,1,0,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
    glPopMatrix();
    glPushMatrix();
        glRotatef(90,0,0,1);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
        glRotatef(90,0,1,0);
        drawCylinder();
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

    glColor3d(0,0,1);
    drawOctahedron();
    drawAllCylinders();
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
