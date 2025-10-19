#include <iostream>
using namespace std;

// Base (parent) class
class Animal {
public:
    string name;

    void eat() {
        cout << name << " is eating." << endl;
    }
};

// Derived (child) class
class Dog : public Animal {
public:
    void bark() {
        cout << name << " is barking." << endl;
    }
};

int main() {
    Dog dog;
    dog.name = "Buddy";

    // inherited function
    dog.eat();

    // function from Dog itself
    dog.bark();

    return 0;
}
