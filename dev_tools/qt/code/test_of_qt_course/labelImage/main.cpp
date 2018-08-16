#include "mainwindow.h"
#include <QApplication>

//int main(int argc, char *argv[])
//{
//     Mat image = imread("/home/klm/work/gitwork/work_note/dev_tools/qt/doc/code/test_of_qt_course/opencv_test/test.jpg");
//     if(!image.empty())
//     {
//         cvNamedWindow("window", CV_WINDOW_NORMAL);
//         imshow("window",image);
//         waitKey(0);

//     }
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
