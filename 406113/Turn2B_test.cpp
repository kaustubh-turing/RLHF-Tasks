#include <iostream>
#include <type_traits>

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type 
square(T x) {
    return x * x;
}

template<typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type 
square(T x) {
    std::cout << "Square operation not supported for non-integral types" << std::endl;
}

int main() {
    int a = 5;
    double b = 6.0;

    std::cout << "Square of " << a << " is: " << square(a) << std::endl;
    square(b);  // Will print error message

    return 0;
} 