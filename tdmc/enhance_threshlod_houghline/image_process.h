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

#include <cblas.h>

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

/***
 * \@brief  完成实际的直线检测操作
 * 
 * \@param vec_std: 标准的直线方向: 起始值由用户指定. vec_std.size() == 2  
*/
void do_detection(Mat &image, vector<float> &vec_std, double y_std, string pre_savename, VideoCapture *cap = NULL);


double cosin_theta(float *vec1, float *vec2, int n);
double cosin_theta(vector<float> &vec1, vector<float> &vec2);

void on_mouse(int event, int x, int y, int flags, void *param);


#endif