#ifndef _FIT_LINE_H_
#define _FIT_LINE_H_

#include <iostream>
#include <vector>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

class FitLine {
public:

    // 遍历像素得到所有点的位置
    std::vector<cv::Point> getAllPoints(const cv::Mat &oneLine);

    // 直线拟合   
    void fitToLine();

    void drawFitLine(cv::Mat &image, int lineLength = 100, cv::Scalar color = cv::Scalar(0, 0, 255));

private:
    std::vector<cv::Point> vpPoints_;
    cv::Vec4f vfLine_;    // 前两项为单位方向向量, 后两项为直线上一点的坐标
};




#endif