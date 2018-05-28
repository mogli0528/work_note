#include "line_contours.h"

std::vector<cv::Vec4i> LineDetector::findLines(const cv::Mat &image)
{
    vLines_.clear();
    HoughLinesP( image, 
                 vLines_,     // 输出是 cv::Vec4i, 包含线段的起点和终点坐标.  
                 dRho_,        // 用于搜索直线的累加器的距离分辨率, 单位是像素.
                 dTheta_,      // 用于搜索直线的累加器的角度分辨率, 单位是弧度.
                 iMinVote_,     // 用于搜索直线的累加器的投票阈值
                 dMinLength_,    // 检测直线的最小长度
                 dMaxGap_        // 允许组成连续线段的最大像素间隔
               );

    return vLines_;
}

void LineDetector::drawDetectedLines(cv::Mat image, cv::Scalar color)
{
    std::vector<cv::Vec4i>::const_iterator it = vLines_.begin();

    cv::Point pt1;
    cv::Point pt2;
    while(it != vLines_.end())
    {
        pt1 = cv::Point((*it)[0], (*it)[1]);
        pt2 = cv::Point((*it)[2], (*it)[3]);
        cv::line(image, pt1, pt2, color, 1);

        it++;
    }
}

// 霍夫变换累加器原理   
void accTest()
{
    cv::Mat acc(200, 180, CV_8U, cv::Scalar(0));

    int x = 50, y = 30;
    for ( int i = 0; i < 180; i++) {
        double theta = i*CV_PI/180.;
        double rho = x*cos(theta) + y*sin(theta);

        int j = static_cast<int>(rho + 100.5);
        acc.at<uchar>(j, i)++;
    }
    
    x = 30, y = 10;
    for ( int i = 0; i < 180; i++) {
        double theta = i*CV_PI/180.;
        double rho = x*cos(theta) + y*sin(theta);

        int j = static_cast<int>(rho + 100.5);
        acc.at<uchar>(j, i)++;
    }
    cv::imshow("acc", acc*200);
}

std::vector<std::vector<cv::Point> >Contours::getContours(const cv::Mat &image)
{
    cv::findContours( image, 
                      vpContours_, 
                      CV_RETR_EXTERNAL, 
                      CV_CHAIN_APPROX_NONE);

    return vpContours_;
}


void Contours::drawContours(cv::Mat &image)
{
    cv::drawContours( image,  
                      vpContours_,
                      -1,
                      cv::Scalar(0, 0, 255), 
                      2
                    );
}

std::vector<std::vector<cv::Point> > Contours::filterContoursByLength()
{
    std::vector<std::vector<cv::Point> >::iterator it = vpContours_.begin();

    while(it != vpContours_.end() ) {
        if (it->size() < iMinLength_ || it-> size() > iMaxLength_)
            it = vpContours_.erase(it);
        else    
            it++;
    }

    return vpContours_;
}