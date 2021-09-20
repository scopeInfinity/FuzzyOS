// Simple C++ program

#include <stdio.h>

class Shape {
    char name[64];
protected:
    Shape(char *name) {
        std::snprintf(this->name, sizeof(this->name), name);
        std::printf("Shape [%s] created\n", this->name);
    }
    ~Shape() {
        std::printf("Shape [%s] destroyed\n", this->name);
    }
public:
    // non-pure virtual function.
    virtual double getArea() {
        std::printf("I SHOULD NOT BE CALLED\n");
    }
    char *getName() {
        return this->name;
    }
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

void printShapeDetails(Shape *shape) {
    std::printf("Shape Details\n");
    std::printf(" - Name: %s\n", shape->getName());
    std::printf(" - Area: %d\n", (int)shape->getArea());
}

int main(int argc, char *argv[]) {
    Rectangle r1("R1", 10, 20);
    Square s1("S1", 50);

    printShapeDetails(&r1);
    printShapeDetails(&s1);
    return 0;
}