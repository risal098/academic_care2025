#include <iostream>

class Counter {
private:
    static inline int value = 0; // C++17 inline static member
public:
    static int get()  { return value; }
    static void set(int v)  { value = v; }
    static void increment(int delta = 1){ value += delta; }
    static void reset()  { value = 0; }
};

int main() {
    std::cout << "Initial: " << Counter::get() << '\n';
    Counter::set(42);
    std::cout << "After set: " << Counter::get() << '\n';
    Counter::increment();
    std::cout << "After increment: " << Counter::get() << '\n';
    Counter::increment(8);
    std::cout << "After +8: " << Counter::get() << '\n';
    Counter::reset();
    std::cout << "After reset: " << Counter::get() << '\n';
    return 0;
}
