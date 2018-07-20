#ifndef _INTEGRAL_IMAGE__H_
#define _INTEGRAL_IMAGE__H_

#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <boost/shared_ptr.hpp>

using namespace std;

class IntIntegralImage {
 public:
    IntIntegralImage() { }
    IntIntegralImage(const cv::Mat &image) {  
        this->image_ = image; 
        this->width_ = image_.cols;
        this->height_ = image_.rows;
        this->channels_ = image_.channels();

        ssum_ = new int(3);
        blockSum_ = new int(3);
    }

    ~IntIntegralImage() {
        delete []ssum_;
        delete []blockSum_;
    }


    cv::Mat getImage() {
        return image_;
    }

    /**
     * 两个点将图像分割成 4 块  
    */
    int* getBlockSum(int x1, int y1, int block_width, int block_height);
    int* getBlockSquareSum(int x1, int y1, int block_width, int block_height);
    void calculate_sum();
    void calculate_sum_and_squaresum();

 private: 

    // sum index tables
    std::vector<std::vector<int> > sum_;   // 3 channels or signal channel
    std::vector<std::vector<int> > squaresum_;
    int* blockSum_;
    int* ssum_;
    cv::Mat image_;
    int width_;
    int height_;
    int channels_;
    
};

#endif