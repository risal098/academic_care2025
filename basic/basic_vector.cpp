#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <optional>

using namespace std;

// Simple user type used in examples
struct User {
    int id;
    string name;
    string phone;

    User() = default;
    User(int i, string n, string p) : id(i), name(move(n)), phone(move(p)) {}
};

// Generator for unique ids (inline static since C++17)
inline int next_user_id() {
    static int id = 100; // starting id
    return id++;
}

// Create (append) a new user
void create_user(vector<User>& users, string name, string phone) {
    users.emplace_back(next_user_id(), move(name), move(phone)); // in-place construction
}

// Read: print all users
void list_users(const vector<User>& users) {
    cout << "Users (count=" << users.size() << ", capacity=" << users.capacity() << ")\n";
    for (const auto& u : users) {
        cout << "  id=" << u.id << " name=\"" << u.name << "\" phone=\"" << u.phone << "\"\n";
    }
    cout << '\n';
}

// Helper: find user index by id (returns -1 if not found)
int find_user_index(const vector<User>& users, int id) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

// Read: find and return a copy (optional)
optional<User> find_user(const vector<User>& users, int id) {
    int idx = find_user_index(users, id);
    if (idx < 0) return nullopt;
    return users[idx];
}

// Update: modify user fields by id
bool update_user(vector<User>& users, int id, const string& new_name, const string& new_phone) {
    int idx = find_user_index(users, id);
    if (idx < 0) return false;
    users[idx].name = new_name;
    users[idx].phone = new_phone;
    return true;
}

// Delete: remove user by id
bool delete_user(vector<User>& users, int id) {
    int idx = find_user_index(users, id);
    if (idx < 0) return false;
    users.erase(users.begin() + idx); // O(n) operation
    return true;
}

// Demonstrate reserve, shrink_to_fit, swap and move semantics
void capacity_and_reserve_demo() {
    vector<User> v;
    cout << "Reserve demo: capacity growth\n";
    for (int i = 0; i < 6; ++i) {
        v.emplace_back(next_user_id(), "u" + to_string(i), "0" + to_string(i));
        cout << "  size=" << v.size() << " cap=" << v.capacity() << '\n';
    }
    cout << "Reserve to 32\n";
    v.reserve(32);
    cout << "  size=" << v.size() << " cap=" << v.capacity() << '\n';
    cout << "Shrink to fit\n";
    v.shrink_to_fit();
    cout << "  size=" << v.size() << " cap=" << v.capacity() << '\n';
}

// Demonstrate sort and algorithm usage
void sort_and_algorithms_demo(vector<User>& users) {
    cout << "Sort users by name\n";
    sort(users.begin(), users.end(), [](const User& a, const User& b){
        return a.name < b.name;
    });
    list_users(users);

    cout << "Find first user with phone starting with '08' using std::find_if\n";
    auto it = find_if(users.begin(), users.end(), [](const User& u){
        return !u.phone.empty() && u.phone[0] == '0';
    });
    if (it != users.end()) {
        cout << "  found id=" << it->id << " name=" << it->name << '\n';
    } else {
        cout << "  not found\n";
    }
    cout << '\n';
}

int main() {
    // Example vector to store users
    vector<User> users;
    users.reserve(8); // avoid reallocations while creating initial users

    // Create
    create_user(users, "Alice", "081234");
    create_user(users, "Bob", "082345");
    create_user(users, "Charlie", "083456");

    // Read
    list_users(users);

    // Update Bob's phone
    auto bob = find_user(users, users[1].id);
    if (bob) {
        cout << "Updating Bob's phone\n";
        update_user(users, bob->id, bob->name, "089999");
    }
    list_users(users);

    // Delete Charlie
    cout << "Deleting Charlie (id=" << users.back().id << ")\n";
    delete_user(users, users.back().id);
    list_users(users);

    // Insert a user at position 1
    cout << "Inserting Dina at index 1 using insert (copy) and emplace for in-place construction\n";
    users.insert(users.begin() + 1, User(next_user_id(), "Dina", "080000")); // insert copy/moved object
    list_users(users);

    // Emplace at front (inefficient for vector but valid)
    users.emplace(users.begin(), next_user_id(), "Eve", "080111");
    list_users(users);

    // Reserve, capacity, shrink demo
    capacity_and_reserve_demo();

    // Sorting and algorithms demo
    sort_and_algorithms_demo(users);

    // Swap and move semantics
    cout << "Swap demo: move users into new vector\n";
    vector<User> backup;
    backup = move(users); // users becomes empty, ownership moved
    cout << "After move: users.size=" << users.size() << " backup.size=" << backup.size() << '\n';
    // restore
    users = move(backup);

    // Clear all users
    cout << "Clearing all users\n";
    users.clear();
    cout << "Now size=" << users.size() << " capacity=" << users.capacity() << '\n';

    return 0;
}







/*
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <optional>

using namespace std;

// Simple user type used in examples
struct User {
    int id;
    string name;
    string phone;

    User() = default;
    User(int i, string n, string p) : id(i), name(move(n)), phone(move(p)) {}
};

// Generator for unique ids (inline static since C++17)
inline int next_user_id() {
    static int id = 100; // starting id
    return id++;
}

// Create (append) a new user
void create_user(vector<User>& users, string name, string phone) {
    users.emplace_back(next_user_id(), move(name), move(phone)); // in-place construction
}

// Read: print all users
void list_users(const vector<User>& users) {
    cout << "Users (count=" << users.size() << ", capacity=" << users.capacity() << ")\n";
    for (const auto& u : users) {
        cout << "  id=" << u.id << " name=\"" << u.name << "\" phone=\"" << u.phone << "\"\n";
    }
    cout << '\n';
}

// Helper: find user index by id (returns -1 if not found)
int find_user_index(const vector<User>& users, int id) {
    for (size_t i = 0; i < users.size(); ++i) {
        if (users[i].id == id) return static_cast<int>(i);
    }
    return -1;
}

// Read: find and return a copy (optional)
optional<User> find_user(const vector<User>& users, int id) {
    int idx = find_user_index(users, id);
    if (idx < 0) return nullopt;
    return users[idx];
}

// Update: modify user fields by id
bool update_user(vector<User>& users, int id, const string& new_name, const string& new_phone) {
    int idx = find_user_index(users, id);
    if (idx < 0) return false;
    users[idx].name = new_name;
    users[idx].phone = new_phone;
    return true;
}

// Delete: remove user by id
bool delete_user(vector<User>& users, int id) {
    int idx = find_user_index(users, id);
    if (idx < 0) return false;
    users.erase(users.begin() + idx); // O(n) operation
    return true;
}

// Demonstrate reserve, shrink_to_fit, swap and move semantics
void capacity_and_reserve_demo() {
    vector<User> v;
    cout << "Reserve demo: capacity growth\n";
    for (int i = 0; i < 6; ++i) {
        v.emplace_back(next_user_id(), "u" + to_string(i), "0" + to_string(i));
        cout << "  size=" << v.size() << " cap=" << v.capacity() << '\n';
    }
    cout << "Reserve to 32\n";
    v.reserve(32);
    cout << "  size=" << v.size() << " cap=" << v.capacity() << '\n';
    cout << "Shrink to fit\n";
    v.shrink_to_fit();
    cout << "  size=" << v.size() << " cap=" << v.capacity() << '\n';
}

// Demonstrate sort and algorithm usage
void sort_and_algorithms_demo(vector<User>& users) {
    cout << "Sort users by name\n";
    sort(users.begin(), users.end(), [](const User& a, const User& b){
        return a.name < b.name;
    });
    list_users(users);

    cout << "Find first user with phone starting with '08' using std::find_if\n";
    auto it = find_if(users.begin(), users.end(), [](const User& u){
        return !u.phone.empty() && u.phone[0] == '0';
    });
    if (it != users.end()) {
        cout << "  found id=" << it->id << " name=" << it->name << '\n';
    } else {
        cout << "  not found\n";
    }
    cout << '\n';
}

int main() {
    // Example vector to store users
    vector<User> users;
    users.reserve(8); // avoid reallocations while creating initial users

    // Create
    create_user(users, "Alice", "081234");
    create_user(users, "Bob", "082345");
    create_user(users, "Charlie", "083456");

    // Read
    list_users(users);

    // Update Bob's phone
    auto bob = find_user(users, users[1].id);
    if (bob) {
        cout << "Updating Bob's phone\n";
        update_user(users, bob->id, bob->name, "089999");
    }
    list_users(users);

    // Delete Charlie
    cout << "Deleting Charlie (id=" << users.back().id << ")\n";
    delete_user(users, users.back().id);
    list_users(users);

    // Insert a user at position 1
    cout << "Inserting Dina at index 1 using insert (copy) and emplace for in-place construction\n";
    users.insert(users.begin() + 1, User(next_user_id(), "Dina", "080000")); // insert copy/moved object
    list_users(users);

    // Emplace at front (inefficient for vector but valid)
    users.emplace(users.begin(), next_user_id(), "Eve", "080111");
    list_users(users);

    // Reserve, capacity, shrink demo
    capacity_and_reserve_demo();

    // Sorting and algorithms demo
    sort_and_algorithms_demo(users);

    // Swap and move semantics
    cout << "Swap demo: move users into new vector\n";
    vector<User> backup;
    backup = move(users); // users becomes empty, ownership moved
    cout << "After move: users.size=" << users.size() << " backup.size=" << backup.size() << '\n';
    // restore
    users = move(backup);

    // Clear all users
    cout << "Clearing all users\n";
    users.clear();
    cout << "Now size=" << users.size() << " capacity=" << users.capacity() << '\n';

    return 0;
}

*/