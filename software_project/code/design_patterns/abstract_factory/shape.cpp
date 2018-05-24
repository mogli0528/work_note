#include <iostream>

#define SIMPLE

using namespace std;

class Shape {
 public:
    Shape() { id_ = total_++; }; 

    virtual void draw() = 0;

 protected:
    int id_;
    static int total_;
};
int Shape::total_ = 0;

class Circle : public Shape {
 public:
    void draw()
    {
        cout << "draw circle: " << id_ << endl;
    }
};


class Square : public Shape {
 public:
    void draw()
    {
        cout << "draw square: " << id_ << endl;
    }
};

class Rectangle : public Shape {
 public:
    void draw()
    {
        cout << "draw rectangle: " << id_ << endl;
    }
};

class Ellipse : public Shape {
 public:
    void draw()
    {
        cout << "draw ellipse: " << id_ << endl;
    }
};

class Factory {
 public:
    virtual Shape *getCrvIst() = 0;
    virtual Shape *getStrtIst() = 0;
};

class SimpleShapeFactory : public Factory {
 public:
    Shape *getCrvIst()
    {
        return new Circle();
    }

    Shape *getStrtIst()
    {
        return new Square();
    }
};

class RobustShapeFactory : public Factory {
 public:
    Shape *getCrvIst()
    {
        return new Ellipse();
    }

    Shape *getStrtIst()
    {
        return new Rectangle();
    }
};


int main()
{
    Factory *factory;
#ifdef SIMPLE
    factory = new SimpleShapeFactory();
#elif ROBUST
    factory = new RobustShapeFactory();
#endif

    factory->getCrvIst()->draw();
    factory->getStrtIst()->draw();

    return 0;
}