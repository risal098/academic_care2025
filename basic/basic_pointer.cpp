
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

// --- Human class ---
class Human {
public:
    string name;
    int age;

    Human(string n, int a) : name(n), age(a) {}
};

int main(){
    vector<int> people = {
       1,2,3,4,5
    };
    for ( auto &person : people) {
        cout<<person<<endl;
    }
    return 0;
}






