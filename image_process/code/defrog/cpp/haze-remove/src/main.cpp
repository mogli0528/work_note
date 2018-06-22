#define _CRT_SECURE_NO_WARNINGS
#include "haze_remove.h"
#include "guidedfilter.h"
#include <boost/date_time/posix_time/posix_time.hpp>

/**
 * 大致的时间消耗: 
 * 
 *   DarkChannelPrior() Time Cost:  3   ms
 *   refine_sky_color() Time Cost:  6   ms
 *   TransmissionMat() Time Cost:   12  ms
 *   guidedFilter() Time Cost:      80  ms
 *   haze_remove() Time Cost:       5   ms
 *   Total Time Cost:               106 ms
 * 
*/
// #pragma comment( lib, "opencv_world310d.lib" ) 
using namespace std;
using namespace cv;

void time_consume(void *func()){
    boost::posix_time::ptime start_cpu, stop_cpu;
    float elapsed, total_elapsed = 0.0;

    // 计算暗通道
    start_cpu = boost::posix_time::microsec_clock::local_time();

    (*func)();
    
    stop_cpu = boost::posix_time::microsec_clock::local_time();
    elapsed = (stop_cpu - start_cpu).total_milliseconds();
    cout << " Time Cost: " << elapsed << "ms" << endl;
}

int main(int argc, char * argv[])
{
    std::string img_name = 
        "/home/klm/work/gitwork/work_note/image_process/images/frog_city.jpg";

    if(argc > 1)
        img_name = std::string(argv[1]);
    else if( 1 == argc && !img_name.empty())
        std::cout << "Using Default image: " << img_name << endl;

    Mat dark_channel, trans, trans_refine, free_img;
    boost::posix_time::ptime start_cpu, stop_cpu;
    float elapsed, total_elapsed = 0.0;

    //读入图片
    HazeRemove hr(img_name);

    // 计算暗通道
    start_cpu = boost::posix_time::microsec_clock::local_time();

    dark_channel = hr.DarkChannelPrior();
    
    stop_cpu = boost::posix_time::microsec_clock::local_time();
    elapsed = (stop_cpu - start_cpu).total_milliseconds();
    cout << "DarkChannelPrior() Time Cost: " << elapsed << "ms" << endl;
    total_elapsed += elapsed;

    // 计算全球光照值 A
    start_cpu = boost::posix_time::microsec_clock::local_time();

    hr.refine_sky_color();

    stop_cpu = boost::posix_time::microsec_clock::local_time();
    elapsed = (stop_cpu - start_cpu).total_milliseconds();
    cout << "refine_sky_color() Time Cost: " << elapsed << "ms" << endl;
    total_elapsed += elapsed;

    // 这里考虑全球大气光照值(3 通道的平均值)
    // 计算 tx
    start_cpu = boost::posix_time::microsec_clock::local_time();

    dark_channel = hr.DarkChannelPrior_A();
    trans_refine = hr.TransmissionMat();
 
    stop_cpu = boost::posix_time::microsec_clock::local_time();
    elapsed = (stop_cpu - start_cpu).total_milliseconds();
    cout << "TransmissionMat() Time Cost: " << elapsed << "ms" << endl;
    total_elapsed += elapsed;

    // 导向滤波,得到精细的透射率图
    start_cpu = boost::posix_time::microsec_clock::local_time();

    trans = guidedFilter(hr.get_image(), trans_refine, 60, 0.0001);
    
    stop_cpu = boost::posix_time::microsec_clock::local_time();
    elapsed = (stop_cpu - start_cpu).total_milliseconds();
    cout << "guidedFilter() Time Cost: " << elapsed << "ms" << endl;
    total_elapsed += elapsed;

    // 去雾
    // 此处 如果用 tran 的话就是导向滤波部分
    // 如果是trans_refine 就没有用导向滤波 效果不是那么的好
    // 0.07 是为了提高处理后图像的亮度, 因为一般图像去雾后会变暗.   
    start_cpu = boost::posix_time::microsec_clock::local_time();

    free_img = hr.haze_remove(trans, 0.07);

    stop_cpu = boost::posix_time::microsec_clock::local_time();
    elapsed = (stop_cpu - start_cpu).total_milliseconds();
    cout << "haze_remove() Time Cost: " << elapsed << "ms" << endl;
    total_elapsed += elapsed;

    cout << "Total Time Cost: " << total_elapsed << "ms" << endl;

    //imwrite("output.jpg", free_img * 255);
    cv::imshow("原图", hr.get_origin_image());
    cv::imshow("Dark Channel Prior", dark_channel);
    cv::imshow("去雾后", free_img);
    //cv::imshow("Refined Transmission Mat",trans_refine);
    //cv::imshow("fitler", tran);

    cv::waitKey(0);
    cout << endl;

    return 0;
}