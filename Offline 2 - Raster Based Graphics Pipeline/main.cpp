#include<bits/stdc++.h>
#include "bitmap_image.hpp"
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

static unsigned long int g_seed = 1;
inline int random_rgb(){
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}
class Triangle{
public:
    Point points[3];
    int color_RGB[3];
    Triangle(Point p1, Point p2, Point p3){
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
        for(int i=0;i<3;i++)
            color_RGB[i] = random_rgb() % 256;
    }
    Triangle(){
        for(int i=0;i<3;i++)
            color_RGB[i] = random_rgb() % 256;
    }

    Point getP1(){
        return points[0];
    }
    Point getP2(){
        return points[1];
    }
    Point getP3(){
        return points[2];
    }

    friend istream& operator>>(istream &input, Triangle &triangle){
        input >> triangle.points[0] >> triangle.points[1] >> triangle.points[2];
        return input;
    }

    friend ostream& operator<<(ostream &output, Triangle &triangle){
        output << triangle.points[0] << triangle.points[1] << triangle.points[2] << endl;
        // output << "color:\n";
        // for(int i=0;i<3;i++)
        //     output << triangle.color_RGB[i] << " ";
        // output << endl;
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

int main(){
    ifstream fin("scene.txt");
    ofstream fout("stage1.txt");
    fout << setw(7) << fixed << setprecision(7);
    Point eye, look;
    Vector up;
    fin >> eye >> look >> up;
    double fovY, aspectRatio, near, far;
    fin >> fovY >> aspectRatio >> near >> far;
    int triangle_count = 0;
    
    // stage 1: Modeling Transformation

    Matrix identity_matrix(4);
    identity_matrix.createIdentityMatrix();
    transformation_matrices.push(identity_matrix);
    Matrix current_transformation_matrix = identity_matrix;
    string cmd;
    while(true){
        fin >> cmd;
        if(cmd=="triangle"){
            triangle_count++;
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
    for(int i=1;i<=triangle_count;i++){
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
    for(int i=0;i<triangle_count;i++){
        Point p1, p2, p3;
        fin >> p1 >> p2 >> p3;
        p1 = transformPoint(projection_transformation_matrix, p1);
        p2 = transformPoint(projection_transformation_matrix, p2);
        p3 = transformPoint(projection_transformation_matrix, p3);
        fout << p1 << p2 << p3 << endl;
    }
    fin.close();
    fout.close();

    // stage 4: clipping and z-buffering

    int screen_width, screen_height;
    ifstream configFile("config.txt");
    configFile >> screen_width >> screen_height;
    fin.open("stage3.txt");
    fout.open("z_buffer.txt");
    vector<Triangle> triangles;
    for(int i=0;i<triangle_count;i++){
        Triangle triangle;
        fin >> triangle;
        triangles.push_back(triangle);
    }
    double right_limit = 1;
    double left_limit = -1;
    double top_limit = 1;
    double bottom_limit = -1;

    double dx = (right_limit - left_limit) / screen_width;
    double dy = (top_limit - bottom_limit) / screen_height;
    double top_y = top_limit - dy/2;
    double left_x = left_limit + dx/2;
    double max_z = 1;
    double min_z = -1;
    vector<vector<double> >z_buffer(screen_height, vector<double>(screen_width, max_z));
    bitmap_image image(screen_width, screen_height);
    for(int i=0;i<screen_height;i++){
        for(int j=0;j<screen_width;j++){
            image.set_pixel(j, i, 0, 0, 0);
        }
    }

    for(int t_i=0;t_i<triangles.size();t_i++){
        // cout << triangles[t_i];
        double mx_y = max(triangles[t_i].getP1().getY(), 
                        max(triangles[t_i].getP2().getY(), 
                            triangles[t_i].getP3().getY()));
        double mn_y = min(triangles[t_i].getP1().getY(),
                        min(triangles[t_i].getP2().getY(), 
                            triangles[t_i].getP3().getY()));
        double top_scan_y = min(top_y, mx_y);
        double bottom_scan_y = max(-top_y, mn_y);
        int top_scan_y_index = round(abs(top_scan_y - top_y) / dy);
        int bottom_scan_y_index = round(abs(bottom_scan_y - top_y) / dy);
        // cout << mx_y << " " << mn_y << endl;
        // cout << top_scan_y_index << " " << bottom_scan_y_index << endl;
        for(int y_i=top_scan_y_index; y_i<=bottom_scan_y_index; y_i++){
            vector<double>x_intersections, z_intersections;
            double ys = top_y - y_i*dy;
            for(int j=0;j<3;j++){
                int k = (j+1)%3;
                // cout << triangles[i].points[j];
                // cout << triangles[i].points[k];
                if(triangles[t_i].points[j].getY() == triangles[t_i].points[k].getY()) continue;
                if(triangles[t_i].points[j].getY() < ys && triangles[t_i].points[k].getY() < ys) continue;
                if(triangles[t_i].points[j].getY() > ys && triangles[t_i].points[k].getY() > ys) continue;
                double xs = triangles[t_i].points[j].getX() + (ys - triangles[t_i].points[j].getY()) * 
                            (triangles[t_i].points[k].getX() - triangles[t_i].points[j].getX()) / 
                            (triangles[t_i].points[k].getY() - triangles[t_i].points[j].getY());
                double zs = triangles[t_i].points[j].getZ() + (ys - triangles[t_i].points[j].getY()) * 
                            (triangles[t_i].points[k].getZ() - triangles[t_i].points[j].getZ()) / 
                            (triangles[t_i].points[k].getY() - triangles[t_i].points[j].getY());
                if(xs >= -left_x) xs = -left_x;
                else if(xs <= left_x) xs = left_x;
                x_intersections.push_back(xs);  
                z_intersections.push_back(zs);           
            }
            if(x_intersections.size()!=2) continue;
            int right_scan_x_index = round((max(x_intersections[0], x_intersections[1]) - left_x) / dx);
            int left_scan_x_index = round((min(x_intersections[0], x_intersections[1]) - left_x) / dx);
            for(int x_i = left_scan_x_index; x_i<=right_scan_x_index;x_i++){
                double xa = left_x + x_i*dx;    
                double za = z_intersections[0] + (xa - x_intersections[0]) * 
                            (z_intersections[1] - z_intersections[0]) / 
                            (x_intersections[1] - x_intersections[0]);
                if(za < min_z) continue;
                if(z_buffer[y_i][x_i] > za){
                    z_buffer[y_i][x_i] = za;
                    image.set_pixel(x_i, y_i, triangles[t_i].color_RGB[0], triangles[t_i].color_RGB[1], triangles[t_i].color_RGB[2]);
                }
            }
        }
    }
    fout << setw(6) << fixed << setprecision(6);

    for(int i=0;i<screen_height;i++){
        for(int j=0;j<screen_width;j++){
            if(z_buffer[i][j] < max_z){
                fout << z_buffer[i][j] << "\t";
            }
        }
        fout << endl;
    }
    

    string file_name = "out.bmp";
    image.save_image(file_name);
    z_buffer.clear();
    return 0;
}