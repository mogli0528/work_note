#include <string>
#include <boost/python.hpp>

using namespace std;
using namespace boost::python;

char const * greet()
{
    return "hello world";
}

// struct World
// {
//     void set(string msg) { this->msg = msg; }

//     string greet() { return "hello " + msg; }
//     string msg;
// };

struct World
{
    World(){} // 构造函数
    World(string msg):msg(msg){} //增加构造函数
    World(double a, double b):a(a),b(b) {} //另外一个构造函数
    void set(string msg) { this->msg = msg; }
    string greet() { return "hello " + msg; }
    double sum_s() { return a + b; }
    string msg;
    double a;
    double b;
};

BOOST_PYTHON_MODULE(hello_ext) // 导出的 module 名字
{
    def("greet", greet);

    // default constructor
    // class_<World>("World")
    //     .def("greet", &World::greet)
    //     .def("set", &World::set);
   
    class_<World>("World")
        .def(init<string>()) 
        .def(init<double,double>()) // expose another construct
        .def("greet", &World::greet)
        .def("set", &World::set)
        .def("sum_s", &World::sum_s);
}