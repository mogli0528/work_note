
#pragma once
#include "afxwin.h"
#include <cv.h>
#include "cxcore.h"  
#include "math.h" 
#include <highgui.h>
#include<vector>
#include <iostream>  
#include "opencv2/core/core.hpp"    
#include "opencv2/highgui/highgui.hpp"    
#include "opencv2/imgproc/imgproc.hpp"    


using namespace std;
using namespace cv;

class Ctry :
    public CCmdTarget
{
public:
    Ctry();
    virtual ~Ctry();
    DECLARE_MESSAGE_MAP()
    afx_msg void OnTryTyr1();
    afx_msg void OnTryPath();

public:
    Mat getimage(Mat &a);
    Mat guidedFilter2(cv::Mat I, cv::Mat p, int r, double eps);
    IplImage* getDarkChannel(IplImage* &src);
    IplImage* getMinIcy(IplImage* dark,int w);
    double getA(IplImage* dark, IplImage*hazeImage);
    IplImage* getTransmission(IplImage* Icy, double Ac);

    IplImage* getDehazedImage(IplImage* hazeImage, IplImage* guidedt,double Ac);

public:

};

