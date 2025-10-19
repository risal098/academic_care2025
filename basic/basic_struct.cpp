#include <iostream>
using namespace std;


struct Human {
    int mata;
    int tangan;
};

int main() {
    Human h1; // Declare a Human structure variable
    h1.mata = 2; // Assign value to mata
    h1.tangan = 2; // Assign value to tangan

    cout << "Human h1: (" << h1.mata << ", " << h1.tangan << ")" << endl;

    Human h2 = {3, 2}; // Initialize a Human structure variable
    cout << "Human h2: (" << h2.mata << ", " << h2.tangan << ")" << endl;

    return 0;
}