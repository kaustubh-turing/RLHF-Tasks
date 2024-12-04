#include <iostream>
using namespace std;

class Shape {
public:
    Shape() { 
        cout << "Shape constructor called." << endl; 
    }
    
    // Test Case 1: With virtual destructor
    virtual ~Shape() { 
        cout << "Shape destructor called." << endl; 
    }
    
    // Test Case 2: Without virtual destructor (comment out the above and uncomment this)
    /*
    ~Shape() { 
        cout << "Shape destructor called." << endl; 
    }
    */
};

class Circle : public Shape {
private:
    double* radius;

public:
    Circle(double r) {
        cout << "Circle constructor called." << endl;
        radius = new double(r); // Dynamically allocate memory
    }
    
    ~Circle() {
        cout << "Circle destructor called." << endl;
        delete radius; // Clean up allocated memory
    }
};

void testWithVirtualDestructor() {
    cout << "\nTest 1: With virtual destructor:" << endl;
    Shape* ptr = new Circle(5.0);
    delete ptr;
}

int main() {
    cout << "Testing Shape hierarchy with virtual destructor..." << endl;
    
    testWithVirtualDestructor();
    
    cout << "\nProgram completed successfully." << endl;
    return 0;
} 