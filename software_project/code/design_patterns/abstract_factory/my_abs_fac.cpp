#include <iostream>

#define LINUX
using namespace std;

class Widget {
 public:
    virtual void draw() = 0;
};

class LinuxButton : public Widget {
 public:
    void draw() { cout << "Linux Button" << endl; };
};
class LinuxMenu : public Widget {
 public:
    void draw() { cout << "Linux Menu" << endl; };
};

class WindowsButton : public Widget {
 public:
    void draw() { cout << "Windows Button" << endl; };
};
class WindowsMenu : public Widget {
 public:
    void draw() { cout << "Windows Menu" << endl; };
};


class Factory {
 public:
    virtual Widget * createButton() = 0; 
    virtual Widget * createMenu() = 0; 
};

class LinuxFactory : public Factory {
 public: 
    LinuxFactory()
    {
        cout << "Linux Factory" << endl;
    }
    Widget* createButton() 
    {
        return new LinuxButton();
    }
    Widget* createMenu() 
    {
        return new LinuxMenu();
    }
};


class WindowsFactory : public Factory {
 public:  
    WindowsFactory()
    {
        cout << "Windows Factory" << endl;
    }
    Widget* createButton() 
    {
        return new LinuxButton();
    }
    Widget* createMenu() 
    {
        return new LinuxMenu();
    }
};


class Client {
 public:
    Client(Factory *f) 
    {
        factory = f;
    }

    ~Client() 
    {
        delete factory;
        factory = 0;
    }

    void draw() 
    { 
        Widget *wBtn = factory->createButton();
        wBtn->draw();

        displayWdnOne();
        displayWdnTwo();
    }

    void displayWdnOne() 
    {
        Widget *w[] = {
            factory->createButton(),
            factory->createMenu() 
        };
        w[0]->draw();
        w[1]->draw();
    }

    void displayWdnTwo() 
    {
        Widget *w[] = {
            factory->createMenu(),
            factory->createButton() 
        };
        w[0]->draw();
        w[1]->draw();
    }

 private:
    Factory *factory;

};


int main()
{
    Factory * factory = 0;
#ifdef LINUX
    factory = new LinuxFactory();
#else  // WINDOWS
    factory = new WindowsFactory();
#endif

    Client *client = new Client(factory);
    client->draw();
}