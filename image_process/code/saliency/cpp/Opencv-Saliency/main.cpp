/*************************************************

Copyright: Guangyu Zhong all rights reserved

Author: Guangyu Zhong

Date:2014-09-27

Description: codes for Manifold Ranking Saliency Detection
Reference http://ice.dlut.edu.cn/lu/Project/CVPR13[yangchuan]/cvprsaliency.htm

**************************************************/
#include<iostream>
#include<cv.h>
#include<highgui.h>
#include"PreGraph.h"
using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    
    Mat img = imread("/home/klm/work/gitwork/work_note/image_process/images/light_gamma.png");
    PreGraph SpMat;
    Mat superpixels = SpMat.GeneSp(img);
    Mat sal = SpMat.GeneSal(img);
    cout << sal;
    Mat salMap = SpMat.Sal2Img(superpixels, sal);
    Mat tmpsuperpixels;
    normalize(salMap, tmpsuperpixels, 255.0, 0.0, NORM_MINMAX);
    tmpsuperpixels.convertTo(tmpsuperpixels, CV_8UC3, 1.0);
    imshow("sal mask", tmpsuperpixels);
    waitKey();
    return 0;
}
