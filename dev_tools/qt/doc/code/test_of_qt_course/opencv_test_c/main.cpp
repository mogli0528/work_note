#include <QCoreApplication>

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Mat image = imread("beach.jpg");
//    if(image.data)
    {
     cvNamedWindow("window", CV_WINDOW_NORMAL);
     imshow("window",image);
     waitKey(0);

    }
//     else {
//     cout << "open failed" << endl;
//    }
   return a.exec();
}
