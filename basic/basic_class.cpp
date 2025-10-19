#include <iostream>
using namespace std;


class Human{
    private:
        string aib="mabok";
    public:
    int mata;
    int tangan;
    void ngomong(){
        cout << "Hello World!" << endl;
    }
    void ngomongaib(){
        cout << "My aib is " << aib << endl;
    }
};


class Point {
private:
    int z; 

public:
    int x;
    int y; 
    
    void setZ(int value) {
        z = value;
    }
    int getZ() {
        return z;
    }
};
int main() {
    Human h1;
    h1.mata = 2;
    h1.tangan = 2;

    cout << "Human h1: (" << h1.mata << ", " << h1.tangan << ")" << endl;
    h1.ngomong();
    h1.ngomongaib();
    Point p1; // Declare a Point class object
    p1.x = 10; // Assign value to x
    p1.y = 20; // Assign value to y
    p1.setZ(30); // Assign value to z
    cout << "Point p1: (" << p1.x << ", " << p1.y << ", " << p1.getZ() << ")" << endl;

    Point p2; // Declare another Point class object
    p2.x = 30; // Assign value to x
    p2.y = 40; // Assign value to y
    p2.setZ(50); // Assign value to z
    cout << "Point p2: (" << p2.x << ", " << p2.y << ", " << p2.getZ() << ")" << endl;

    return 0;
}





