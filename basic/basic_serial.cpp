#include <iostream>
#include <vector>
#include <fstream>
#include <string>

// --- include Boost headers ---
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>   // 🔥 needed for std::vector
#include <boost/serialization/string.hpp>   // 🔥 needed for std::string

using namespace std;

// --- Human class ---
class Human {
public:
    string name;
    int age;
    int tangan;

    Human() = default;
    Human(string n, int a) : name(n), age(a) {}

private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & name;
        ar & age;
        ar & tangan;
    }
};

// --- Save function ---
bool saveHumans(const vector<Human> &humans, const string &filename) {
    ofstream ofs(filename); //buka file
    if (!ofs.is_open()) return false; //kalo ga kebuka return false
    boost::archive::text_oarchive oa(ofs); //disiapin
    oa << humans;  // serialize the entire vector
    return true;
}

// --- Load function ---
vector<Human> loadHumans(const string &filename) {
    vector<Human> humans;
    ifstream ifs(filename);
    if (!ifs.is_open()) return humans;

    boost::archive::text_iarchive ia(ifs);
    ia >> humans;  // deserialize into vector
    return humans;
}

// --- Main ---
int main() {
    vector<Human> people = {
        {"Alices cantik", 25},
        {"Bob", 30},
        {"Charlie", 22}
    };

    cout << "Saving humans..." << endl;
    if (saveHumans(people, "humans.dat"))
        cout << "✅ Saved successfully.\n";
    else
        cout << "❌ Failed to save.\n";

    cout << "Loading humans..." << endl;
    vector<Human> loaded = loadHumans("humans.dat");

    cout << "\n=== Loaded Humans ===\n";
    for (const auto &h : loaded)
        cout << "Name: " << h.name << ", Age: " << h.age << endl;

    return 0;
}
