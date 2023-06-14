#include <GL/glut.h>
#include <cmath>
#include<iostream>

using namespace std;

GLint t = 0;
GLint p_t = 0;
GLdouble center_x = 0;
GLdouble center_y = 0.45;
GLdouble radius = 0.3;
GLdouble PI = acos(-1);
GLdouble s_max = 0.18;
GLdouble s = 0.0;
GLdouble pendulum_x = 0;
GLdouble pendulum_y = 0.35;
GLdouble pendulum_open_x = 0;
GLdouble pendulum_open_y = -0.5;
GLdouble pendulum_length = pendulum_y - pendulum_open_y;
GLdouble pendulum_period = 2;
GLdouble w = 2*PI/pendulum_period;
GLdouble pendulum_radius = 0.16;
GLdouble sec_theta_deg;
GLdouble min_theta_deg;
GLdouble hour_theta_deg;
GLdouble d_sec_theta_deg = 6;
GLdouble d_min_theta_deg = 0.1;
GLdouble d_hour_theta_deg = 1/600.0;

void initialize(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    int h = ltm->tm_hour;
    int m = ltm->tm_min;
    int s = ltm->tm_sec;

    // h = 5;
    // m = 58;
    // s = 55;

    sec_theta_deg = -(s*6.0);
    min_theta_deg = -(m*6.0 + s/10.0-90);
    hour_theta_deg = -(h*30.0 + m/2.0-90);
    cout << "Year:" << 1900 + ltm->tm_year<<endl;
    cout << "Month: "<< 1 + ltm->tm_mon<< endl;
    cout << "Day: "<< ltm->tm_mday << endl;
    cout << "Time: "<< ltm->tm_hour << ":";
    cout << ltm->tm_min << ":";
    cout << ltm->tm_sec << endl;
}

void drawClockStructure(){
    GLfloat r = 1.8, ex = 0.16;
    glColor3f(0.37, 0.18, 0);
    glBegin(GL_POLYGON);
        glVertex2f(center_x-radius/r, center_y+radius+ex);
        glVertex2f(center_x+radius/r, center_y+radius+ex);
        glVertex2f(center_x+radius+ex, center_y+radius/r);
        glVertex2f(center_x+radius+ex, center_y-radius/r);
        glVertex2f(center_x+radius/r, center_y-radius-ex);
        glVertex2f(center_x-radius/r, center_y-radius-ex);
        glVertex2f(center_x-radius-ex, center_y-radius/r);
        glVertex2f(center_x-radius-ex, center_y+radius/r);
    glEnd();
}

void drawPendulum(){
    glColor3f(0.53, 0.28, 0.04);
    glBegin(GL_POLYGON);
        glVertex2d(pendulum_open_x - 0.05, pendulum_open_y);
        glVertex2d(pendulum_open_x + 0.05, pendulum_open_y);
        glVertex2d(pendulum_x + 0.05, pendulum_y);
        glVertex2d(pendulum_x - 0.05, pendulum_y);

    glEnd();
    glColor3d(0.67, 0.4, 0.14);
    glBegin(GL_POLYGON);
    for(float theta = 0; theta<360; theta+=1.0){
        glVertex2d(pendulum_open_x + pendulum_radius*cos(theta*M_PI/180.0), pendulum_open_y+pendulum_radius*sin(theta*M_PI/180.0));
    }
    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.97, 0.69, 0.42);

    for(float theta = 0; theta<360; theta+=1.0){
        glVertex2d(pendulum_open_x + (pendulum_radius-0.05)*cos(theta*M_PI/180.0), pendulum_open_y+(pendulum_radius-0.05)*sin(theta*M_PI/180.0));
    }
    glEnd();
}

void drawPendulumBG(){
    GLfloat r = 1.8, ex = 0.16;
    glColor3f(0.37, 0.18, 0);
    glBegin(GL_POLYGON);
        glVertex2f(center_x+radius+ex, center_y);
        glVertex2f(center_x+radius+ex, center_y-pendulum_length-1.2*pendulum_radius);
        glVertex2f(center_x, center_y-pendulum_length-2.9*pendulum_radius);
        glVertex2f(center_x-radius-ex, center_y-pendulum_length-1.2*pendulum_radius);
        glVertex2f(center_x-radius-ex, center_y);
    glEnd();
    glBegin(GL_POLYGON);
        glColor3f(0.66, 0.47, 0.3);
        glVertex2f(center_x+radius+ex/1.5, center_y);
        glColor3f(0.94, 0.75, 0.57);
        glVertex2f(center_x+radius+ex/1.5, center_y-pendulum_length-pendulum_radius);
        glColor3f(0.94, 0.75, 0.57);
        glVertex2f(center_x, center_y-pendulum_length-2.5*pendulum_radius);
        glVertex2f(center_x-radius-ex/1.5, center_y-pendulum_length-pendulum_radius);
        glColor3f(0.66, 0.47, 0.3);
        glVertex2f(center_x-radius-ex/1.5, center_y);
    glEnd();
    
}
void drawClockDialBG(GLdouble center_x, GLdouble center_y, GLdouble radius){
    glColor3f(0.56, 0.46, 0.36);
    glLineWidth(10.0);
    glBegin(GL_LINE_LOOP);
    GLfloat dialRadius = 1.2*radius;
    for(float theta = 0; theta<360; theta+=1){
        glVertex2d((dialRadius)*cos(theta*M_PI/180.0) + center_x, (dialRadius)*sin(theta*M_PI/180.0) + center_y);
    }
    glEnd();
    glColor3f(0.85, 0.78, 0.75);
    glBegin(GL_POLYGON);
    for(float theta = 0; theta<360; theta+=1.0){
        glVertex2d((dialRadius)*cos(theta*M_PI/180.0) + center_x, (dialRadius)*sin(theta*M_PI/180.0) + center_y);
    }
    glEnd();
    glColor3f(0, 0, 0);

    glLineWidth(1.0);
    glBegin(GL_LINE_LOOP);
    for(float theta = 0; theta<360; theta+=0.5){
        glVertex2d(radius*cos(theta*M_PI/180.0) + center_x, radius*sin(theta*M_PI/180.0) + center_y);
    }
    glEnd();
    GLfloat ex = 0.03;
    glBegin(GL_LINE_LOOP);
    for(float theta = 0; theta<360; theta+=0.5){
        glVertex2d((radius+ex)*cos(theta*M_PI/180.0) + center_x, (radius+ex)*sin(theta*M_PI/180.0) + center_y);
    }
    glEnd();

    glBegin(GL_LINES);
    for(float theta = 0; theta<360; theta+=6){
        glVertex2d(radius*cos(theta*M_PI/180.0) + center_x, radius*sin(theta*M_PI/180.0) + center_y);
        glVertex2d((radius+ex)*cos(theta*M_PI/180.0) + center_x, (radius+ex)*sin(theta*M_PI/180.0) + center_y);
    }
    glEnd();
    glLineWidth(3.0);
    for(float theta = 0; theta<360; theta+=30){
        GLfloat theta_rad = theta*M_PI/180.0;
        GLfloat phi = 4*M_PI/360.0;
        glBegin(GL_POLYGON);
            glVertex2d(radius*cos(theta_rad) + center_x, radius*sin(theta_rad) + center_y);
            glVertex2d((radius+ex)*cos(theta_rad-phi) + center_x, (radius+ex)*sin(theta_rad-phi) + center_y);
            glVertex2d((radius+ex)*cos(theta_rad+phi) + center_x, (radius+ex)*sin(theta_rad+phi) + center_y);
        glEnd();
    }
}

void drawSecHand(){
    glColor3f(0, 0, 0);
    glLineWidth(2.5);
    glBegin(GL_LINES);
        glVertex2f(0,-.25*radius);
        glVertex2f(0,0);
    glEnd();
    glLineWidth(1.0);
    glBegin(GL_LINES);
        glVertex2f(0,0);
        glVertex2f(0,radius);
    glEnd();
}

void drawMinHand(){
    glColor3f(0, 0, 0);
    glLineWidth(2.0);
    glBegin(GL_POLYGON);
        glVertex2f(0,-0.004);
        glVertex2f(0.65*radius, -0.035*radius);
        glVertex2f(0.9*radius, 0);
        glVertex2f(0.65*radius, 0.035*radius);
        glVertex2f(0,0.004);
    glEnd();
}

void drawHourHand(){
    glColor3f(0, 0, 0);
    glLineWidth(2.0);
    glBegin(GL_POLYGON);
        glVertex2f(0,-0.007);
        glVertex2f(0.45*radius, -0.05*radius);
        glVertex2f(0.65*radius, 0);
        glVertex2f(0.45*radius, 0.05*radius);
        glVertex2f(0,0.007);
    glEnd();
}
void display(){
    glClear(GL_COLOR_BUFFER_BIT);
    glPointSize(10.0);
    glColor3d(0, 0, 1);
    GLdouble theta = 30;

    drawPendulumBG();
    drawPendulum();

    drawClockStructure();
    drawClockDialBG(center_x, center_y, radius);

    glPushMatrix();
        glTranslatef(center_x, center_y, 0);
        glRotatef(sec_theta_deg, 0, 0, 1);
        drawSecHand();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(center_x, center_y, 0);
        glRotatef(min_theta_deg, 0, 0, 1);
        drawMinHand();
    glPopMatrix();
    glPushMatrix();
        glTranslatef(center_x, center_y, 0);
        glRotatef(hour_theta_deg, 0, 0, 1);
        drawHourHand();
    glPopMatrix();
  
    glColor3f(0.31, 0.25, 0.19);
    glBegin(GL_POLYGON);
    for(float theta = 0; theta<360; theta+=0.5){
        glVertex2d(0.01*cos(theta*M_PI/180.0) + center_x, 0.01*sin(theta*M_PI/180.0) + center_y);
    }
    glEnd();
   
    glFlush();
}

void timer(int value){
    t++;
    p_t++;
    if(p_t==2000) p_t = 0;
    glutPostRedisplay();
    if(t==1000){    
        t = 0;
        sec_theta_deg = sec_theta_deg-d_sec_theta_deg;
        min_theta_deg = min_theta_deg-d_min_theta_deg;
        hour_theta_deg = hour_theta_deg-d_hour_theta_deg;
    }
    s = s_max*cos(w*p_t/1000.0);
    GLdouble theta = s/pendulum_length;
    pendulum_open_x = pendulum_x + pendulum_length*sin(theta);
    pendulum_open_y = pendulum_y - pendulum_length*cos(theta);

    glutTimerFunc(1, timer, 0);
}

int main(int argc, char** argv){
    initialize();
    glutInit(&argc, argv);                  // Initialize GLUT
    glutInitWindowSize(640, 640);           // Set the window's initial width & height
    glutInitWindowPosition(100, 50);        // Position the window's initial top-left corner
    glutCreateWindow("Clock");              // Create a window with the given title
    glutTimerFunc(0, timer, 0);             // First timer call immediately
    glutDisplayFunc(display);               // Register display callback handler for window re-paint
    glClearColor(1.0, 1.0, 1.0, 1.0);       // Set background color to white
    glutMainLoop();                         // Enter the infinitely event-processing loop
    return 0;
}