#ifndef _CALC_HISTOGRAM_H
#define _CALC_HISTOGRAM_H

#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


class LineDetector {
public:
    LineDetector() : dRho_(1.0), dTheta_(CV_PI/180), 
                     iMinVote_(20), dMaxGap_(20), dMinLength_(30) {}

    // 设置直线检测累加器的分辨率
    inline void setAccResolution(double rho, double theta)
    {
        dRho_ = rho;
        dTheta_ = theta;
    }

    // 设置接受直线的最小投票数
    void setMinVote(int minv) { iMinVote_ = minv; }

    // 设置 gap 和直线的最小长度.  
    inline void setLengthAndGap(double maxg, double minl)
    {
        dMinLength_ = minl;
        dMaxGap_ = maxg;
    }

    // 使用概率霍夫变换检测直线
    std::vector<cv::Vec4i> findLines(const cv::Mat &image);

    void drawDetectedLines(cv::Mat image, cv::Scalar color=cv::Scalar(0, 0, 255));

private:

    std::vector<cv::Vec4i> vLines_;        // 检测到的直线
    double dRho_;           // 用于搜索直线的累加器的距离分辨率, 单位是像素.
    double dTheta_;         // 用于搜索直线的累加器的角度分辨率, 单位是弧度.

    int iMinVote_;           // 最小投票数

    double dMaxGap_;         // 允许组成连续线段的最大像素间隔
    double dMinLength_;      // 检测直线的最小长度
};
void accTest();


class Contours {
public:
    Contours() : iMinLength_(100), iMaxLength_(1000) {}
    std::vector<std::vector<cv::Point> > getContours(const cv::Mat &image);
    void drawContours(cv::Mat &image);
    void setMinMaxLength(int min, int max) { iMinLength_ = min;  iMaxLength_ = max; }
    std::vector<std::vector<cv::Point> > filterContoursByLength();

private:
    std::vector<std::vector<cv::Point> > vpContours_;
    int iMinLength_;
    int iMaxLength_;
};


#endif
