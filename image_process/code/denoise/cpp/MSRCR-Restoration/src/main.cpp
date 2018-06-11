#include "cv.h"
#include "highgui.h"

#include "MSRCR.h"

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    vector<double> sigema;
    vector<double> weight;
    for(int i = 0; i < 3; i++)
        weight.push_back(1./3);
    // 由于MSRCR.h中定义了宏USE_EXTRA_SIGMA，所以此处的vector<double> sigema并没有什么意义
    sigema.push_back(30);
    sigema.push_back(150);
    sigema.push_back(300);

//    string path = "/home/grq/style.mp4";
//    VideoCapture video(path);
//    Mat frame, imgdst;
//    Msrcr msrcr;
//    video >> frame;
//    while(!frame.empty())
//    {
//        video >> frame;
//        imshow("SRCR", frame);
//        msrcr.MultiScaleRetinex(frame, imgdst, weight, sigema, 128, 128);
//        imshow("dst", imgdst);
//        waitKey(20);
//    }

    char key;
    Mat img, imgdst;
    Msrcr msrcr;

    if (argc < 2){
        return -1;
    }

    img = imread(argv[1]);
    imshow("Frame", img);
    // 多次运行增强算法的效果可能会比单次的好   
    msrcr.MultiScaleRetinexCR(img, imgdst, weight, sigema, 128, 128);
    msrcr.MultiScaleRetinexCR(imgdst, imgdst, weight, sigema, 128, 128);
    // msrcr.MultiScaleRetinexCR(imgdst, imgdst, weight, sigema, 128, 128);
    imshow("dst", imgdst);

    key = (char) cvWaitKey(0);

    return 0;
}
