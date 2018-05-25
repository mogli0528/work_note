#ifndef _CALC_HISTOGRAM_H
#define _CALC_HISTOGRAM_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>





class Histogram1D {
public:
    Histogram1D() 
    {
        channels_[0] = 0;
        histSize_[0] = 256;
        hranges_[0] = 0.0;
        hranges_[1] = 255.0;
        ranges_[0] = hranges_;
        histImage_ = cv::Mat(cv::Size(histSize_[0], histSize_[0]), CV_8U, cv::Scalar(255));
    }

    cv::MatND getHistogram(const cv::Mat &image);
    cv::Mat& getHistogramImage(const cv::Mat &image);
    cv::Mat applyLookUp(const cv::Mat &image, const cv::Mat &lookup);
    cv::Mat stretch(const cv::Mat &image, int minValue=0); 

private:
    int channels_[1];
    int histSize_[1];
    const float* ranges_[1];   // const 必不可少, float* 需要经过二次封装为 float**
    float hranges_[2];
    cv::Mat histImage_;
};

cv::MatND Histogram1D::getHistogram(const cv::Mat &image)
{
    if(image.channels() != 1){
        std::cout << "#channels should be 1" << std::endl;
    }

    cv::MatND hist;

    cv::calcHist(  &image, 
                    1,            // 计算直方图的图像张数
                    channels_,    // 图像的通道数量
                    cv::Mat(),        // 掩模
                    hist,       // 返回的直方图, 还有个 SparseMat& hist,
                    1,               // 直方图维度
                    histSize_,    // 直方图中项的个数
                    ranges_   // 像素值的范围
                );

    return hist;
}

cv::Mat& Histogram1D::getHistogramImage(const cv::Mat &image)
{
    cv::MatND hist = getHistogram(image);
    float binVal = 0.;
    int y = 0;

    // 获取最大值和最小值
    double minVal = 0.0, maxVal = 0.0;
    cv::minMaxLoc(hist, &minVal, &maxVal, 0, 0);

    //设置最高点为 nbins 的 90%
    int hpt = static_cast<int>(0.9 * histSize_[0]);

    for(int x = 0; x < histSize_[0]; x++){

        binVal = hist.at<float>(x);
        y = static_cast<int>(binVal*hpt/maxVal); 

        cv::line( histImage_, cv::Point(x, 0), 
                  cv::Point(x, y), 
                  cv::Scalar(0)
                );
    }
    
    // 上下翻转
    cv::flip(histImage_, histImage_, 0);

    return histImage_; 
}

cv::Mat Histogram1D::applyLookUp(const cv::Mat &image, const cv::Mat &lookup)
{
    cv::Mat result;

    cv::LUT(image, lookup, result);   // lookup table

    return result;
}

// Stretches the source image.
cv::Mat Histogram1D::stretch(const cv::Mat &image, int minValue) 
{
    // Compute histogram first
    cv::MatND hist= getHistogram(image);

    // find left extremity of the histogram
    int imin= 0;
    for( ; imin < histSize_[0]; imin++ ) {
        std::cout<<hist.at<float>(imin)<<std::endl;
        if (hist.at<float>(imin) > minValue)
            break;
    }
    
    // find right extremity of the histogram
    int imax= histSize_[0]-1;
    for( ; imax >= 0; imax-- ) {

        if (hist.at<float>(imax) > minValue)
            break;
    }

    // Create lookup table
    int dims[1]={256};
    cv::MatND lookup(1,dims,CV_8U);

    for (int i=0; i<256; i++) {
    
        if (i < imin) lookup.at<uchar>(i)= 0;
        else if (i > imax) lookup.at<uchar>(i)= 255;
        else lookup.at<uchar>(i)= static_cast<uchar>(255.0*(i-imin)/(imax-imin)+0.5);
    }

    // Apply lookup table
    cv::Mat result;
    result= applyLookUp(image,lookup);

    return result;
}



/**
 * 彩色图像的直方图   
*/
class ColorHistogram {
public:
    ColorHistogram()
    {
        channels_[0] = 0;
        channels_[1] = 1;
        channels_[2] = 2;

        histSize_[0] = histSize_[1] = histSize_[2] = 256;

        hranges_[0] = 0.0; 
        hranges_[1] = 255.0; 

        ranges_[0] = ranges_[1] = ranges_[2] = hranges_;
    }

    cv::MatND getHistogram(const cv::Mat &image);
    cv::Mat& getHistogramImage(const cv::Mat &image);

private:
    int channels_[3];
    int histSize_[3];
    const float *ranges_[3];
    float hranges_[2];
};

cv::MatND ColorHistogram::getHistogram(const cv::Mat &image)
{
    cv::MatND hist;

    if(image.channels() != 3){
        std::cout << "Error: #channels should be 3" << std::endl;
        return hist;
    }

    cv::calcHist( &image,  
                  1,
                  channels_,
                  cv::Mat(),
                  hist,
                  3,
                  histSize_,
                  ranges_
                );

    return hist;
}

#endif
