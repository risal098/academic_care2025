#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>

using namespace std;

class User{
private:
    friend class boost::serialization::access; // Required by Boost

    string name;
    string phone;
    int loginState;
    int id; //0 = out, 1= login
    static inline int sequenceCounter;
    vector<User> contact;

     // The core serialization function
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        // Simply list all members to be serialized
        ar & name;
        ar & phone;
        ar & loginState;
        ar & id;
        ar & contact;
    }

    bool checkExistingContact(string phone){
        for(auto user: contact){
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
        sequenceCounter++;
        loginState = 0;
        contact = vector<User>();
    }

    void addContactPrompt(){
        string name, phone;
        bool isFail = false;
        while(true){
            cout << "Add new contact name: ";
            cin >> name;
            cout << "Add " << name << " phone: ";
            cin >> phone;
            //check phone number duplicate
            isFail = checkExistingContact(phone);
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
        if(contact.size() == 0){
            cout << "No contact found!" << endl;
            return;
        }
        for(auto user: contact){
            cout << i << ", " << user.id << ". " << user.name << ", " << user.phone << endl;
        }
        cout << endl;
    }

    bool checkUserExist(string phone){
        for(auto user: contact){
            if(user.getPhone() == phone){
                return true;
            }
        }
        return false;
    }

    User seekContact(string phone){
        User foundUser;
        for(auto user: contact){
            if(user.getPhone() == phone){
                foundUser = user;
                return foundUser;
            }
        }
        //in reality we won't expect this to be executed
        return User();
    }

    void updateContact(int id, User other){
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
        ofstream file(filename, ios::binary);
        if (!file.is_open()) {
            cerr
                << "Error: Failed to open file for writing."
                << endl;
            return;
        }
        file.write(reinterpret_cast<const char*>(this),
                   sizeof(*this));
        file.close();
        cout << "Object serialized successfully." << endl;
    } 

    //  Function for Deserialization
    static User deserialize(const string& filename)
    {
        User obj("", 0);
        obj.setLoginState(0);
        obj.contact = vector<User>();
        ifstream file(filename, ios::binary);
        if (!file.is_open()) {
            cerr
                << "Error: Failed to open file for reading."
                << endl;
            return obj;
        }
        file.read(reinterpret_cast<char*>(&obj),
                  sizeof(obj));
        file.close();
        cout << "Object deserialized successfully." << endl;
        return obj;
    }
    

    string getName(){return name;};
    string getPhone(){return phone;};
    void setName(string name){this->name = name;};
    void setPhone(string phone){this->phone = phone;};
    int getLoginState(){return loginState;};
    void setLoginState(int state){ loginState = state;};
    static int getId(){return sequenceCounter;};
    static void resetId(){
        sequenceCounter = 100;
    }
};