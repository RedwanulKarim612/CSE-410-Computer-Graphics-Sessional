#include<bits/stdc++.h>
#include <GL/glut.h>  
#include "bitmap_image.hpp"
#include "util.h"

using namespace std;

Vector l, u, r;
Point pos;
double angle = 0.0;
double rotation_step = 5.0;
vector<Object *> objects;
vector<Light *> lights;
double near, far, fovX, fovY, aspectRatio;
double screenHeight, screenWidth, planeDistance;
double recursionLevel, pixels;
double checkerBoardWidth;
double checkerBoardAmbientCoeff, checkerBoardDiffuseCoeff, checkerBoardReflectionCoeff;
bitmap_image image;
void captureImage(){
    for(int i=0;i<pixels; i++){
        for(int j=0;j<pixels;j++){
            image.set_pixel(i,j,0.0,0.0,0.0);
        }
    }
    double dx = screenWidth*1.0/pixels;
    double dy = screenHeight*1.0/pixels;
    // cout << dx << " " << dy << endl;
    Point midPoint = l*near + pos;
    Point topLeft = (u*(screenHeight/2.0)) - (r*(screenWidth/2.0)) + midPoint;
    for(int i=0;i<pixels;i++){
        for(int j=0;j<pixels;j++){
            double min_t = -1;
            Point cur = r*(j*dx) - u*(i*dy) + topLeft;
            Vector rayDirection(pos,cur);
            rayDirection.normalize();
            Ray ray(pos, rayDirection);
            for(int k=0;k<objects.size();k++){
                double color[3] = {0.0, 0.0, 0.0};
                double t = objects[k]->illuminate(ray, color, recursionLevel, objects, lights, far-near);
                // double t = objects[k]->findIntersection(ray);
                if(t<0.0) continue;
                if(t<min_t || min_t<0.0){
                    image.set_pixel(j, i, color[0]*255, color[1]*255, color[2]*255);
                    min_t = t;
                }
            }
        }
    }
    image.save_image("out.bmp");
}


void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1000,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1000,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1000);
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
    glMatrixMode(GL_MODELVIEW);
    drawAxes();
    // drawCheckerBoard();
    // cout << objects.size() << endl;
    for(int i=0;i<objects.size();i++){
        if(objects[i]->isFloor) objects[i]->curPosition = pos;
        objects[i]->draw();
    }
    for(int i=0;i<lights.size();i++){
        lights[i]->draw();
    }
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
    gluPerspective(fovY, aspect, near, far);
}


void keyboardListener(unsigned char key, int xx,int yy){
    double rate = 0.05;
	switch(key){

		case '1':
			r.x = r.x*cos(rate)+l.x*sin(rate);
			r.y = r.y*cos(rate)+l.y*sin(rate);
			r.z = r.z*cos(rate)+l.z*sin(rate);

			l.x = l.x*cos(rate)-r.x*sin(rate);
			l.y = l.y*cos(rate)-r.y*sin(rate);
			l.z = l.z*cos(rate)-r.z*sin(rate);
			break;

        case '2':
			r.x = r.x*cos(-rate)+l.x*sin(-rate);
			r.y = r.y*cos(-rate)+l.y*sin(-rate);
			r.z = r.z*cos(-rate)+l.z*sin(-rate);

			l.x = l.x*cos(-rate)-r.x*sin(-rate);
			l.y = l.y*cos(-rate)-r.y*sin(-rate);
			l.z = l.z*cos(-rate)-r.z*sin(-rate);
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
			u.x = u.x*cos(rate)+r.x*sin(rate);
			u.y = u.y*cos(rate)+r.y*sin(rate);
			u.z = u.z*cos(rate)+r.z*sin(rate);

			r.x = r.x*cos(rate)-u.x*sin(rate);
			r.y = r.y*cos(rate)-u.y*sin(rate);
			r.z = r.z*cos(rate)-u.z*sin(rate);
			break;

        case '6':
			u.x = u.x*cos(-rate)+r.x*sin(-rate);
			u.y = u.y*cos(-rate)+r.y*sin(-rate);
			u.z = u.z*cos(-rate)+r.z*sin(-rate);

			r.x = r.x*cos(-rate)-u.x*sin(-rate);
			r.y = r.y*cos(-rate)-u.y*sin(-rate);
			r.z = r.z*cos(-rate)-u.z*sin(-rate);
			break;
        case '0':
            cout << "capturing image....\n";
            captureImage();
            cout << "image captured\n";
            break;
        case 'R':
            if(lights.size()>0){
                lights[0]->color[0] = 1.0;
                lights[0]->color[1] = 0.0;
                lights[0]->color[2] = 0.0;
                break;
            }
        case 'B':
            if(lights.size()>1){
                lights[1]->color[0] = 0.0;
                lights[1]->color[1] = 0.0;
                lights[1]->color[2] = 1.0;
                break;
            }
		default:
			break;
	}
	glutPostRedisplay();
}


void specialKeyListener(int key, int x,int y)
{
    double cameraMovementRate = 3.0;
	switch(key){
		case GLUT_KEY_UP:		//down arrow key
			pos.x+=l.x * cameraMovementRate;
			pos.y+=l.y * cameraMovementRate;
			pos.z+=l.z * cameraMovementRate;
			break;
		case GLUT_KEY_DOWN:		// up arrow key
			pos.x-=l.x * cameraMovementRate;
			pos.y-=l.y * cameraMovementRate;
			pos.z-=l.z * cameraMovementRate;
			break;

		case GLUT_KEY_RIGHT:
			pos.x+=r.x * cameraMovementRate;
			pos.y+=r.y * cameraMovementRate;
			pos.z+=r.z * cameraMovementRate;
			break;
		case GLUT_KEY_LEFT :
			pos.x-=r.x * cameraMovementRate;
			pos.y-=r.y * cameraMovementRate;
			pos.z-=r.z * cameraMovementRate;
			break;

		case GLUT_KEY_PAGE_UP:
		    pos.x+=u.x * cameraMovementRate;
			pos.y+=u.y * cameraMovementRate;
			pos.z+=u.z * cameraMovementRate;
			break;
		case GLUT_KEY_PAGE_DOWN:
            pos.x-=u.x * cameraMovementRate;
			pos.y-=u.y * cameraMovementRate;
			pos.z-=u.z * cameraMovementRate;
			break;

		default:
			break;
	}
	glutPostRedisplay();
}


int main(int argc, char** argv){
    // pos = Point(31.4356, -108.588, -114.923); 
    // u = Vector(0.353521, -0.759857, 0.545564);
    // r = Vector(0.915634, 0.400432, -0.0356058);
    // l = Vector(-0.191406, 0.512125, 0.837312);

    pos = Point(0, -200, 40);
    u = Vector(0, 0, 1);
    r = Vector(1, 0, 0);
    l = Vector(0, 1, 0);
    ifstream desFile("description.txt");
    int numberOfObjects;
    desFile >> near >> far >> fovY >> aspectRatio;
    desFile >> recursionLevel >> pixels;
    desFile >> checkerBoardWidth;
    desFile >> checkerBoardAmbientCoeff >> checkerBoardDiffuseCoeff >> checkerBoardReflectionCoeff;
    desFile >> numberOfObjects;
    fovX = aspectRatio * fovY;
    screenWidth = 2*near*tan(fovX*M_PI/360.0);
    screenHeight = 2*near*tan(fovY*M_PI/360.0);
    image = bitmap_image(pixels, pixels);
    pos.x=-pos.x;
    swap(pos.y, pos.z);
    swap(u.y, u.z);
    u.x = -u.x;
    swap(r.y, r.z);
    r.x = -r.x;
    swap(l.y, l.z);
    l.x = -l.x;
    double floorColor[3] = {0,0,0};
    Object *floor = new Floor(checkerBoardWidth,floorColor,checkerBoardAmbientCoeff,checkerBoardDiffuseCoeff,0.0,checkerBoardReflectionCoeff,30.0);
    objects.push_back(floor);
    // Floor floor = Floor(checkerBoardWidth, floorColor, checkerBoardAmbientCoeff, checkerBoardDiffuseCoeff, checkerBoardReflectionCoeff, 30);
    
    while(numberOfObjects--){
        string type;
        desFile >> type;
        if(type=="sphere"){
            Point center;
            double radius;
            double color[3];
            double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
            double shininess;
            desFile >> center.x >> center.z >> center.y;
            center.x = -center.x;
            desFile >> radius;
            desFile >> color[0] >> color[1] >> color[2];
            desFile >> ambientCoeff >> diffuseCoeff >> specularCoeff >> reflectionCoeff >> shininess;
            Object *o = new Sphere(center, radius, color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess);
            objects.push_back(o);
        }
        else if(type=="pyramid"){
            Point lowest_point;
            double width, height;
            double color[3];
            double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
            double shininess;
            desFile >> lowest_point.x >> lowest_point.z >> lowest_point.y;
            lowest_point.x = -lowest_point.x;
            desFile >> width >> height;
            desFile >> color[0] >> color[1] >> color[2];
            desFile >> ambientCoeff >> diffuseCoeff >> specularCoeff >> reflectionCoeff >> shininess;
            lowest_point.x = lowest_point.x - width/2.0;
            lowest_point.z = lowest_point.z + width/2.0;
            Object *o = new Pyramid(lowest_point, width, height, color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess);
            objects.push_back(o);
        }
        else if(type=="cube"){
            Point bottom_lower_left;
            double side;
            double color[3];
            double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
            double shininess;
            desFile >> bottom_lower_left.x >> bottom_lower_left.z >> bottom_lower_left.y;
            desFile >> side;
            bottom_lower_left.x = -bottom_lower_left.x;
            bottom_lower_left.x = bottom_lower_left.x - side;
            desFile >> color[0] >> color[1] >> color[2];
            desFile >> ambientCoeff >> diffuseCoeff >> specularCoeff >> reflectionCoeff >> shininess;
            Object *o = new Cube(bottom_lower_left, side, color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess);
            objects.push_back(o);
            // cout << "cube\n";
        }
    }
    int noOfLights;
    desFile >> noOfLights;
    while(noOfLights--){
        Point position;
        double fallOff;
        desFile >> position.x >> position.z >> position.y;
        position.x = -position.x;
        desFile >> fallOff;
        Light *l = new Light(position, fallOff);
        lights.push_back(l);
    }
    int noOfSpotLights;
    desFile >> noOfSpotLights;
    while(noOfSpotLights--){
        Point position;
        Point target;
        double fallOff;
        double cutoffAngle;
        desFile >> position.x >> position.z >> position.y;
        position.x = -position.x;
        desFile >> fallOff;
        desFile >> target.x >> target.z >> target.y;
        target.x = -target.x;
        desFile >> cutoffAngle;

        Light *l = new Light(position, fallOff, cutoffAngle, target);
        lights.push_back(l);
    }
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(pixels, pixels);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();  
    glutMainLoop();                             // Enter the event-processing loop
    for(int i=0;i<objects.size();i++) delete(objects[i]);
    for(int i=0;i<lights.size();i++) delete(lights[i]);
    return 0;
}