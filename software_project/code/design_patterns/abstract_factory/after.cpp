#include <iostream>
#define LINUX

using namespace std;

/**
 * Abstract base product. It should define an interface
 * which will be common to all products. Clients will
 * work with products through this interface, so it
 * should be sufficient to use all products.
 */

/**
 * 抽象基类 porduct. 应该定义一个所有 product 都会用到的接口.
*/
class Widget {
 public:
  virtual void draw() = 0;
};

/**
 * 创建 product 1.
 */
class LinuxButton : public Widget {
 public:
  void draw() { cout << "LinuxButton\n"; }
};
class LinuxMenu : public Widget {
 public:
  void draw() { cout << "LinuxMenu\n"; }
};

/**
 * 创建 product 2.
 */
class WindowsButton : public Widget {
 public:
  void draw() { cout << "WindowsButton\n"; }
};
class WindowsMenu : public Widget {
 public:
  void draw() { cout << "WindowsMenu\n"; }
};

/**
 * Abstract factory: 定义了创建所有相关 product 需要的方法.  
 */
class Factory {
 public:
  virtual Widget *create_button() = 0;
  virtual Widget *create_menu() = 0;
};

/**
 * 每一个具体的 Factory 对应上边的一个 product
 * 类. 每一个具体的 Factory 类创建自己的具体 product.  
 * 
 * 注意: 返回类型为抽象基类的类型(多态性).
 */
class LinuxFactory : public Factory {
 public:
  Widget *create_button() {
    return new LinuxButton;
  }
  Widget *create_menu() {
    return new LinuxMenu;
  }
};


/**
 * 每一个具体的 Factory 对应上边的一个 product
 * 类. 每一个具体的 Factory 类创建自己的具体 product.  
 * 
 * 注意: 返回类型为抽象基类的类型(多态性).
 */
class WindowsFactory : public Factory {
 public:
  Widget *create_button() {
    return new WindowsButton;
  }
  Widget *create_menu() {
    return new WindowsMenu;
  }
};

/**
 * Client receives a factory object from its creator.
 *
 * All clients work with factories through abstract
 * interface. They don't know concrete classes of
 * factories. Because of this, you can interchange
 * concrete factories without breaking clients.
 *
 * Clients 从来不直接创建平台相关的对象, 它会把创建平台相关的对象交给 factory,
 * 即抽象工厂的具体实现类.  
 * 
 * Clients don't know the concrete classes of created
 * products either, since abstract factory methods
 * returns abstract products.
 */
class Client {
 private:
  Factory *factory;

 public:
  Client(Factory *f) {
    factory = f;
  }

  void draw() {
    Widget *w = factory->create_button();
    w->draw();
    display_window_one();
    display_window_two();
  }

  void display_window_one() {
    Widget *w[] = {
        factory->create_button(),
        factory->create_menu()
    };
    w[0]->draw();
    w[1]->draw();
  }

  void display_window_two() {
    Widget *w[] = {
        factory->create_menu(),
        factory->create_button()
    };
    w[0]->draw();
    w[1]->draw();
  }
};

/**
 * Now the nasty switch statement is needed only once to
 * pick and create a proper factory. Usually that's
 * happening somewhere in program initialization code.
 */
int main() {
  Factory *factory;
#ifdef LINUX
  factory = new LinuxFactory;
#else // WINDOWS
  factory = new WindowsFactory;
#endif

  Client *c = new Client(factory);
  c->draw();
}