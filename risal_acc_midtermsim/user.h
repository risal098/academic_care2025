#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp> 
using namespace std;
inline bool save_users(const std::vector<class User>& users, const std::string& filename);
inline std::vector<class User> load_users(const std::string& filename);
class User{
private:
    friend class boost::serialization::access; // Required by Boost

    string name;
    string phone;
    int loginState;
    int id; //0 = out, 1= login
    int idconnect; //id of the user who owns this contact (buat format contact list)
    static inline int sequenceCounter;
    vector<User> contact;

     // The core serialization function
      template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        (void)version; // silence -Wunused-parameter when warnings are treated as errors
        // Simply list all members to be serialized
        ar & name;
        ar & phone;
        ar & loginState;
        ar & id;
        ar & contact;
    }
   
    bool validateContact(string phone,vector<User> users){
        bool isFound = false;
        contact = load_users( this->name + to_string(this->id) + this->phone + "_contact_data.bin");
        for(auto user: users){ //registered acc
            if(user.getPhone() == phone){
                cout << "Phone registered" << endl;
                isFound = true;
            }
        }
        cout<< "isFound status: " << isFound << endl;
        if(!isFound) return true;
        for(auto user: contact){ //duplicate account
            if(user.getPhone() == phone){
                cout << "Phone number already exists, use different one!" << endl;
                return true;
            }
        }

        return false;
    }

    bool addContact(User user){
        if(user.getId()!=0){
            contact.push_back(user);
            cout << user.name << " is successfully created" << endl;
            save_users(contact, this->name + to_string(this->id) + this->phone + "_contact_data.bin");
        }

        else{
            return false;
            cout << "fail to add new contact!" << endl;
        }
        return true;
    }

public:
    User(){
        sequenceCounter = 100;
        contact = vector<User>();
    }

    User(int id, string name, string phone): name(name), phone(phone) {
        this->id = id;
        loginState = 0;
        contact = vector<User>();
    }

    User(string name, string phone): name(name), phone(phone) {
        this->id = sequenceCounter;
        //sequenceCounter++;
        loginState = 0;
        contact = vector<User>();
    }

    void addContactPrompt(vector<User> users){
        string name, phone;
        bool isFail = false;
        while(true){
            cout << "Add new contact name: ";
            cin >> name;
            cout << "Add " << name << " phone: ";
            cin >> phone;
            //check phone number duplicate and is registerd
            isFail = validateContact(phone, users);
            if(isFail){
                char choice;
                cout << "Do you want to Repeat adding contact?, (y/n (will exit) ): ";
                cin >> choice;
                if(choice == 'y' || choice == 'Y') return;
                else continue;
            }
            break;
        }
        addContact(User(name, phone));
        cout << endl;
    }

    void printContact(){
        int i = 1;
        cout << "Contact List" << endl;
        contact = load_users( this->name + to_string(this->id) + this->phone + "_contact_data.bin");
        if(contact.size() == 0){
            cout << "No contact found!" << endl;
            return;
        }
        for(auto user: contact){
            cout << i << ", " << user.id << ". " << user.name << ", " << user.phone << endl;
        }
        cout << endl;
    }

    bool checkUserExist(string query){
        contact = load_users( this->name + to_string(this->id) + this->phone + "_contact_data.bin");
        cout<< "Searching for contact with query: " << query << endl;
        for(auto user: contact){
            //cout << "Checking user: " << user.getName() << ", " << user.getPhone() << ", " << user.getThisId() << query<<endl;
            if(user.getPhone() == query || user.getName() == query || to_string(user.getId()) == query){
                return true;
            }
        }
        return false;
    }

    User seekContact(string query){
        contact = load_users( this->name + to_string(this->id) + this->phone + "_contact_data.bin");
        User foundUser;
        for(auto user: contact){
            cout << "Checking user: " << user.getName() << ", " << user.getPhone() << ", " << user.getThisId() << query<<endl;
            if(user.getPhone() == query || user.getName() == query || to_string(user.getId()) == query){
                foundUser = user;
                return foundUser;
            }
        }
        //in reality we won't expect this to be executed
        return User();
    }

    void updateContact(int id, User other){
        contact = load_users( this->name + to_string(this->id) + this->phone + "_contact_data.bin");
        for(auto &user: contact){
            if(user.getId() == id){
                user = other;
                cout << "Contact has been updated: " << id << endl;
                return;
            }
        }
        cout << "Contact with id: " << id << " is not found!" << endl;
        return;
    }

     //  Function for Serialization
    void serialize(const string& filename)
    {
        ofstream ofs(filename);
        if (!ofs.is_open()) {
            cerr << "Error: Failed to open file for writing." << endl;
            return;
        }
        boost::archive::text_oarchive oa(ofs);
        oa << *this;
        cout << "Object serialized successfully." << endl;
    } 

    //  Function for Deserialization using Boost.Serialization
    static User deserialize(const string& filename)
    {
        User obj; // default-constructed
        ifstream ifs(filename);
        if (!ifs.is_open()) {
            cerr << "Error: Failed to open file for reading." << endl;
            return obj;
        }
        boost::archive::text_iarchive ia(ifs);
        ia >> obj;
        cout << "Object deserialized successfully." << endl;
        return obj;
    }
    

    string getName(){return name;};
    string getPhone(){return phone;};
    int getThisId(){return id;};
    void setName(string name){this->name = name;};
    void setPhone(string phone){this->phone = phone;};
    int getLoginState(){return loginState;};
    void setLoginState(int state){ loginState = state;};
    static int getId(){return sequenceCounter;};
        static void setNextId(int nextId){ sequenceCounter = nextId;};
    static void resetId(){
        sequenceCounter = 100;
    }
};

// Save a list of users
inline bool save_users(const std::vector<User>& users, const std::string& filename) { //argumen users
    std::ofstream ofs(filename, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open()) return false;
    boost::archive::text_oarchive oa(ofs);
    oa << users;
    // ensure stream is good
    return ofs.good();
}

// Load a list of users
inline std::vector<User> load_users(const std::string& filename) {
    std::vector<User> users;
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs.is_open()) return users; // file missing -> return empty list
    // also check file size zero
    ifs.seekg(0, std::ios::end);
    if (ifs.tellg() <= 0) return users;
    ifs.seekg(0, std::ios::beg);

    try {
        boost::archive::text_iarchive ia(ifs);
        ia >> users;
    } catch (const boost::archive::archive_exception &e) {
        // failed to deserialize -> return empty list
        std::cerr << "Warning: failed to load users: " << e.what() << std::endl;
        users.clear();
    }
    return users;
}


// Helper: return maximum user.id in a vector (0 if empty)
inline int max_user_id(const std::vector<User>& users) {
    int maxid = 0;
    for (auto u : users) {
        maxid = std::max(maxid, u.getThisId());
        //cout<< "Existing user id: " << u.getThisId() <<" name=" << u.getName() <<endl;
    }
    return maxid;
}