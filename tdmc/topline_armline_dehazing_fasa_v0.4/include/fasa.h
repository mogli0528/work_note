#ifndef _FAST_ACCURATE_SIZE_AWARE_HPP_
#define _FAST_ACCURATE_SIZE_AWARE_HPP_

#include <iostream>
#include "opencv2/opencv.hpp"
#include <time.h>
#include <fstream>

using namespace cv;
using namespace std;

/**
 * \brief: Calculate the LAB histogram   
 * 
 * \param: im, input image in RGB color space
 * 
 * \return: histogram, output LAB histogram
 *          histogramIndex, mark the LAB value in original image
 *          averageX, 
 *          averageY,
 *          averageX2,
 *          averageY2,
 *          LL,
 *          AA,
 *          BB,
*/
void calculateHistogram(Mat im,
                        Mat &averageX,
                        Mat &averageY,
                        Mat &averageX2,
                        Mat &averageY2,
                        vector<float> &LL,
                        vector<float> &AA,
                        vector<float> &BB,
                        Mat &histogram,
                        Mat &histogramIndex);

/**
 * \brief: Reduce the colors continue and calculate the weights coefficient   
 * 
 * \param: histogram, LAB histogram (calculated before)
 *         LL, AA, BB, L-A-B values (calculated before)
 * 
 * \return: numberOfColors, number of colors until finally reduced
 *          reverseMap, 
 *          map,
 *          colorDistance, color distance measured by ||Q_i, Q_j||  
 *          exponentialColorDistance, the weights coefficient
*/
int preComputeParameters( Mat &histogram,
                          vector<float> LL,
                          vector<float> AA,
                          vector<float> BB,
                          int numberOfPixels,
                          vector<int> &reverseMap,
                          Mat &map,
                          Mat &colorDistance,
                          Mat &exponentialColorDistance);


/**
 * \brief: Bilateral Filter, used for calculate (mx, my), (Vx, Vy)
 *         can be accelerated by `integral image`  
 * 
 * \param: colorDistance, color distance measured by ||Q_i, Q_j||  
 *         exponentialColorDistance, the weights coefficient
 *         reverseMap, 
 *         histogramPtr,
 *         reverseMap, 
 *         averageXPtr, averageYPtr, averageX2Ptr, averageY2Ptr, 
 * 
 * \return: numberOfColors, number of colors until finally reduced
 *          (mx, my), (Vx, Vy), spatial center and variances of the colors.
 *          contrast, global contrast, mixed with probability to get the salient map
*/
void bilateralFiltering(Mat &colorDistance,
                        Mat &exponentialColorDistance,
                        vector<int> &reverseMap,
                        int* histogramPtr,
                        float* averageXPtr, float* averageYPtr,
                        float* averageX2Ptr, float* averageY2Ptr,
                        Mat &mx, Mat &my, Mat &Vx, Mat &Vy,
                        Mat &contrast);


/**
 * \brief: Modeling Salient Probability, computations are same as that paper writes.
 * 
 * \param: (mx, my), (Vx, Vy), spatial center and variances of the colors.
 *         modelMean, the mean value of the model
 *         modelInverseCovariance, the inverse of the covatiance Matrix.
 * 
 * \return: shapeProbability, Salient Probability
*/
void calculateProbability(Mat &mx, Mat &my, Mat &Vx, Mat &Vy, 
                          Mat &modelMean, Mat &modelInverseCovariance,
                          int width, int height,
                          Mat &Xsize, Mat &Ysize,
                          Mat &Xcenter, Mat &Ycenter,
                          Mat &shapeProbability); 

void computeSaliencyMap(Mat shapeProbability,
                        Mat contrast,
                        Mat exponentialColorDistance,
                        Mat histogramIndex,
                        int* mapPtr,
                        Mat& SM,
                        Mat& saliency);

int do_fasa(cv::Mat &im, cv::Rect_<float> &bBox);

#endif