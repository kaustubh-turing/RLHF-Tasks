#include <iostream>
#include <type_traits>

// Helper type trait to detect if a type has print() method
template<typename T, typename = void>
struct has_print_method : std::false_type {};

template<typename T>
struct has_print_method<T, 
    std::void_t<decltype(std::declval<T>().print())>> 
    : std::true_type {};

// Function for types with print() method
template<typename T>
typename std::enable_if_t<has_print_method<T>::value, void>
printValue(const T& value) {
    value.print();
}

// Function for types without print() method
template<typename T>
typename std::enable_if_t<!has_print_method<T>::value, void>
printValue(const T& value) {
    std::cout << value << std::endl;
}

// A class with a 'print()' method
class HasPrint {
public:
    HasPrint(int val) : data_(val) {}
    
    void print() const {
        std::cout << "Printing from HasPrint: " << data_ << std::endl;
    }

private:
    int data_;
};

// A class without a 'print()' method
class NoPrint {
public:
    NoPrint(int val) : data_(val) {}
    
    friend std::ostream& operator<<(std::ostream& os, const NoPrint& np) {
        return os << np.data_;
    }

private:
    int data_;
};

int main() {
    HasPrint hp{42};
    NoPrint np{1337};
    int i = 10;

    printValue(hp);   // Uses print() method
    printValue(np);   // Uses operator<<
    printValue(i);    // Uses operator<<

    return 0;
} 