#include<bits/stdc++.h>

using namespace std;

const double PI = acos(-1);

class Point{
    double x, y, z;
public:
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
    double x, y, z;
public:
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

    double getX(){
        return x;
    }

    double getY(){
        return y;
    }

    double getZ(){
        return z;
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

Vector rodriguesFormula(Vector x, Vector a, double angle){
    angle = angle*PI/180.0;
    Vector result = x * cos(angle) + a*(1-cos(angle))*x.dotProduct(a) + (a*x)*sin(angle);
    return result;
}

class Matrix{
    int rows;
    int columns;
    vector<vector<double>> matrix;
public:
 
    Matrix(int rows, int columns){
        this->rows = rows;
        this->columns = columns;
        matrix.resize(this->rows);
        for(int i = 0; i < this->rows; i++){
            matrix[i].resize(this->columns);
        }
    }

    Matrix(int dimension){
        this->rows = dimension;
        this->columns = dimension;
        matrix.resize(dimension);
        for(int i = 0; i < dimension; i++){
            matrix[i].resize(dimension);
        }
    }


    Matrix(Point pt){
        this->rows = 4;
        this->columns = 1;
        matrix.resize(this->rows);
        for(int i = 0; i < this->rows; i++){
            matrix[i].resize(this->columns);
        }
        matrix[0][0] = pt.getX();
        matrix[1][0] = pt.getY();
        matrix[2][0] = pt.getZ();
        matrix[3][0] = 1;
    }

    void set(int i, int j, double value){
        matrix[i][j] = value;
    }
    double get(int i, int j){
        return matrix[i][j];
    }

    Matrix operator*(Matrix &other){
        if(this->columns!=other.rows) exit(1);
        Matrix result(this->rows, other.columns);
        for(int i = 0; i < this->rows; i++){
            for(int j = 0; j < other.columns; j++){
                double sum = 0;
                for(int k = 0; k < this->columns; k++){
                    sum += matrix[i][k] * other.get(k, j);
                }
                result.set(i, j, sum);
            }
        }
        return result;
    }

    void createIdentityMatrix(){
        for(int i = 0; i < this->rows; i++){
            for(int j = 0; j < this->columns; j++){
                if(i==j) matrix[i][j] = 1.0;
                else matrix[i][j] = 0.0;
            }
        }
    }

    void createTranslationMatrix(double tx, double ty, double tz){
        createIdentityMatrix();
        matrix[0][this->columns-1] = tx;
        matrix[1][this->columns-1] = ty;
        matrix[2][this->columns-1] = tz;
    }

    void createScalingMatrix(double sx, double sy, double sz){
        createIdentityMatrix();
        matrix[0][0] = sx;
        matrix[1][1] = sy;
        matrix[2][2] = sz;
    }

    void createRotationMatrix(Vector a, Vector b, Vector c){
        createIdentityMatrix();
        matrix[0][0] = a.getX();
        matrix[1][0] = a.getY();
        matrix[2][0] = a.getZ();
        matrix[0][1] = b.getX();
        matrix[1][1] = b.getY();
        matrix[2][1] = b.getZ();
        matrix[0][2] = c.getX();
        matrix[1][2] = c.getY();
        matrix[2][2] = c.getZ();
    }

    void createProjectionTransformationMatrix(double near, double far, double t, double r){
        matrix[0][0] = near/r;
        matrix[1][1] = near/t;
        matrix[2][2] = -(far+near)/(far-near);
        matrix[3][2] = -1;
        matrix[2][3] = -(2*far*near)/(far-near); 
    }

    void createRotationMatrix(double angle, double ax, double ay, double az){
        Vector a(ax, ay, az);
        a.normalize();
        Vector i(1.0,0.0,0.0);
        Vector j(0.0,1.0,0.0);
        Vector k(0.0,0.0,1.0);

        Vector c1 = rodriguesFormula(i,a,angle);
        Vector c2 = rodriguesFormula(j,a,angle);
        Vector c3 = rodriguesFormula(k,a,angle);
        this->createRotationMatrix(c1,c2,c3);
    }

    friend ostream& operator<<(ostream &output, Matrix &matrix){
        for(int i = 0; i < matrix.rows; i++){
            for(int j = 0; j < matrix.columns; j++){
                output << matrix.get(i, j) << " ";
            }
            output << endl;
        }
        return output;
    }
};

Point transformPoint(Matrix &transformation_matrix, Point &point){
    Matrix point_matrix(point);
    Matrix result = transformation_matrix * point_matrix;
    Point transformed_point(result.get(0, 0), result.get(1, 0), result.get(2, 0));
    transformed_point = transformed_point * (1.0/result.get(3, 0));
    
    return transformed_point;
}

stack<Matrix>transformation_matrices;

int main(int argc, char *argv[]){
    ifstream fin(argv[1]);
    ofstream fout("stage1.txt");
    fout << setw(7) << fixed << setprecision(7);
    Point eye, look;
    Vector up;
    fin >> eye >> look >> up;
    double fovY, aspectRatio, near, far;
    fin >> fovY >> aspectRatio >> near >> far;
    int triangles = 0;
    
    // stage 1: Modeling Transformation

    Matrix identity_matrix(4);
    identity_matrix.createIdentityMatrix();
    transformation_matrices.push(identity_matrix);
    Matrix current_transformation_matrix = identity_matrix;
    string cmd;
    while(true){
        fin >> cmd;
        if(cmd=="triangle"){
            triangles++;
            Point p1,p2,p3;
            fin >> p1 >> p2 >> p3;
            p1 = transformPoint(current_transformation_matrix, p1);
            p2 = transformPoint(current_transformation_matrix, p2);
            p3 = transformPoint(current_transformation_matrix, p3);
            fout  << p1 << p2 << p3 << endl;
        }
        else if(cmd=="translate"){
            double tx, ty, tz;
            fin >> tx >> ty >> tz;
            Matrix translation_matrix(4);
            translation_matrix.createTranslationMatrix(tx, ty, tz);
            current_transformation_matrix = current_transformation_matrix * translation_matrix;
        }
        else if(cmd=="scale"){
            double sx, sy, sz;
            fin >> sx >> sy >> sz;
            Matrix scaling_matrix(4);
            scaling_matrix.createScalingMatrix(sx, sy, sz);
            current_transformation_matrix = current_transformation_matrix * scaling_matrix;
        }
        else if(cmd=="rotate"){
            double angle, ax, ay, az;
            fin >> angle >> ax >> ay >> az;
            Matrix rotation_matrix(4);
            rotation_matrix.createRotationMatrix(angle, ax, ay, az);
            current_transformation_matrix = current_transformation_matrix * rotation_matrix;
        }
        else if(cmd=="push"){
            transformation_matrices.push(current_transformation_matrix);
        }
        else if(cmd=="pop"){
            current_transformation_matrix = transformation_matrices.top();
            transformation_matrices.pop();

        }
        if(cmd=="end") break;
    }
    fin.close();
    fout.close();

    // stage 2: View Transformation

    Vector l(eye, look);
    l.normalize();
    Vector r = l * up;
    r.normalize();
    Vector u = r * l;
    Matrix view_translation_matrix(4);
    view_translation_matrix.createTranslationMatrix(-eye.getX(), -eye.getY(), -eye.getZ());
    Matrix view_rotation_matrix(4);
    view_rotation_matrix.createRotationMatrix(r,u,-l);
    Matrix view_transformation_matrix = view_rotation_matrix * view_translation_matrix;
    fin.open("stage1.txt");
    fout.open("stage2.txt");
    for(int i=1;i<=triangles;i++){
        Point p1,p2,p3;
        fin >> p1 >> p2 >> p3;
        p1 = transformPoint(view_transformation_matrix, p1);
        p2 = transformPoint(view_transformation_matrix, p2);
        p3 = transformPoint(view_transformation_matrix, p3);
        fout << p1 << p2 << p3 << endl;
    }
    fin.close();
    fout.close();

    // stage 3: Projection Transformation

    fin.open("stage2.txt");
    fout.open("stage3.txt");
    double fovX = fovY * aspectRatio;
    double t = near * tan(fovY*PI/360.0);
    double s = near * tan(fovX*PI/360.0);
    Matrix projection_transformation_matrix(4);
    projection_transformation_matrix.createProjectionTransformationMatrix(near, far, t, s);
    for(int i=0;i<triangles;i++){
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        p1 = transformPoint(projection_transformation_matrix, p1);
        p2 = transformPoint(projection_transformation_matrix, p2);
        p3 = transformPoint(projection_transformation_matrix, p3);
        fout << p1 << p2 << p3 << endl;
    }
    fin.close();
    fout.close();
    return 0;
}