#ifndef _IMAGE_PROCESS_H_
#define _IMAGE_PROCESS_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>  
#include <cmath>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;  
using namespace std;  


enum {
    ENHANCE_HIST,
    ENHANCE_LAPLACE,
    ENHANCE_LOG,
    ENHANCE_GAMMA
};


/***
 *  图像增强(4种): 直方图均衡化, Laplace 变换, Log 变换, Gamma 变换   
 *  
*/
void image_enhance(Mat &image, Mat &dst, int method, void * param);

/***
 * 自适应阈值处理 
 * 
*/
void threshold_adaptive(Mat &src, Mat &dst);
#endif