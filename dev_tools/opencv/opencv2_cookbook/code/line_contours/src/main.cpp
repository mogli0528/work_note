#include "line_contours.h"
#include "fit_line.h"

#include <cmath>


int main(int argc, char const *argv[])
{
    if(argc < 2){
        std::cout << "Usage: " << std::string(argv[0]).substr(2) 
                  << " \"/path/to/an/image\" " << std::endl;
        return -1;
    }

    std::vector<cv::Vec4i> lines;
    cv::Mat contours;
    cv::Mat image = cv::imread(argv[1]);
    cv::Mat image_copy = image.clone();
    cv::Mat image_contours = image.clone();
    cv::Mat gray_contours;

    // 霍夫直线检测
    cv::Canny(image, contours, 185, 300);

    LineDetector* ld = new LineDetector();
    // ld->setAccResolution(1.0, 180/CV_PI);
    ld->setMinVote(80);
    ld->setLengthAndGap(100, 20);

    lines = ld->findLines(contours); 
    ld->drawDetectedLines(image);

    // 直线拟合  
    FitLine* fl = new FitLine();
    int n = 0;
    cv::Vec4i line = lines[n];
    cv::Mat oneLine(contours.size(), CV_8U, cv::Scalar(0));
    cv::line( oneLine, 
              cv::Point(line[0], line[1]), 
              cv::Point(line[2], line[3]),
              cv::Scalar(255), 
              5
            );
    cv::bitwise_and(contours, oneLine, oneLine);
    fl->getAllPoints(oneLine);
    fl->fitToLine();
    fl->drawFitLine(image_copy);
    
    // 提取连通区域
    Contours *c = new Contours();
    cv::cvtColor(image_contours, gray_contours, cv::COLOR_BGR2GRAY);
    cv::threshold(gray_contours, gray_contours, 60, 255, cv::THRESH_BINARY_INV);
    c->getContours(gray_contours);
    c->filterContoursByLength();
    c->drawContours(image_contours);

    cv::imshow("fitLine", image_copy);
    cv::imshow("Contours", image_contours);
    cv::imshow("Edges", image);
    accTest();
    if(27 == cv::waitKey()){
        cv::destroyAllWindows();
        delete ld;
        delete fl;
        delete c;
    }


    return 0;
}
