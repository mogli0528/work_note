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

/***
 * \@breif 图像增广 
 * 
 * \@pram image: 输入的源图像
 * \@pram dst: 处理后输出的目标图像
 * \@pram method: 指定要使用的增广方法
 * \@pram TODO: param 用于对应每种算法的特定参数   
 * 
 * 目前支持 4 中图像增广方式: 
 *   1. 直方图均衡化  
 *   2. Laplace 变换
 *   3. log 变换
 *   4. Gamma 变换
*/
enum {
    ENHANCE_HIST = 0,
    ENHANCE_LAPLACE,
    ENHANCE_LOG,
    ENHANCE_GAMMA
};
void image_enhance(Mat &image, Mat &dst, int method, void * param);


/***
 * \@ breif 自适应阈值处理. 生成二值化后的图像, 内部已经制定好自适应阈值的一些参数.     
 *  
 * \@pram src: 输入的源图像
 * \@pram dst: 处理后输出的目标图像
 *  
*/
void threshold_adaptive(Mat &src, Mat &dst);


void do_detection(Mat &image, string pre_savename, VideoCapture *cap = NULL);


#endif