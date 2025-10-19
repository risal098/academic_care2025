#include <iostream>
#include <regex>
#include "user.h"

using namespace std;

enum PrimaryPrompt{LOGIN, REGISTER, EXIT, MAIN_PROMPT};
enum subUserPrompt{USER_VERIFICATION, LOGIN_MENU};
enum featurePrompt{WHOAMI, LIST_CONTACT, ADD_CONTACT, SEEK_CONTACT, LOGOUT, LOGIN_MENU_PROMPT};

void editPrompt(User &other, int id){
    cout << "Do you want to edit contact with id: " << id << " ? (y/n): ";
    char choice;
    cin >> choice;
    if(choice == 'n' || choice == 'N') return ;
    cout << endl;
    //proceed to edit
    string name, phone;
    cout << "Enter updated name: "; cin >> name;
    cout << "Enter updated phone: "; cin >> phone;

    other.setName(name);
    other.setPhone(phone);

    //assume the contact is found
    cout << "Contact has been updated: " << id << endl;
    return;
}

int main() {
    PrimaryPrompt prompt = MAIN_PROMPT;
    subUserPrompt sub_login_prompt = USER_VERIFICATION;
    featurePrompt login_menu_prompt = LOGIN_MENU_PROMPT;
    const string serialname = "user_data.bin";

    //reset static variables to 0
    User::resetId();

    vector<User> users;
    //create a default user for testing
    User user;
    //test case
    user = User("test", "123");
    users.push_back(user);

    //reload from serialized data
    //user = User::deserialize(serialname);
    while (true) {
        switch (prompt) {
            case MAIN_PROMPT:
                cout << "Select an option: " << endl;
                cout << "1. Login" << endl;
                cout << "2. Register" << endl;
                cout << "3. Exit" << endl;
                int choice;
                cin >> choice;
                prompt = static_cast<PrimaryPrompt>(choice - 1);
                break;
            case LOGIN:{
                // login is prompting entering name and phone number to confirmation
                //cout << "Login selected." << endl;
                switch (sub_login_prompt){
                    case USER_VERIFICATION:{
                        string name, phone;
                        cout << "Please enter name and phone number for Login." << endl;
                        cout << "Name: "; cin >> name;
                        cout << "Phone: "; cin >> phone;
                        // refactor this block by searching the user from the user list
                        bool isFound = false;
                        for(auto u: users){
                            if((u.getName() == name) & (phone == u.getPhone())){
                                user = u;
                                isFound = true;
                                break;
                            }
                        }

                        if(!isFound){
                            cout << "entered info is not recognized!" << endl;
                            prompt = MAIN_PROMPT;
                            sub_login_prompt = USER_VERIFICATION;
                        } else {
                            sub_login_prompt = LOGIN_MENU;
                            user.setLoginState(1);
                            cout << "Login successful!" << endl;
                            sub_login_prompt = LOGIN_MENU;
                            prompt = LOGIN;
                        }
                        break;
                        //check the entered data
                        /*
                        if ((user.getName() == name) & (phone == user.getPhone())){
                            sub_login_prompt = LOGIN_MENU;
                            user.setLoginState(1);
                        } else{
                            cout << "entered info is not recognized!" << endl;
                        }
                        break;
                        */
                    }
                    case LOGIN_MENU:{
                        //check login state
                        switch(login_menu_prompt){
                            case LOGIN_MENU_PROMPT:
                                if(user.getLoginState()==0){
                                    //ask for login
                                    sub_login_prompt = USER_VERIFICATION;
                                    break;
                                }

                                //TODO: Must check user login state
                                cout << "Please select the following" << endl;
                                cout << "1. WHO AM I" << endl;
                                cout << "2. List contact" << endl;
                                cout << "3. Add contact" << endl;
                                cout << "4. Seek contact" << endl;
                                cout << "5. Logout" << endl;
                                cin >> choice;
                                login_menu_prompt = static_cast<featurePrompt>(choice - 1);
                                //cout << "promptId: " << login_menu_prompt << endl;
                                break;
                            case WHOAMI:
                                cout << "Name: " << user.getName() << endl;
                                cout << "Phone: " << user.getPhone() << endl;
                                login_menu_prompt = LOGIN_MENU_PROMPT;
                                cout << endl;
                                break;
                            case LIST_CONTACT:
                                user.printContact();
                                login_menu_prompt = LOGIN_MENU_PROMPT;
                                break;
                            case ADD_CONTACT:
                                user.addContactPrompt();
                                login_menu_prompt = LOGIN_MENU_PROMPT;
                                break;
                            case SEEK_CONTACT:{
                                string phone;
                                cout << "Please enter phone number to search: ";
                                cin >> phone;
                                bool isExist = user.checkUserExist(phone);
                                if(isExist){
                                    User foundUser = user.seekContact(phone);
                                    cout << "Contact found: " << foundUser.getId() << ", " << foundUser.getName() << endl;
                                    editPrompt(foundUser, foundUser.getId());
                                    //update the contact on vector
                                    user.updateContact(foundUser.getId(), foundUser);
                                } else {
                                    cout << "Contact with phone number " << phone << " is not found." << endl;
                                }
                                cout << endl;
                                login_menu_prompt = LOGIN_MENU_PROMPT;
                                break;
                            }
                            case LOGOUT:
                                //have to seek the user from the user list and update the login state
                                for(auto &u: users){
                                if((u.getName() == user.getName()) & (user.getPhone() == u.getPhone()))
                                    u.setLoginState(0);
                                }

                                prompt = MAIN_PROMPT;
                                sub_login_prompt = USER_VERIFICATION;
                                login_menu_prompt = LOGIN_MENU_PROMPT;

                                cout << "You have been logged out!" << endl;
                                break;
                        }
                    }
                }
                break;
            }
            case REGISTER:{
                // register ask for the user internal data, then immediately make the current user login
                string name, phone;
                cout << "Please enter name and phone number for registration." << endl;
                cout << "Name: "; cin >> name;
                cout << "Phone: "; cin >> phone;
                user = User(name, phone);
                //add to user list
                users.push_back(user);

                //after register set the state to LOGIN for the other menu
                prompt = LOGIN;
                sub_login_prompt = LOGIN_MENU;
                cout << "Registration is successful, please login to continue" << endl;
                break;
            }
            case EXIT:
                //user.serialize(serialname);
                return 1;
        }
    }

    return 0;
}