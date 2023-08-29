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

class Matrix{
public:
    int rows, columns;
    vector<vector<double> >mat;
    Matrix(){
        this->rows = 3;
        this->columns = 3;
        this->mat.resize(rows);
        for(int i=0;i<rows;i++){
            this->mat[i].resize(columns);
        }
    }
    Matrix(int rows, int columns){
        this->rows = rows;
        this->columns = columns;
        this->mat.resize(rows);
        for(int i=0;i<rows;i++){
            this->mat[i].resize(columns);
        }
    }
    void setVal(int row, int column, double val){
        this->mat[row][column] = val;
    }
    double determinant(){
        double r1 = mat[0][0]*(mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1]);
        double r2 = mat[0][1]*(mat[1][0]*mat[2][2] - mat[1][2]*mat[2][0]);
        double r3 = mat[0][2]*(mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0]);
        double det = r1-r2+r3;
        return det;
    }
};

class Triangle{
public:
    Point points[3];
    double color[3];
    Triangle(){
        this->points[0] = Point();
        this->points[1] = Point();
        this->points[2] = Point();
    }

    Triangle(Point points[3]){
        this->points[0] = points[0];
        this->points[1] = points[1];
        this->points[2] = points[2];
    }

    Triangle(Point a, Point b, Point c, double color[]){
        this->points[0] = a;
        this->points[1] = b;
        this->points[2] = c;
        this->color[0] = color[0];
        this->color[1] = color[1];
        this->color[2] = color[2];
    }

    void draw(){
        // cout << "drawing triangle\n";
        // cout << "points: " << points[0] << points[1] << points[2] << endl;
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glBegin(GL_TRIANGLES);
                glVertex3f(points[0].x, points[0].y, points[0].z);
                glVertex3f(points[1].x, points[1].y, points[1].z);
                glVertex3f(points[2].x, points[2].y, points[2].z);
            glEnd();
        glPopMatrix();
    }

    double findIntersection(Ray ray){
        Matrix A(3,3);
        A.setVal(0, 0, points[0].x-points[1].x);
        A.setVal(1, 0, points[0].y-points[1].y);
        A.setVal(2, 0, points[0].z-points[1].z);
        A.setVal(0, 1, points[0].x-points[2].x);
        A.setVal(1, 1, points[0].y-points[2].y);
        A.setVal(2, 1, points[0].z-points[2].z);
        A.setVal(0, 2, ray.direction.x);
        A.setVal(1, 2, ray.direction.y);
        A.setVal(2, 2, ray.direction.z);
        Matrix betaM = A;
        betaM.setVal(0,0,points[0].x-ray.origin.x);
        betaM.setVal(1,0,points[0].y-ray.origin.y);
        betaM.setVal(2,0,points[0].z-ray.origin.z);
        double beta = betaM.determinant()/A.determinant();
        if(beta<0.0 || beta>1.0) return -1.0;
        Matrix gammaM = A;
        gammaM.setVal(0,1,points[0].x-ray.origin.x);
        gammaM.setVal(1,1,points[0].y-ray.origin.y);
        gammaM.setVal(2,1,points[0].z-ray.origin.z);
        double gamma = gammaM.determinant()/A.determinant();
        if(gamma<0.0 || gamma>1.0-beta) return -1.0;
        Matrix tM = A;        
        
        tM.setVal(0,2,points[0].x-ray.origin.x);
        tM.setVal(1,2,points[0].y-ray.origin.y);
        tM.setVal(2,2,points[0].z-ray.origin.z);

        double t = tM.determinant()/A.determinant();
        if(t<0.0) return -1.0;
        return t;
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
        // cout << "drawing sphere\n";
        // cout << "center: " << center << endl;
        // cout << "radius: " << radius << endl;
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
        Point a,b,c,d,h;
        a = Point(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
        b = Point(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
        c = Point(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
        d = Point(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
        h = Point(lowest_point.x, lowest_point.y + height, lowest_point.z);
        vector<Triangle>triangles;
        triangles.push_back(Triangle(a,b,h, this->color));
        triangles.push_back(Triangle(b,c,h, this->color));
        triangles.push_back(Triangle(c,d,h, this->color));
        triangles.push_back(Triangle(d,a,h, this->color));
        triangles.push_back(Triangle(a,b,c, this->color));
        triangles.push_back(Triangle(a,c,d, this->color));
        for(int i=0;i<triangles.size();i++)
            triangles[i].draw();    
        glPushMatrix();
            // glTranslatef(lowest_point.x, lowest_point.y, lowest_point.z);
            glBegin(GL_QUADS);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
            glEnd();
            // glBegin(GL_TRIANGLES);
            //     glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
            //     glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
            //     glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            // glEnd();
            // glBegin(GL_TRIANGLES);
            //     glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
            //     glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
            //     glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            // glEnd();
            // glBegin(GL_TRIANGLES);
            //     glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
            //     glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
            //     glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            // glEnd();
            // glBegin(GL_TRIANGLES);
            //     glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
            //     glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
            //     glVertex3f(lowest_point.x, lowest_point.y + height, lowest_point.z);
            glEnd();
        glPopMatrix();
    }

    virtual double findIntersection(Ray ray){
        Point a,b,c,d,h;
        a = Point(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
        b = Point(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
        c = Point(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
        d = Point(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
        h = Point(lowest_point.x, lowest_point.y + height, lowest_point.z);
        vector<Triangle>triangles;
        triangles.push_back(Triangle(a,b,h, this->color));
        triangles.push_back(Triangle(b,c,h, this->color));
        triangles.push_back(Triangle(c,d,h, this->color));
        triangles.push_back(Triangle(d,a,h, this->color));
        double min_t = -1.0;
        int in = 0;
        for(int i=0;i<triangles.size();i++){
            double t = triangles[i].findIntersection(ray);
            if(t<0.0) continue;
            if(min_t<0.0 || t<min_t) {min_t = t; in = i;}
        }
        if(min_t<0.0) return -1.0;
        return min_t;
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
        // cout << "drawing cube\n";
        // cout << "bottom left: " << bottom_left << endl;
        // cout << "side: " << side << endl;
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glTranslatef(bottom_left.x, bottom_left.y, bottom_left.z);
            glutSolidCube(side);
        glPopMatrix();
    }

    virtual double findIntersection(Ray ray){
        Point bottom_left = this->bottom_left;
        Point bottom_right = Point(bottom_left.x + side, bottom_left.y, bottom_left.z);
        Point top_left = Point(bottom_left.x, bottom_left.y + side, bottom_left.z);
        Point top_right = Point(bottom_left.x + side, bottom_left.y + side, bottom_left.z);
        Point front_bottom_left = Point(bottom_left.x, bottom_left.y, bottom_left.z + side);
        Point front_bottom_right = Point(bottom_left.x + side, bottom_left.y, bottom_left.z + side);
        Point front_top_left = Point(bottom_left.x, bottom_left.y + side, bottom_left.z + side);
        Point front_top_right = Point(bottom_left.x + side, bottom_left.y + side, bottom_left.z + side);
        vector<Triangle>triangles;
        triangles.push_back(Triangle(bottom_left, bottom_right, top_right, this->color));
        triangles.push_back(Triangle(bottom_left, top_left, top_right, this->color));
        triangles.push_back(Triangle(bottom_left, bottom_right, front_bottom_right, this->color));
        triangles.push_back(Triangle(bottom_left, front_bottom_left, front_bottom_right, this->color));
        triangles.push_back(Triangle(bottom_left, front_bottom_left, top_left, this->color));
        triangles.push_back(Triangle(front_bottom_left, front_top_left, top_left, this->color));
        triangles.push_back(Triangle(front_bottom_left, front_bottom_right, front_top_right, this->color));
        triangles.push_back(Triangle(front_bottom_left, front_top_left, front_top_right, this->color));
        triangles.push_back(Triangle(front_bottom_right, bottom_right, top_right, this->color));
        triangles.push_back(Triangle(front_bottom_right, front_top_right, top_right, this->color));
        triangles.push_back(Triangle(front_top_left, front_top_right, top_left, this->color));
        triangles.push_back(Triangle(front_top_right, top_right, top_left, this->color));
        double min_t = -1.0;
        int in = 0;
        for(int i=0;i<triangles.size();i++){
            double t = triangles[i].findIntersection(ray);
            if(t<0.0) continue;
            if(min_t<0.0 || t<min_t) {min_t = t; in = i;}
        }
        if(min_t<0.0) return -1.0;
        return min_t;
    }
};