// Simple C++ program
#include <stdio.h>
#include <iostream.h>
#include <vector.h>

namespace geometry {

class Shape {
    char name[64];
protected:
    Shape(char *name) {
        std::snprintf(this->name, sizeof(this->name), name);
        std::cout << "Shape [" << this->name << "] created" << std::endl;
    }
    ~Shape() {
        std::cout << "Shape [" << this->name << "] destroyed" << std::endl;
    }
public:
    // non-pure virtual function.
    virtual double getArea() {
        std::cout << "I SHOULD NOT BE CALLED" << std::endl;
    }
    char *getName() {
        return this->name;
    }
};

class Rectangle : public Shape {
    int length, width;
public:
    Rectangle(char name[], int length, int width) : Shape(name), length(length), width(width) {
        std::cout << "[" << name << "] is " << length << "x" << width <<" rectangle" << std::endl;
    }
    double getArea() {
        return this->length * this->width;
    }
};

class Square : public Rectangle {
public:
    Square(char name[], int side) : Rectangle(name, side, side) {
        std::cout << "[" << name << "] is side " << side <<" square" << std::endl;
    }
};

} // namespace geometry end

void printShapeDetails(geometry::Shape *shape) {
    std::cout << "Shape Details" << std::endl;
    std::cout << " - Name: " << shape->getName() << std::endl;
    std::cout << " - Area: " << (int)shape->getArea() << std::endl;
}
#include <string.h>
int main(int argc, char *argv[]) {
    // shapes

    geometry::Rectangle r1("R1", 10, 20);
    geometry::Square s1("S1", 50);

    printShapeDetails(&r1);
    printShapeDetails(&s1);

    // new, delete and std::string
    char *name = new char[1024];
    std::cout << "Enter your name: ";
    std::cin >> name;
    std::string cool_name;
    cool_name += '[';
    cool_name += name;
    cool_name += ']';
    std::cout << "Your name is: " << cool_name << std::endl;
    delete[] name;

    // vector
    std::vector<int> list(4, 100);
    list[0] = 0;
    for(std::size_t i = 0; i < list.size(); i++) {
        std::cout << "list[" << (int)i << "] = " << list[i] << std::endl;
    }

    std::cout << "Exiting..." << std::endl;
    return 0;
}