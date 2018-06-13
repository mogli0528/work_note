#ifndef _INTEGRAL_IMAGE__H_
#define _INTEGRAL_IMAGE__H_

#include <vector>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class IntIntegralImage {
 public:
    IntIntegralImage() { }
    IntIntegralImage(const cv::Mat &image) {  
        this->image_ = image; 
        calculate_sum_and_squaresum();
    }

    cv::Mat getImage() {
        return image_;
    }

    /**
     * 两个点将图像分割成 4 块  
    */
    int getBlockSum(int x1, int y1, int block_width, int block_height) {

        // 右下角点  
        int y2 = y1 + block_height;
        int x2 = x1 + block_width;

        int tl = sum_[y1*width_+x1];
        int tr = sum_[y2*width_+x1];
        int bl = sum_[y1*width_+x2];
        int br = sum_[y2*width_+x2];
        int s = (br - bl - tr + tl);
        
        return s;
    }

    float getBlockSquareSum(int x1, int y1, int block_width, int block_height) {
        
        // 右下角点  
        int y2 = y1 + block_height;
        int x2 = x1 + block_width;

        float tl = squaresum_[y1*width_+x1];
        float tr = squaresum_[y2*width_+x1];
        float bl = squaresum_[y1*width_+x2];
        float br = squaresum_[y2*width_+x2];
        float var = (br - bl - tr + tl);
        
        return var;
    }

 private: 
   
    void calculate_sum(int w, int h) {
        // 初始化积分图
        // this->width_ = w+1;
        // this->height_ = h+1;
        this->width_ = w;
        this->height_ = h;
        sum_.resize(width_*height_, 0);
        uchar* image_data = image_.data;

        // 计算积分图
        int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
        for( int row = 1; row < height_; row++ ) {
            for( int col = 1; col < width_; col++ ) {
                
                // 计算和查找表
                p1 = image_data[(row-1)*w+col-1]&0xff; // p(x, y)
                p2 = sum_[row*width_+col-1];  // p(x-1, y)
                p3 = sum_[(row-1)*width_+col]; // p(x, y-1);
                p4 = sum_[(row-1)*width_+col-1]; // p(x-1, y-1);
                
                sum_[row*width_+col]= p1+p2+p3-p4;
            }
        }
    }

    void calculate_sum_and_squaresum() {
        // this->width_ = w+1;
        // this->height_ = h+1;
        this->width_ = this->image_.cols;
        this->height_ = this->image_.rows;
        this->channels_ = this->image_.channels();

        sum_.resize( width_ * height_ * channels_, 0 );
        squaresum_.resize( width_ * height_ * channels_, 0 );
        uchar* image_data = image_.data;

        // rows
        int p1=0, p2=0, p3=0, p4;
        float sp2=0, sp3=0, sp4=0;
        for(int row=1; row<height_; row++ ) {
            for(int col=1; col<width_; col++) {
                
                // 计算和查找表
                p1=image_data[(row-1)*width_+col-1] & 0xff;// p(x, y)
                // std::cout << p1 << std::endl;
                p2=sum_[row*width_+col-1]; // p(x-1, y)
                p3=sum_[(row-1)*width_+col]; // p(x, y-1);
                p4=sum_[(row-1)*width_+col-1]; // p(x-1, y-1);
                sum_[row*width_+col]= p1+p2+p3-p4;

                // 计算平方查找表
                sp2=squaresum_[row*width_+col-1]; // p(x-1, y)
                sp3=squaresum_[(row-1)*width_+col]; // p(x, y-1);
                sp4=squaresum_[(row-1)*width_+col-1]; // p(x-1, y-1);
                squaresum_[row*width_+col]=p1*p1+sp2+sp3-sp4;
            }
        }
    } 
    
    // sum index tables
    std::vector<int> sum_;   // 3 channels or signal channel
    std::vector<float> squaresum_;
    cv::Mat image_;
    int width_;
    int height_;
    int channels_;
    
};

#endif