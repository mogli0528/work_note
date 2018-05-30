#ifndef _LINE_DETECTOR_H_
#define _LINE_DETECTOR_H_

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <cblas.h>
#include <glog/logging.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

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
                  bGotArmLine_(false),
                  bGotTopLine_(false),
                  dTopLineYStd_ (0.0), 
                  pTopLineVecStd_(cv::Point2i(0, 0)), 
                  dTopLineYDetect_ (0.0), 
                  pTopLineVecDetect_(cv::Point2i(0, 0)), 
                  dArmLineXStd_ (0.0), 
                  pArmLineVecStd_(cv::Point2i(0, 0)), 
                  dArmLineXDetect_ (0.0), 
                  pArmLineVecDetect_(cv::Point2i(0, 0)),
                  dEpsino_(0.002), 
                  dTheta_(0.0),
                  iOffset_(2), 
                  dFps_(23) // 帧率  
    {
        bVideoFlag_ = true;
        vCap_ = cap;
        vCap_.read(mFrame_);
        
        videoShape_ = cv::Size( vCap_.get(CV_CAP_PROP_FRAME_WIDTH), 
                            vCap_.get(CV_CAP_PROP_FRAME_HEIGHT) ); 
        iFourcc_ = CV_FOURCC('M', 'P', '4', '2');
    }

    LineDetector( const cv::Mat &src ) : 
                  bVideoFlag_(false),
                  bSaveToFile_(false),
                  bGotArmLine_(false),
                  bGotTopLine_(false),
                  dTopLineYStd_ (0.0), 
                  pTopLineVecStd_(cv::Point2i(0, 0)), 
                  dTopLineYDetect_ (0.0), 
                  pTopLineVecDetect_(cv::Point2i(0, 0)), 
                  dArmLineXStd_ (0.0), 
                  pArmLineVecStd_(cv::Point2i(0, 0)), 
                  dArmLineXDetect_ (0.0), 
                  pArmLineVecDetect_(cv::Point2i(0, 0)),
                  dEpsino_(0.002), 
                  dTheta_(0.0),
                  iOffset_(2),
                  dFps_(10) // 帧率  
    {
        mFrame_ = src;
    }
    void setEpsino(const double e) { dEpsino_ = e; }
    void setFps(const double fps) { dFps_ = fps; }
    void setOffset(const double off) { iOffset_ = off; }
    void setLineVecStd(const double x, const double y)
    {
        pTopLineVecStd_.x = x;
        pTopLineVecStd_.y = y;
    }
    void setToSaveVideo(const bool save){ bSaveToFile_ = save; }

    /***
     * \@brief  完成实际的直线检测操作
     * 
     * \@param method: 使用的图像增强方法
     * \@param topLineRoi: 指定 topLine 所在的 ROI 区域
     * \@param armLineRoi: 指定 armLine 所在的 ROI 区域
     * \@param videoName: 指定输出视频文件名
     * \@param method: 使用的图像增强方法
    */
    virtual int doDetection( const int method, 
                     const cv::Rect &topLineRoi, 
                     const cv::Rect &armLineRoi, 
                     const std::string &videoName = "~/output.avi") = 0;

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

protected:
    void setTopLineYStd(const double y){ dTopLineYStd_ = y; }
    void setArmLineXStd(const double y){ dArmLineXStd_ = y; }
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
    double dFps_; // 帧率  
    cv::Size videoShape_; 
    int iFourcc_;
    
    // 保存检测出来的直线
    // cv::Vec4i line_;
    // 用户绘制直线
    bool bGotTopLine_;
    bool bGotArmLine_;

    // topLineArea
    int iTopLineDeltaX_, iTopLineDeltaY_; 
    cv::Mat topLineAreaEnhanced_;
    cv::Mat topLineAreaEdges_;
    cv::Mat topLineAreaThreshold_;
    std::vector<cv::Vec4i> topLineAreaLines_;
    cv::Vec4i topLineAreaLine_;
    // 根据斜率取舍直线   
    cv::Point2i pTopLineVecStd_;  // 用户指定的直线的方向向量
    double dTopLineYStd_;         // 用户指定的直线与 y 轴的交点的 y 值
    double dTopLineSlopStd_;      // 用户指定的直线斜率
    cv::Point2i pTopLineVecDetect_; // 检测出的直线的方向向量
    double dTopLineYDetect_;
    double dTopLineSlopDetect_;      // 用户指定的直线斜率
    double dEpsino_;                        // 阈值
    double dTheta_;                         // 角度余弦
    int iOffset_;

    // armLineArea
    int iArmLineDeltaX_, iArmLineDeltaY_; 
    cv::Mat armLineAreaEnhanced_;
    cv::Mat armLineAreaEdges_;
    cv::Mat armLineAreaThreshold_;
    std::vector<cv::Vec4i> armLineAreaLines_;
    cv::Vec4i armLineAreaLine_;
    // 根据斜率取舍直线   
    cv::Point2i pArmLineVecStd_;  // 用户指定的直线的方向向量
    double dArmLineXStd_;         // 用户指定的直线与 y 轴的交点的 y 值
    double dArmLineSlopStd_;      // 用户指定的直线斜率
    cv::Point2i pArmLineVecDetect_; // 检测出的直线的方向向量
    double dArmLineXDetect_;
    double dArmLineSlopDetect_;      // 用户指定的直线斜率
    // double dEpsino_;                        // 阈值
    double dArmLineTheta_;                         // 角度余弦
    // int iOffset_;
};

class TopLineDetector : public LineDetector {
public:
    TopLineDetector(const cv::VideoCapture cap) : LineDetector(cap) {}
    TopLineDetector(const cv::Mat src) : LineDetector(src) {}
    int doDetection( const int method, 
                     const cv::Rect &topLineRoi, 
                     const cv::Rect &ArmLineRoi, 
                     const std::string &videoName = "~/output.avi");
    int detectTopLine(const cv::Rect &roi, cv::Point &pt1Detect, cv::Point &ptDetect);
    int detectArmLine(const cv::Rect &roi, cv::Point &pt1Detect, cv::Point &ptDetect);
        // 
    void drawInitTopLine(const std::string &win_name, const cv::Rect &topLineRoi, const cv::Rect &armLineRoi);



private:
};

} // end of namespace

void on_mouse(int event, int x, int y, int flags, void *param);  

#endif
