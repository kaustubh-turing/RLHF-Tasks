#include <iostream>
using namespace std;

class Shape {
public:
    Shape() = default;
    virtual ~Shape() = default; // Virtual destructor

    virtual double calculateArea() = 0; // Pure virtual function
};

class Rectangle : public Shape {
private:
    double length;
    double width;

public:
    Rectangle(double length, double width) : length(length), width(width) {}
    ~Rectangle() override = default;

    double calculateArea() override {
        return length * width;
    }
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double radius) : radius(radius) {}
    ~Circle() override = default;

    double calculateArea() override {
        return 3.14 * radius * radius;
    }
};

int main() {
    Shape* shapes[] = { new Rectangle(10, 20), new Circle(5.5) };

    for (Shape* shape : shapes) {
        cout << "Area: " << shape->calculateArea() << endl;
        delete shape;
    }

    return 0;
} 