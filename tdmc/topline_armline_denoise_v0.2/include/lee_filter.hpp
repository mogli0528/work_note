#ifndef _LEE_FILTER_H_
#define _LEE_FILTER_H_

#include "integer_image.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

const int DENOISE_LEVEL = 5;
const int SIZE = 9;   // patch size

uchar CLIP255(int x);

void lee_filter_1_chs(const cv::Mat &src, cv::Mat &dst, int sigmma);
void lee_filter_3_chs(const cv::Mat &src, cv::Mat &dst, int sigmma);
void lee_filter_1_chs_ii(const cv::Mat &src, cv::Mat &dst, int sigmma, IntIntegralImage &ii);
void lee_filter_3_chs_ii(const cv::Mat &src, cv::Mat &dst, int sigmma, IntIntegralImage &ii);

#endif