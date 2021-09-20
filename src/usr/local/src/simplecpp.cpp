// Simple C++ program

#include <stdio.h>

class Shape {
    char name[64];
protected:
    // virtual functions aren't supported yet.
    double getArea() {
        return 0;
    }

    Shape(char *name) {
        std::snprintf(this->name, sizeof(this->name), name);
        std::printf("Shape [%s] created\n", this->name);
    }
    // destructors aren't supported yet.
    // ~Shape() {
    //     std::printf("Shape [%s] destroyed\n", this->name);
    // }
};

class Rectangle : public Shape {
    int length, width;
public:
    Rectangle(char name[], int length, int width) : Shape(name), length(length), width(width) {
        std::printf("[%s] is %dx%d rectangle\n", name, length, width);
    }
    double getArea() {
        return this->length * this->width;
    }
};

class Square : public Rectangle {
public:
    Square(char name[], int side) : Rectangle(name, side, side) {
        std::printf("[%s] is %d side square\n", name, side);
    }
};

int main(int argc, char *argv[]) {
    Rectangle r1("R1", 10, 20);
    Square s1("S1", 50);
    // Shape justShape("ImInvalid");

    return 0;
}