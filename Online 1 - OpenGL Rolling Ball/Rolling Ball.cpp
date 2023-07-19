#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

struct point {
    GLfloat x, y, z;
};
// Global variables
struct point pos;   // position of the eye
struct point l;     // look/forward direction
struct point r;     // right direction
struct point u;     // up direction

float angle = 0.0;
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
GLfloat triangleScale = 0;
float rotation_step = 5.0;
float len = sqrt(2.0);

float roll_angle = 0.0;
float roll_angle_d = 10;
float ball_pos_z = 0;

void drawOctHSurface(){
    glPushMatrix();
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
    GLfloat d = (1-triangleScale)*len/2.0;
    GLfloat radius = d*sin(thetta);
    GLfloat len = sqrt(2);
    glPushMatrix();
        glRotatef(-45,0,1,0);
        glTranslatef(len/2,0,0);
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
    float n1[3];        
    float n2[3];        
    float v[3];         
    float a1;           
    float a2;           

    int pointsPerRow = (int)pow(2, subdivision) + 1;

    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            float scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

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
    GLfloat d = (1.0-triangleScale);
    GLfloat radius = d*sin(thetta)*len/2.0;
    glPushMatrix();
        glTranslatef(1-d,0,0);
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

void drawGrid(){
    glColor3f(1, 1, 1);
    glLineWidth(5);
    glBegin(GL_LINES);
        glVertex3f(-100,0,0);
        glVertex3f(100,0,0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(0,0,-100);
        glVertex3f(0,0,100);
    glEnd();
    glLineWidth(1);
    glBegin(GL_LINES);
        for(int i=-100;i<=100;i++){
            glVertex3f(i,0,-100);
            glVertex3f(i,0,100);
        }
        for(int i=-100;i<=100;i++){
            glVertex3f(-100,0,i);
            glVertex3f(100,0,i);
        }
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
    // gluLookAt(eyex,eyey,eyez,
    //           centerx,centery,centerz,
    //           upx,upy,upz);
    gluLookAt(pos.x,pos.y,pos.z,
              pos.x+l.x,pos.y+l.y,pos.z+l.z,
              u.x,u.y,u.z);
    // draw
    // glScalef(2,2,2);
    drawGrid();
    glPushMatrix();
            glTranslatef(0,1/sqrt(2.0),ball_pos_z);
            glRotatef(roll_angle,1,0,0);
            drawOctahedron();
            drawAllCylinders();
            drawAllSphereSegments();
            // cout << 1/sqrt(3.0) << endl;
    glPopMatrix();
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

void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.01;
	switch(key){
        case ',':
            triangleScale = max(0.0,triangleScale - 0.05);
            break;
        case '.':
            triangleScale = min(1.0,triangleScale + 0.05);
            break;
		case '1':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
			break;

        case '2':

			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
            
			break;

        case '3':
			l.x = l.x*cos(rate)+u.x*sin(rate);
			l.y = l.y*cos(rate)+u.y*sin(rate);
			l.z = l.z*cos(rate)+u.z*sin(rate);

			u.x = u.x*cos(rate)-l.x*sin(rate);
			u.y = u.y*cos(rate)-l.y*sin(rate);
			u.z = u.z*cos(rate)-l.z*sin(rate);
			break;

        case '4':
			l.x = l.x*cos(-rate)+u.x*sin(-rate);
			l.y = l.y*cos(-rate)+u.y*sin(-rate);
			l.z = l.z*cos(-rate)+u.z*sin(-rate);

			u.x = u.x*cos(-rate)-l.x*sin(-rate);
			u.y = u.y*cos(-rate)-l.y*sin(-rate);
			u.z = u.z*cos(-rate)-l.z*sin(-rate);
			break;

        case '5':

			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
            
			break;

        case '6':

			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case 'a':
            roll_angle+=roll_angle_d;
            if(roll_angle>=360) roll_angle -= 360;
            ball_pos_z+=roll_angle_d*M_PI/180.0/sqrt(2.0);
            // angle-=rotation_step;
            // if(angle<0) angle += 360;
            break;
        case 'd':
            roll_angle-=roll_angle_d;
            if(roll_angle<0) roll_angle += 360;
            ball_pos_z-=roll_angle_d*M_PI/180.0/sqrt(2.0);
            // angle+=rotation_step;
            // if(angle>=360) angle -= 360;
            break;
		default:
			break;
	}
	glutPostRedisplay();
}

void specialKeyListener(int key, int x,int y)
{
	switch(key){
		case GLUT_KEY_UP:		
			pos.x+=l.x;
			pos.y+=l.y;
			pos.z+=l.z;
			break;
		case GLUT_KEY_DOWN:		
			pos.x-=l.x;
			pos.y-=l.y;
			pos.z-=l.z;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x;
			pos.y+=r.y;
			pos.z+=r.z;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x;
			pos.y-=r.y;
			pos.z-=r.z;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x;
			pos.y+=u.y;
			pos.z+=u.z;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x;
			pos.y-=u.y;
			pos.z-=u.z;
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
	glutPostRedisplay();
}
/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    pos.x=3.5;pos.y=3.5;pos.z=3.5;
    l.x=-1/sqrt(3.0);l.y=-1/sqrt(3.0);l.z=-1/sqrt(3.0);
    u.x=-1/sqrt(6.0);u.y=2/sqrt(6.0);u.z=-1/sqrt(6.0);
    r.x = u.y * l.z - u.z * l.y;
    r.y = u.z * l.x - u.x * l.z;
    r.z = u.x * l.y - u.y * l.x;

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