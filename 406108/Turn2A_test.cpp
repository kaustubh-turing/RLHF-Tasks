#include <iostream>
#include <memory>
using namespace std;

class DrawingObject {
public:
    virtual void draw() = 0; // Pure virtual function declaration
    virtual ~DrawingObject() = default; // Virtual destructor
};

class Circle : public DrawingObject {
private:
    double radius;

public:
    Circle(double radius) : radius(radius) {}

    void draw() override {
        cout << "Drawing a circle with radius: " << radius << endl;
    }
};

class Square : public DrawingObject {
private:
    double side;

public:
    Square(double side) : side(side) {}

    void draw() override {
        cout << "Drawing a square with side length: " << side << endl;
    }
};

int main() {
    unique_ptr<DrawingObject> obj1 = make_unique<Circle>(5.0);
    unique_ptr<DrawingObject> obj2 = make_unique<Square>(3.0);

    obj1->draw();
    obj2->draw();

    return 0;
} 