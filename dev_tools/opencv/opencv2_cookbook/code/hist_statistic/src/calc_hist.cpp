/*------------------------------------------------------------------------------------------*\
   This file contains material supporting chapter 5 of the cookbook:  
   Computer Vision Programming using the OpenCV Library. 
   by Robert Laganiere, Packt Publishing, 2011.

   This program is free software; permission is hereby granted to use, copy, modify, 
   and distribute this source code, or portions thereof, for any purpose, without fee, 
   subject to the restriction that the copyright notice may not be removed 
   or altered from any source or altered source distribution. 
   The software is released on an as-is basis and without any warranties of any kind. 
   In particular, the software is not guaranteed to be fault-tolerant or free from failure. 
   The author disclaims all warranties with regard to this software, any use, 
   and any consequent failure, is purely the responsibility of the user.
 
   Copyright (C) 2010-2011 Robert Laganiere, www.laganiere.name
\*------------------------------------------------------------------------------------------*/

#include "calc_hist.h"

int main(int argc, char **argv)
{
    if(argc < 2){
        std::cout << "Usage: " << std::string(argv[0]).substr(2) 
                  << " \"/path/to/an/image\" " << std::endl;
        return -1;
    }
	cv::Mat image = cv::imread(argv[1]);
    ColorHistogram ch;
    cv::MatND hist = ch.getHistogram(image);
    cv::cvtColor(image, image, CV_BGR2GRAY);

    // 获取灰度图直方图
    Histogram1D h;
    hist = h.getHistogram(image);
    
    // 根据直方图的边界像素值来对灰度图进行阈值化.   
    cv::Mat threshold;
    cv::threshold(image, threshold, 60, 255, cv::THRESH_BINARY);

    // lookup 反转图像像素强度值
    int dim[1] = {256};
    cv::Mat lut(1, dim, CV_8U);
    for(int i = 0; i < 256; i++){
        lut.at<uchar>(i) = 255 - i;
    }
    cv::Mat lutRet = h.applyLookUp(image, lut);
    cv::Mat stchRet = h.stretch(image, 100);

    cv::imshow("lutRet", lutRet);
    cv::imshow("stchRet", stchRet);
    cv::imshow("hist", h.getHistogramImage(image));
    cv::imshow("src", image);
    cv::imshow("demo", threshold);
	cv::waitKey();

	return 0;
}