#ifndef _HISTOGRAM_H
#define _HISTOGRAM_H

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

/**
 * 工厂模式
*/
class Histogram {
public:
    virtual cv::MatND getHistogram(const cv::Mat &image) = 0;
     
};


class GrayHistogram : public Histogram {
public:
    cv::MatND getHistogram(const cv::Mat &image)
    {

    }

};


class RGBHistogram : public Histogram {
public:
    cv::MatND getHistogram(const cv::Mat &image)
    {
        
    }

};

class HSVHistogram : public Histogram {
public:
    cv::MatND getHistogram(const cv::Mat &image)
    {
        
    }

};

class Factory {
public:
    virtual Histogram *getInstance() = 0;
};

class GrayHistFactory : public Factory {
public:
    Histogram *getInstance() 
    {
        return new GrayHistogram();
    }
};

class RGBHistFactory : public Factory {
public:
    Histogram *getInstance() 
    {
        return new RGBHistogram();
    }
};

class HSVHistFactory : public Factory {
public:
    Histogram *getInstance() 
    {
        return new HSVHistogram();
    }
};