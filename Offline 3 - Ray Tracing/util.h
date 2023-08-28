#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

class Vector;

class Point{
public:
    double x, y, z;
    Point(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Point(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double getX(){
        return x;
    }
    double getY(){
        return y;
    }
    double getZ(){
        return z;
    }

    Point operator*(double scalar){
        Point result;
        result.x = this->x * scalar;
        result.y = this->y * scalar;
        result.z = this->z * scalar;
        return result;
    }

    

    friend istream& operator>>(istream &input, Point &point){
        input >> point.x >> point.y >> point.z;
        return input;
    }

    friend ostream& operator<<(ostream &output, Point &point){
        output << point.x << " " << point.y << " " << point.z << endl;
        return output;
    }
};


class Vector{
public:
    double x, y, z;
    Vector(){
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    
    Vector(double x, double y, double z){
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector(Point pt){
        this->x = pt.getX();
        this->y = pt.getY();
        this->z = pt.getZ();
    }

    Vector(Point pt1, Point pt2){
        this->x = pt2.getX() - pt1.getX();
        this->y = pt2.getY() - pt1.getY();
        this->z = pt2.getZ() - pt1.getZ();
    }

    void normalize(){
        double magnitude = sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
        this->x /= magnitude;
        this->y /= magnitude;
        this->z /= magnitude;
    }

    friend istream& operator>>(istream &input, Vector &vector){
        input >> vector.x >> vector.y >> vector.z;
        return input;
    }

    friend ostream& operator<<(ostream &output, Vector &vector){
        output << vector.x << " " << vector.y << " " << vector.z << endl;
        return output;
    }

    Vector operator*(Vector &other){
        Vector result;
        result.x = this->y*other.z - this->z*other.y;
        result.y = this->z*other.x - this->x*other.z;
        result.z = this->x*other.y - this->y*other.x;
        return result;
    }

    Vector operator*(double scalar){
        Vector result;
        result.x = this->x * scalar;
        result.y = this->y * scalar;
        result.z = this->z * scalar;
        return result;
    }

    Vector operator+(Vector other){
        Vector result;
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        result.z = this->z + other.z;
        return result;
    }

    Vector operator-(Vector other){
        Vector result;
        result.x = this->x - other.x;
        result.y = this->y - other.y;
        result.z = this->z - other.z;
        return result;
    }

    Point operator+(Point other){
        Point result;
        result.x = this->x + other.x;
        result.y = this->y + other.y;
        result.z = this->z + other.z;
        return result;
    }

    Vector operator-(){
        Vector result;
        result.x = -this->x;
        result.y = -this->y;
        result.z = -this->z;
        return result;
    }

    double dotProduct(Vector &other){
        return this->x*other.x + this->y*other.y + this->z*other.z;
    }
};

class Ray{
public:
    Point origin;
    Vector direction;

    Ray(Point origin, Vector direction){
        this->origin = origin;
        this->direction = direction;
    }
};

class Object{
public:
    double color[3];
    double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
    double shininess;
    Object(){
        this->color[0] = 0;
        this->color[1] = 0;
        this->color[2] = 0;
        this->ambientCoeff = 0;
        this->diffuseCoeff = 0;
        this->specularCoeff = 0;
        this->reflectionCoeff = 0;
        this->shininess = 0;
    }
    Object(double color[3], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess){
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
        this->ambientCoeff = ambientCoeff;
        this->diffuseCoeff = diffuseCoeff;
        this->specularCoeff = specularCoeff;
        this->reflectionCoeff = reflectionCoeff;
        this->shininess = shininess;
    }
    virtual void draw(){
        // cout << "drawing object\n";
    }

    virtual double findIntersection(Ray ray){
        return 0.0;
    }
};

class Sphere: public Object{
public:
    Point center;
    double radius;
    Sphere(){
        this->center = Point();
        this->radius = 0;
    }
    Sphere(Point center, double radius, double color[], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess): Object(color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess){
        this->center = center;
        this->radius = radius;
    }
    virtual void draw(){
        cout << "drawing sphere\n";
        cout << "center: " << center << endl;
        cout << "radius: " << radius << endl;
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glTranslatef(center.x, center.y, center.z);
            glutSolidSphere(radius, 30, 30);
        glPopMatrix();
    }
    virtual double findIntersection(Ray ray){
        // return 1.0;
        double a = 1.0;
        Vector originVector = Vector(center, ray.origin);
        double tp = - originVector.dotProduct(ray.direction);
        if(tp<0) return -1.0;
        double ds = originVector.dotProduct(originVector) - tp*tp;
        double tpps = radius*radius - ds;
        if(tpps<0) return -1.0;
        if(originVector.dotProduct(originVector)>radius*radius){
            return tp-sqrt(tpps);
        }
        else return tp+sqrt(tpps);
        // double b = 2*(ray.direction.dotProduct(originVector));
        // double c = originVector.dotProduct(originVector) - radius*radius;
        // cout << b*b-4*a*c << endl;
        // double d = sqrt(b*b-4*a*c);
        
        // double t1 = (-b + d)/2.0;
        // double t2 = (-b - d)/2.0;
        // cout << t1 << " " << t2 << endl;
        // if(t1 < 0 && t2 < 0){
        //     return -1;
        // }
        // else if(t1 < 0){
        //     return t2;
        // }
        // else if(t2 < 0){
        //     return t1;
        // }
        // else{
        //     return min(t1, t2);
        // }
    }
};

class Pyramid: public Object{
public:
    Point lowest_point;
    double width, height;
    Pyramid(){
        this->lowest_point = Point();
        this->width = 0;
        this->height = 0;
    }

    Pyramid(Point lowest_point, double width, double height, double color[3], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess): Object(color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess){
        this->lowest_point = lowest_point;
        this->width = width;
        this->height = height;
    }

    void draw(){
        cout << "drawing pyramid\n";
        cout << "lowest point: " << lowest_point << endl;
        cout << "width: " << width << endl;
        cout << "height: " << height << endl;
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glTranslatef(lowest_point.x, lowest_point.y, lowest_point.z);
            glBegin(GL_QUADS);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            glEnd();
        glPopMatrix();
    }

    virtual double findIntersection(Ray ray){
        return 0;
    }
    
};

class Cube: public Object{
public:
    Point bottom_left;
    double side;

    Cube(){
        this->bottom_left = Point();
        this->side = 0;
    }

    Cube(Point bottom_left, double side, double color[3], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess): Object(color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess){
        this->bottom_left = bottom_left;
        this->side = side;
    }

    void draw(){
        cout << "drawing cube\n";
        cout << "bottom left: " << bottom_left << endl;
        cout << "side: " << side << endl;
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glTranslatef(bottom_left.x, bottom_left.y, bottom_left.z);
            glutSolidCube(side);
        glPopMatrix();
    }
};