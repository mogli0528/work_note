#ifndef _INTEGER_IMAGE_BOX_FILTER_H_
#define _INTEGER_IMAGE_BOX_FILTER_H_

#include <cstdio>
#include <cstring>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;


// typedef unsigned char uchar;

/**
 * 单通道的积分图计算
*/
class IntegerImage
{
public:
    IntegerImage(void);
    ~IntegerImage(void);

    void init(int width, int height, int mwidth=5, int mheight=5);
    void integerImage(unsigned char* img);

    float getMean(int x, int y);    // 以x,y为中心点，mwidth,mheight为直径的局部区域,下同
    float getVar(int x, int y);
    int getSum(int x, int y);
    int getSquareSum(int x, int y);
    int getLocalSize();

private:
    int mwidth ;    // 滑窗的 width, 也就局部统计区
    int mheight ;
    uchar* img;
    int width;
    int height;
    int* f_sum;   // I(x)
    int* f_sum2;  // I(x^2)
};

#endif