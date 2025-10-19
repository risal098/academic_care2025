#include <iostream>
using namespace std;


// Define an enum for menu choices
enum Menu {
    START  ,
    SETTINGS,
    EXIT
};

int main() {
    int choice;

    cout << "=== Main Menu ===\n";
    cout << "1. Start Game\n";
    cout << "2. Settings\n";
    cout << "3. Exit\n";
    cout << "Choose: ";
    cin >> choice;

    // Convert int to enum
    Menu menuChoice = static_cast<Menu>(choice-1);

    switch (menuChoice) {
        case START:
            cout << "Game starting..." << endl;
            break;
        case SETTINGS:
            cout << "Opening settings..." << endl;
            break;
        case EXIT:
            cout << "Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice!" << endl;
            break;
    }

    return 0;
}
