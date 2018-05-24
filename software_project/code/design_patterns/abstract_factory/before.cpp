#include <iostream>
#define LINUX

using namespace std;

/**
 * Abstract base product - 抽象基类.   
 */
class Widget {
 public:
  virtual void draw() = 0;
};

/**
 * Concrete product family 1 - 具体类1.
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
 * Concrete product family 2 - 具体类2.
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
 * Here's a client, which uses concrete products directly.
 * It's code filled up with nasty switch statements
 * which check the product type before its use.
 */
/**
 *  这是一个客户端, 直接使用上面的具体类.  
 *   在使用之前使用分支语句检查对应的类型.
 * 
*/
class Client {
 public:
  void draw() {
#ifdef LINUX
    Widget *w = new LinuxButton;
#else // WINDOWS
    Widget *w = new WindowsButton;
#endif
    w->draw();
    display_window_one();
    display_window_two();
  }

  void display_window_one() {
#ifdef LINUX
    Widget *w[] = {
        new LinuxButton,
        new LinuxMenu
    };
#else // WINDOWS
    Widget *w[] = {
        new WindowsButton,
        new WindowsMenu
    };
#endif
    w[0]->draw();
    w[1]->draw();
  }

  void display_window_two() {
#ifdef LINUX
    Widget *w[] = {
        new LinuxMenu,
        new LinuxButton
    };
#else // WINDOWS
    Widget *w[] = {
        new WindowsMenu,
        new WindowsButton
    };
#endif
    w[0]->draw();
    w[1]->draw();
  }
};

int main() {
  Client *c = new Client();
  c->draw();
}