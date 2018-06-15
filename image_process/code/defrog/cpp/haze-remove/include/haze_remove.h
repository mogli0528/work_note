#include <iostream>
#include <string>
#include <algorithm>

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class HazeRemove{
 
 public:
    HazeRemove() { 
        A_.resize(3, 0.0);  
    }
    /** 
     *  读入图片, 并将其转换为浮点数矩阵后, 除以255将其RBG归一化到0-1之间 
     */ 
    HazeRemove( std::string& img_name ) {

        src_img_ = cv::imread(img_name);
        if( src_img_.empty() ) {

            std::cout << "error: Image Read Failed, No Such File(s): " 
                      << img_name << std::endl;
            real_img_ = src_img_;
        }

        height_ = src_img_.rows;
        width_ = src_img_.cols;
        size_ = src_img_.rows*src_img_.cols;

        src_img_.convertTo(real_img_, CV_32FC3);

        real_img_ /= 255;   // 归一化到[0-1] 之间.  
        dark_channel_ = cv::Mat::zeros(height_, width_, CV_32FC1); 
        dark_channel_out_ = cv::Mat::zeros(height_, width_, CV_32FC1); 
        trans_mat_refine_ = cv::Mat::zeros(height_, width_, CV_32FC1);  
        A_.resize(3, 0.0);
    }

    const cv::Mat& get_image() const{

        return src_img_;
    }

    std::vector<float>& refine_sky_color();
    cv::Mat& TransmissionMat();
    cv::Mat& TransmissionMat_simple();

    cv::Mat& DarkChannelPrior();
    cv::Mat& DarkChannelPrior_A();
    cv::Mat& haze_remove(Mat& t, float exposure = 0);

 private:
    int height_ = 0;      // 图片高度
    int width_ = 0;      // 图片宽度
    int size_ = 0;      // 图片中的像素总数 H*W
    int prior_box_size_ = 15;    // 窗口大小
    double top_brightness_ = 0.001; // 在暗通道中取亮度最高的前 0.1% 像素
    std::vector<float> A_;     // 全球大气的光照值
    // float A_[3];     // 全球大气的光照值
    double w_ = 0.95;         // w
    float t0_ = 0.1;          // T(x) 的最小值 
    
    cv::Mat src_img_;
    cv::Mat real_img_;
    cv::Mat dark_channel_;       // 计算出的暗通道值
    cv::Mat dark_channel_out_;   // 计算出的暗通道值, 考虑到 A 的均值
    cv::Mat trans_mat_refine_;
    cv::Mat reforce_img_;
};

void printMatInfo(const string name, Mat& m);

