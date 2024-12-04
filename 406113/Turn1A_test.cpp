#include <iostream>
#include <type_traits>

template <typename T, typename Enable = void>
class ConditionalSolver {
public:
    T solve() const {
        return T(0);
    }
};

template <typename T>
class ConditionalSolver<T, std::enable_if_t<std::is_integral_v<T>>> {
public:
    explicit ConditionalSolver(T value) : val(value) {}

    T solve() const {
        return val;
    }

private:
    T val;
};

int main() {
    int ival = 10;
    ConditionalSolver<int> solInt(ival);
    ConditionalSolver<double> solDouble;

    std::cout << "Result for integral type (int): " << solInt.solve() << std::endl;
    std::cout << "Result for non-integral type (double): " << solDouble.solve() << std::endl;

    return 0;
} 