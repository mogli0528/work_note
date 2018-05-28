#include "fit_line.h"




std::vector<cv::Point> FitLine::getAllPoints(const cv::Mat &oneLine)
{
    for(int y = 0; y < oneLine.rows; y++) {

        const uchar* pRow = oneLine.ptr<uchar>(y);

        for( int x = 0; x < oneLine.cols; x++) {
            if(pRow[x])
                vpPoints_.push_back(cv::Point(x, y));
        }
    }

    return vpPoints_;
}

void FitLine::fitToLine()
{
    cv::fitLine( cv::Mat(vpPoints_), 
                 vfLine_, 
                 CV_DIST_L2, 
                 0, 
                 0.01, 0.01
                );
}

void FitLine::drawFitLine(cv::Mat &image, int lineLength, cv::Scalar color)
{
    cv::Point pt1 = cv::Point(vfLine_[2], vfLine_[3]);
    int x1 = pt1.x - lineLength*vfLine_[0];
    int y1 = pt1.y - lineLength*vfLine_[1];
    cv::Point pt2 = cv::Point(x1, y1);
    cv::line(image, pt1, pt2, color, 2);
}
