#include<bits/stdc++.h>
#include <GL/glut.h>
using namespace std;

class Vector;
class Light;
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

    double distance(Point other){
        double dx = this->x - other.x;
        double dy = this->y - other.y;
        double dz = this->z - other.z;
        return sqrt(dx*dx + dy*dy + dz*dz);
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

    Vector operator*(Vector other){
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


class Light{
public:
    Point position;
    double fallOff;
    double color[3];
    double cutOffAngle; // in degreees
    Vector direction;
    Light(){
        this->position = Point();
        this->fallOff = 0;
        for(int i=0;i<3;i++){
            this->color[i] = 1.0;
        }   
        cutOffAngle = 365.0;
        direction = Vector(1.0,0,0);
    }
    Light(Point position, double fallOff){
        this->position = position;
        this->fallOff = fallOff;
        for(int i=0;i<3;i++){
            this->color[i] = 1.0;
        }
        cutOffAngle = 365.0;
        direction = Vector(1.0,0,0);
    }
    Light(Point position, double fallOff, double cutOffAngle, Point target){
        this->position = position;
        this->fallOff = fallOff;
        for(int i=0;i<3;i++){
            this->color[i] = 1.0;
        }
        this->cutOffAngle = cutOffAngle;
        this->direction = Vector(this->position, target);
        this->direction.normalize();
    }

    void draw(){
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            glTranslatef(position.x, position.y, position.z);
            glutSolidSphere(2.5, 30, 30);
        glPopMatrix();
        if(cutOffAngle<360.0) {
            glLineWidth(10.0);
            glColor3d(1,1,1);
            glBegin(GL_LINES);
            glVertex3f(position.x,position.y,position.z);
            Point t = -direction*15 + position;
            glVertex3f(t.x, t.y, t.z);
            glEnd();
        }
    }    

    bool inSideCutoffRegion(Vector outgoingRay){
        if(cutOffAngle>360.0) return true;
        double theta = acos(direction.dotProduct(outgoingRay))*180/M_PI;
        if(theta<cutOffAngle) return true;
        return false;
    }
};


class Object{
public:
    double color[3];
    double ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff;
    double shininess;
    bool isFloor = false;
    Point curPosition;
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

    virtual double* getColor(Point pt){
        return this->color;
    }

    virtual double illuminate(Ray ray, double (&color)[3], int depth, vector<Object*>objects, vector<Light *>lights, int distance_threshold){
        double t = findIntersection(ray);
        if(t<0.0) return -1.0;
        if(t>distance_threshold) return -1.0;
        Point intersectionPoint = ray.direction*t + ray.origin;
        double * colorAtIntersection;
        colorAtIntersection = this->getColor(intersectionPoint);
        for(int i=0;i<3;i++){
            color[i] += colorAtIntersection[i]*ambientCoeff;
        }    
        double lambert = 0, phong = 0;
        for(int i=0;i<lights.size();i++){
            Vector lightVector = Vector(intersectionPoint, lights[i]->position);
            lightVector.normalize();
            if(!lights[i]->inSideCutoffRegion(-lightVector)) {continue;}
            // if(lights[i]->cutOffAngle<360.0) cout << "in spotlight\n";
            Ray shadowRay(lightVector*0.000001 + intersectionPoint, lightVector);
            double tToLight = (lights[i]->position.x - intersectionPoint.x)/shadowRay.direction.x;
            bool inShadow = false;
            for(int j=0;j<objects.size();j++){
                double tObj = objects[j]->findIntersection(shadowRay);
                if(tObj<tToLight && tObj>0.0){
                    // cout << "in shadow\n";
                    // cout << "tObj: " << tObj << endl;
                    inShadow = true;
                    break;
                }
            }
            // if(inShadow)cout << "in shadow " << inShadow << endl;
            if(inShadow) continue;
            double distanceFromLight = intersectionPoint.distance(lights[i]->position);
            Vector normal = getNormal(intersectionPoint, lightVector);
            normal.normalize();
            lambert = max(0.0,lightVector.dotProduct(normal)*pow(2.718281828, - distanceFromLight*distanceFromLight*lights[i]->fallOff));
            if(lambert<0.0) lambert = 0.0;
            for(int j=0;j<3;j++){
                color[j] += colorAtIntersection[j]*diffuseCoeff*lambert*lights[i]->color[j];
            }
            Vector reflectionVector = lightVector - normal*(2.0*(lightVector).dotProduct(normal));
            reflectionVector.normalize();
            phong = pow(max(0.0,reflectionVector.dotProduct(ray.direction)), shininess)*pow(2.718281828, - distanceFromLight*distanceFromLight*lights[i]->fallOff);
            for(int j=0;j<3;j++){
                color[j] += colorAtIntersection[j]*specularCoeff*phong*lights[i]->color[j];
            }
        }
        for(int i=0;i<3;i++){
            color[i] = min(max(color[i],0.0), 1.0);
        }
        if(depth==0) return t;
        Vector normal = getNormal(intersectionPoint, -ray.direction);
        Vector reflectionVector = ray.direction - normal*(2.0*ray.direction.dotProduct(normal));
        reflectionVector.normalize();
        Ray reflectedRay = Ray(reflectionVector*0.000001+intersectionPoint, reflectionVector);
        double t2 = -1, t_min = -1, obj_in = -1;
        for(int i=0;i<objects.size();i++){
            t2 = objects[i]->findIntersection(reflectedRay);
            if(t2<0.0) continue;
            if(t_min<0.0 || t2<t_min) {
                t_min = t2;
                obj_in = i;
            }
        }
        if(t_min<0.0) return t;
        double color2[3] = {0,0,0};
        t2 = objects[obj_in]->illuminate(reflectedRay, color2, depth-1, objects, lights, distance_threshold);
        for(int i=0;i<3;i++){
            color[i] += color2[i]*reflectionCoeff;
        }

        return t;
    }

    virtual Vector getNormal(Point point, Vector lightVector){
        return Vector();
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

    bool contains(Point point){
        Matrix A(3,3);
        A.setVal(0,0,points[0].x);
        A.setVal(1,0,points[0].y);
        A.setVal(2,0,points[0].z);
        A.setVal(0,1,points[1].x);
        A.setVal(1,1,points[1].y);
        A.setVal(2,1,points[1].z);
        A.setVal(0,2,points[2].x);
        A.setVal(1,2,points[2].y);
        A.setVal(2,2,points[2].z);
        Matrix alphaM = A;
        alphaM.setVal(0,0,point.x);
        alphaM.setVal(1,0,point.y);
        alphaM.setVal(2,0,point.z);
        double alpha = alphaM.determinant()/A.determinant();
        if(alpha<0.0 || alpha>1.0) return false;
        Matrix betaM = A;
        betaM.setVal(0,1,point.x);
        betaM.setVal(1,1,point.y);
        betaM.setVal(2,1,point.z);
        double beta = betaM.determinant()/A.determinant();
        if(beta<0.0 || beta>1.0) return false;
        Matrix gammaM = A;
        gammaM.setVal(0,2,point.x);
        gammaM.setVal(1,2,point.y);
        gammaM.setVal(2,2,point.z);
        double gamma = gammaM.determinant()/A.determinant();
        if(gamma<0.0 || gamma>1.0) return false;
        if(abs(alpha+beta+gamma-1.0)>0.0001) return false;
        return true;
    }

    Vector getNormal(Point point, Vector lightVector){
        Vector normal = Vector(points[0], points[1])*Vector(points[0], points[2]);
        normal.normalize();
        if(normal.dotProduct(lightVector)<0.0) normal = -normal;
        return normal;
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

    virtual Vector getNormal(Point point, Vector lightVector){
        Vector normal = Vector(center, point);
        normal.normalize();
        if(normal.dotProduct(lightVector)<0.0) normal = -normal;
        return normal;
    }
};

class Pyramid: public Object{
public:
    Point lowest_point;
    double width, height;
    vector<Triangle>triangles;
    Pyramid(){
        this->lowest_point = Point();
        this->width = 0;
        this->height = 0;
    }

    Pyramid(Point lowest_point, double width, double height, double color[3], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess): Object(color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess){
        this->lowest_point = lowest_point;
        this->width = width;
        this->height = height;
        Point a,b,c,d,h;
        a = Point(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
        b = Point(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
        c = Point(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
        d = Point(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
        h = Point(lowest_point.x, lowest_point.y + height, lowest_point.z);
        triangles.push_back(Triangle(a,b,h, this->color));
        triangles.push_back(Triangle(b,c,h, this->color));
        triangles.push_back(Triangle(c,d,h, this->color));
        triangles.push_back(Triangle(d,a,h, this->color));
        triangles.push_back(Triangle(a,b,c, this->color));
        triangles.push_back(Triangle(a,c,d, this->color));
    }

    void draw(){
        glColor3f(color[0], color[1], color[2]);
        for(int i=0;i<triangles.size();i++)
            triangles[i].draw();    
        glPushMatrix();
            glBegin(GL_QUADS);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z + width/2);
                glVertex3f(lowest_point.x + width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z - width/2);
                glVertex3f(lowest_point.x - width/2, lowest_point.y, lowest_point.z + width/2);
            glEnd();
            glEnd();
        glPopMatrix();
    }

    virtual double findIntersection(Ray ray){
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

    virtual Vector getNormal(Point point, Vector lightVector){
        for(int i=0;i<triangles.size();i++){
            if(triangles[i].contains(point)){
                return triangles[i].getNormal(point, lightVector);
            }
        }
        return Vector();
    }
    
};

class Cube: public Object{
public:
    Point bottom_left;
    double side;
    vector<Triangle>triangles;
    Cube(){
        this->bottom_left = Point();
        this->side = 0;
        triangles.resize(12);
    }

    Cube(Point bottom_left, double side, double color[3], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess): Object(color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess){
        this->bottom_left = bottom_left;
        this->side = side;
        Point bottom_right = Point(bottom_left.x + side, bottom_left.y, bottom_left.z);
        Point top_left = Point(bottom_left.x, bottom_left.y + side, bottom_left.z);
        Point top_right = Point(bottom_left.x + side, bottom_left.y + side, bottom_left.z);
        Point front_bottom_left = Point(bottom_left.x, bottom_left.y, bottom_left.z + side);
        Point front_bottom_right = Point(bottom_left.x + side, bottom_left.y, bottom_left.z + side);
        Point front_top_left = Point(bottom_left.x, bottom_left.y + side, bottom_left.z + side);
        Point front_top_right = Point(bottom_left.x + side, bottom_left.y + side, bottom_left.z + side);
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
    }

    void draw(){
        glColor3f(color[0], color[1], color[2]);
        glPushMatrix();
            for(int i=0;i<triangles.size();i++)
                triangles[i].draw();
        glPopMatrix();
    }

    virtual double findIntersection(Ray ray){
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

    virtual Vector getNormal(Point point, Vector lightVector){
        for(int i=0;i<triangles.size();i++){
            if(triangles[i].contains(point)){
                return triangles[i].getNormal(point, lightVector);
            }
        }   
        return Vector();    
    }
};

class Floor: public Object{
public:
    double checkerBoardWidth;
    int prevCellX;
    int prevCellY;
    Floor(){
        checkerBoardWidth = 25.0;
        isFloor = true;
    }
    
    Floor(double checkerBoardWidth, double color[3], double ambientCoeff, double diffuseCoeff, double specularCoeff, double reflectionCoeff, double shininess): Object(color, ambientCoeff, diffuseCoeff, specularCoeff, reflectionCoeff, shininess){
        this->checkerBoardWidth = checkerBoardWidth;
        prevCellX = (int)((curPosition.x+2000)/checkerBoardWidth);
        prevCellY = (int)((curPosition.z+2000)/checkerBoardWidth);
        isFloor = true;
    }

    Floor(double checkerBoardWidth, double color[3]): Object(color, 0.4, 0.4, 0.2, 0.0, 0.0){
        this->checkerBoardWidth = checkerBoardWidth;
        isFloor = true;
    }

    virtual Vector getNormal(Point point, Vector lightVector){
        if(lightVector.y>0.0) return Vector(0, 1, 0);
        else return Vector(0, -1, 0);
    }

    virtual double findIntersection(Ray ray){
        double t = -ray.origin.y/ray.direction.y;
        if(t<0.0) return -1.0;
        return t;
    }

    virtual double* getColor(Point pt){
        int i = (int)((pt.x+1000)/checkerBoardWidth);
        int j = (int)((pt.z+1000)/checkerBoardWidth);
        double *white = new double[3];
        double *black = new double[3];
        for(int i=0;i<3;i++){
            white[i] = 1.0;
            black[i] = 0.0;
        }
        if((i+j)%2==0) return black;
        return white;
    }

    void draw(){
        int in = 0, jn = 0;
        int cellX = (int)((curPosition.x+2000)/checkerBoardWidth);
        int cellY = (int)((curPosition.z+2000)/checkerBoardWidth);
        double xin = cellX*checkerBoardWidth;
        double yin = cellY*checkerBoardWidth;
        for(double i = -2000; i<2000.0; i+=checkerBoardWidth){
            jn = in;
            for(double j=-2000; j<2000; j+=checkerBoardWidth){
                glColor3d(jn&1, jn&1, jn&1);
                glPushMatrix();
                    // if((cellX+cellY)%2==0)
                    int movX=cellX-prevCellX, movY=cellY-prevCellY;
                    if(cellX%2!=prevCellX%2) movX=cellX-prevCellX-1;
                    if(cellY%2!=prevCellY%2) movY=cellY-prevCellY-1;
                    if(movX!=0 || movY!=0) {
                        // cout << "shift\n";
                        glTranslatef(movX*checkerBoardWidth, 0, movY*checkerBoardWidth);
                    }
                    glBegin(GL_QUADS);
                    glVertex3f(i,0,j);
                    glVertex3f(i,0,j+checkerBoardWidth);
                    glVertex3f(i+checkerBoardWidth,0,j+checkerBoardWidth);
                    glVertex3f(i+checkerBoardWidth,0,j);
                    glEnd();    
                glPopMatrix();
                jn++;
            }
            in++;
        }
        // prevCellX = cellX;
        // prevCellY = cellY;
    }
};
