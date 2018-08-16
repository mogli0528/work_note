#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QPixmap pixmap(":/new/icon/images/splash.jpg");

    QSplashScreen splash(pixmap);
    splash.show();
    for(long i = 0; i < 100000000; i++){splash.show();}

    MainWindow w;
    w.show();
    splash.finish(&w);

    return app.exec();
}
