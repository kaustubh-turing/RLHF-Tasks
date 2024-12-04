#include <iostream>

class Base {
public:
    Base() {
        std::cout << "Base constructed" << std::endl;
    }
    
    // Test Case 1: With virtual destructor
    virtual ~Base() {
        std::cout << "Base destructor" << std::endl;
    }
    
    // Test Case 2: Comment out virtual to demonstrate the issue
    /*
    ~Base() {
        std::cout << "Base destructor" << std::endl;
    }
    */
};

class Derived : public Base {
public:
    Derived() {
        std::cout << "Derived constructed" << std::endl;
        array = new int[10]; // Simulate dynamic resource allocation
    }

    ~Derived() override {
        std::cout << "Derived destructor" << std::endl;
        delete[] array;
    }

private:
    int* array;
};

void testWithVirtualDestructor() {
    std::cout << "\nTest 1: With virtual destructor:" << std::endl;
    Base* ptr = new Derived;
    delete ptr;
}

int main() {
    std::cout << "Testing virtual destructor behavior..." << std::endl;
    
    testWithVirtualDestructor();
    
    std::cout << "\nProgram completed successfully." << std::endl;
    return 0;
} 