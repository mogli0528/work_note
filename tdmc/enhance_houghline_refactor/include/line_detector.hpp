#ifndef _LINE_DETECTOR_H_
#define _LINE_DETECTOR_H_

#define LOG_INFO(str) std::cout << str << std::endl

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cblas.h>


#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// bool gClickTwice
// bool gClickOnce;
// cv::Point2i gPoint1;
// cv::Point2i gPoint2;

// bool gClickTwice = false;
// bool gClickOnce = false;
// cv::Point2i gPoint1 = cv::Point(0, 0);
// cv::Point2i gPoint2 = cv::Point(0, 0);

namespace coal{

// 用于排序  
typedef std::pair<float, cv::Vec4i> PAIR_DISTANCE;
typedef std::map<float, cv::Vec4i> DISTANCE;

/** 
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

class LineDetector {
public:
    LineDetector( const cv::VideoCapture &cap ) : 
                  bVideoFlag_(true),
                  bSaveToFile_(false),
                  bGotLine_(false),
                  dLineYStd_ (0.0), 
                  pLineVecStd_(cv::Point2i(0, 0)), 
                  dLineYDetect_ (0.0), 
                  pLineVecDetect_(cv::Point2i(0, 0)), 
                  dEpsino_(0.002), 
                  dTheta_(0.0),
                  iOffset_(2)
    {
        bVideoFlag_ = true;
        vCap_ = cap;
        vCap_.read(mFrame_);
        fFps_ = 15; // 帧率  
        videoShape_ = cv::Size( vCap_.get(CV_CAP_PROP_FRAME_WIDTH), 
                            vCap_.get(CV_CAP_PROP_FRAME_HEIGHT) ); 
        iFourcc_ = CV_FOURCC('M', 'P', '4', '2');
    }

    LineDetector( const cv::Mat &src ) : 
                  bVideoFlag_(false),
                  bSaveToFile_(false),
                  bGotLine_(false),
                  dLineYStd_ (0.0), 
                  pLineVecStd_(cv::Point2i(0, 0)), 
                  dLineYDetect_ (0.0), 
                  pLineVecDetect_(cv::Point2i(0, 0)), 
                  dEpsino_(0.002), 
                  dTheta_(0.0),
                  iOffset_(2)
    {
        mFrame_ = src;
    }
    void setEpsino(const double e) { dEpsino_ = e; }
    void setOffset(const double off) { iOffset_ = off; }
    void setLineVecStd(const double x, const double y)
    {
        pLineVecStd_.x = x;
        pLineVecStd_.y = y;
    }
    void setLineYStd(const double y){ dLineYStd_ = y; }
    void setToSaveVideo(const bool save){ bSaveToFile_ = save; }

    /***
     * \@brief  完成实际的直线检测操作
     * 
     * \@param
    */
    virtual int doDetection(const int method, const cv::Rect &roi) = 0;

    /***
     * 计算两个向量(直线方向)的距离 - 夹角余弦
    */
    double cosin_theta(float *vec1, float *vec2, int n);
    double cosin_theta(cv::Point2i &pt1, cv::Point2i &pt2);
    double cosin_theta(std::vector<float> &vec1, std::vector<float> &vec2);

    /***
     * \@breif 图像增广 
     * 
     * \@pram image: 输入的源图像
     * \@pram dst: 处理后输出的目标图像
     * \@pram method: 指定要使用的增广方法
     * \@pram TODO: param 用于对应每种算法的特定参数   
    */
    void image_enhance(cv::Mat& image, cv::Mat &dst, int method, void * param);
    void threshold_adaptive(const cv::Mat &src, cv::Mat &dst);

    // 
    void drawInitLine(const std::string &win_name, const cv::Rect &roi);

protected:
    void saveToFile(const cv::Mat &image, const std::string &fname);
    void setROI(const cv::Rect &roi) { mRoi_ = mFrame_(roi); }

    // ROI
    cv::Mat mRoi_;

    cv::Mat mFrame_;
    cv::VideoCapture vCap_;
    // std::string sOutputVideoName_;   // 处理结果输出为 video
    
    // video
    bool bVideoFlag_;   // 是否处理 video 文件/ image
    bool bSaveToFile_;     // 是否保存处理后的 video 文件
    float fFps_; // 帧率  
    cv::Size videoShape_; 
    int iFourcc_;
    
    // 保存检测出来的直线
    cv::Vec4i line_;
    int iDeltaX, iDeltaY; 

    // 根据斜率取舍直线   
    cv::Point2i pLineVecStd_;               // 用户指定的直线的方向向量
    double dLineYStd_;                      // 用户指定的直线与 y 轴的交点的 y 值
    cv::Point2i pLineVecDetect_;            // 检测出的直线的方向向量
    double dLineYDetect_;
    double dEpsino_;                        // 阈值
    double dTheta_;                         // 角度余弦
    int iOffset_;

    // 用户绘制直线
    bool bGotLine_;

};

class TopLineDetector : public LineDetector {
public:
    TopLineDetector(const cv::VideoCapture cap) : LineDetector(cap) {}
    TopLineDetector(const cv::Mat src) : LineDetector(src) {}
    int doDetection(const int method, const cv::Rect &roi);

private:
};

} // end of namespace

void on_mouse(int event, int x, int y, int flags, void *param);  

#endif
