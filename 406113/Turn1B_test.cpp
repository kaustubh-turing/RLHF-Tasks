#include <iostream>
#include <type_traits>

template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type 
square(T x) {
    return x * x;
}

template <typename T>
typename std::enable_if<!std::is_integral<T>::value, void>::type 
square(T x) {
    static_assert(std::is_integral<T>::value, 
                  "Function square is only defined for integral types");
}

int main() {
    int a = 5;
    double b = 6.0;

    std::cout << "Square of " << a << " is: " << square(a) << std::endl;
    
    // Uncommenting the following line will cause a compile-time error
    // square(b);  // Error: Function square is only defined for integral types

    return 0;
} 