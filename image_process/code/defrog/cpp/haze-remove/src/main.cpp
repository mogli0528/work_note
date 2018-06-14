#define _CRT_SECURE_NO_WARNINGS
#include "main.h"
#include "guidedfilter.h"
// #pragma comment( lib, "opencv_world310d.lib" ) 
using namespace std;
using namespace cv;


int _PriorSize = 15;    //窗口大小
double _topbright = 0.001; //亮度最高的像素比例
double _w = 0.95;   //w
float t0 = 0.1; //T(x)的最小值   因为不能让tx小于0 等于0 效果不好
int SizeH = 0;      //图片高度
int SizeW = 0;      //图片宽度
int SizeH_W = 0;      //图片中的像素总 数 H*W
Vec<float, 3> a;     //全球大气的光照值
Mat trans_refine;
Mat dark_out1;

// 读入图片
// 并将其转换为3通道的矩阵后 
// 除以255 将其RBG确定在0-1之间 
Mat ReadImage(std::string& img_name)
{

    Mat img = imread(img_name);

    SizeH = img.rows;
    SizeW = img.cols;
    SizeH_W = img.rows*img.cols;

    Mat real_img(img.rows, img.cols, CV_32FC3);
    img.convertTo(real_img, CV_32FC3);

    real_img = real_img / 255;

    return real_img;
}


//计算暗通道
//J^{dark}(x)=min( min( J^c(y) ) )
Mat DarkChannelPrior(Mat& img)
{
    Mat dark = Mat::zeros(img.rows, img.cols, CV_32FC1);//新建一个所有元素为0的单通道的矩阵

    for (int i = 0; i<img.rows; i++)
    {
        for (int j = 0; j<img.cols; j++)
        {

            dark.at<float>(i, j) = min(
                min(img.at<Vec<float, 3>>(i, j)[0], img.at<Vec<float, 3>>(i, j)[1]),
                min(img.at<Vec<float, 3>>(i, j)[0], img.at<Vec<float, 3>>(i, j)[2])
                );//就是两个最小值的过程
        }
    }
    erode(dark, dark_out1, Mat::ones(_PriorSize, _PriorSize, CV_32FC1));//这个函数叫腐蚀 做的是窗口大小的模板运算 ,对应的是最小值滤波,即 黑色图像中的一块块的东西

    return dark_out1;//这里dark_out1用的是全局变量，因为在其它地方也要用到
}

// 这个函数在计算 tx 用到，因为与计算暗通道一样都用到了求最小值的过程，变化不多，所以改了
// 改就用这里了
Mat DarkChannelPrior_(Mat& img)
{
    double A = (a[0] + a[1] + a[2]) / 3.0; // 全球大气光照值 此处是 3 通道的平均值

    Mat dark = Mat::zeros(img.rows, img.cols, CV_32FC1);
    Mat dark_out = Mat::zeros(img.rows, img.cols, CV_32FC1);
    for (int i = 0; i<img.rows; i++)
    {
        for (int j = 0; j<img.cols; j++)
        {
            dark.at<float>(i, j) = min(
                min(img.at<Vec<float, 3>>(i, j)[0] / A, img.at<Vec<float, 3>>(i, j)[1] / A),
                min(img.at<Vec<float, 3>>(i, j)[0] / A, img.at<Vec<float, 3>>(i, j)[2] / A)
                );//同理


        }
    }
    erode(dark, dark_out, Mat::ones(_PriorSize, _PriorSize, CV_32FC1));//同上

    return dark_out;

}


// 解决天空太亮的问题  
// 计算 A 的值
// vec<float ,3> 表示有3个大小的vector 类型为float
Vec<float, 3> Airlight(Mat& img, Mat& dark)
{
    int n_bright = _topbright*SizeH_W;

    // 这里dark_1是一个有图片像素那么多行的矩阵 方便下面循环计算
    Mat dark_1 = dark.reshape(1, SizeH_W); 

    vector<int> max_idx;

    float max_num = 0;

    Vec<float, 3> A(0, 0, 0);
    Mat RGBPixcels = Mat::ones(n_bright, 1, CV_32FC3);

    for (int i = 0; i<n_bright; i++)
    {
        max_num = 0;
        max_idx.push_back(max_num);
        for (float * p = (float *)dark_1.datastart; p != (float *)dark_1.dataend; p++)
        {
            if (*p>max_num)
            {
                max_num = *p; // 记录光照的最大值

                max_idx[i] = (p - (float *)dark_1.datastart); // 位置

                RGBPixcels.at<Vec<float, 3>>(i, 0) = ((Vec<float, 3> *)img.data)[max_idx[i]];//对应的三个通道的值给RGBPixcels

            }
        }
        ((float *)dark_1.data)[max_idx[i]] = 0; // 访问过像素值的标记为0，这样就不会重复访问
    }

    // 将光照值累加
    for (int j = 0; j<n_bright; j++)
    {
        A[0] += RGBPixcels.at<Vec<float, 3>>(j, 0)[0];
        A[1] += RGBPixcels.at<Vec<float, 3>>(j, 0)[1];
        A[2] += RGBPixcels.at<Vec<float, 3>>(j, 0)[2];
    } 

    // 除以总数, 即取所有符合的点的平均值。
    A[0] /= n_bright;
    A[1] /= n_bright;
    A[2] /= n_bright; 

    return A;
}


// Calculate Transmission Matrix
Mat TransmissionMat(Mat& dark)
{
    double A = (a[0] + a[1] + a[2]) / 3.0;
    for (int i = 0; i < dark.rows; i++)
    {
        for (int j = 0; j < dark.cols; j++)
        {
            double temp = (dark_out1.at<float>(i, j));
            double B = fabs(A - temp);
            //	conut++;
            //cout << conut << endl;
            //if (B==)
            if (B - 0.3137254901960784 < 0.0000000000001)//K=80    80/255=0.31   这里浮点数要这样做减法才能正确的比较
            {
                dark.at<float>(i, j) = (1 - _w*dark.at<float>(i, j))*
                    (0.3137254901960784 / (B));//此处为改过的式子部分
            }
            else
            {
                dark.at<float>(i, j) = 1 - _w*dark.at<float>(i, j);
            }
            if (dark.at<float>(i, j) <= 0.2)//保证Tx不失真，因为会以上除出的结果会有不对
            {
                dark.at<float>(i, j) = 0.5;
            }
            if (dark.at<float>(i, j) >= 1)//同上
            {
                dark.at<float>(i, j) = 1.0;
            }

        }
    }

    return dark;
}

Mat TransmissionMat1(Mat& dark)
{
    double A = (a[0] + a[1] + a[2]) / 3.0;
    for (int i = 0; i < dark.rows; i++)
    {
        for (int j = 0; j < dark.cols; j++)
        {

            dark.at<float>(i, j) = (1 - _w*dark.at<float>(i, j));

        }
    }

    return dark;
}

/**
 * Calculate Haze Free Image 
 *     \@param exposure: 去雾后应该加亮的值。是用来增加亮度的， 0.1 比较好
*/
Mat hazefree(Mat& img, Mat& t, Vec<float, 3> a, float exposure = 0)
{
    double AAA = a[0];
    if (a[1] > AAA)
        AAA = a[1];
    if (a[2] > AAA)
        AAA = a[2];
    //取a中的最大的值


    //新开一个矩阵
    Mat freeimg = Mat::zeros(SizeH, SizeW, CV_32FC3);
    img.copyTo(freeimg);

    //两个迭代器，这样的写法可以不用两层循环，比较快点
    Vec<float, 3> * p = (Vec<float, 3> *)freeimg.datastart;
    float * q = (float *)t.datastart;

    for (; p<(Vec<float, 3> *)freeimg.dataend && q<(float *)t.dataend; p++, q++)
    {
        (*p)[0] = ((*p)[0] - AAA) / std::max(*q, t0) + AAA + exposure;
        (*p)[1] = ((*p)[1] - AAA) / std::max(*q, t0) + AAA + exposure;
        (*p)[2] = ((*p)[2] - AAA) / std::max(*q, t0) + AAA + exposure;
    }

    return freeimg;
}


void printMatInfo(const string name, Mat& m)
{
    cout << name << ":" << endl;
    cout << "\t" << "cols=" << m.cols << endl;
    cout << "\t" << "rows=" << m.rows << endl;
    cout << "\t" << "channels=" << m.channels() << endl;
}


int main(int argc, char * argv[])
{
    std::string img_name = 
        "/home/klm/work/gitwork/work_note/image_process/images/frog_city.jpg";

    if(argc > 1)
        img_name = std::string(argv[1]);
    else if( 1 == argc && !img_name.empty())
        std::cout << "Using Default image: " << img_name << endl;

    Mat img, dark_channel, trans, free_img;

    clock_t start, finish;
    double duration1, duration3, duration4, duration7;

    //读入图片
    cout << endl << "读入图片 ..." << endl;
    start = clock();
    img = ReadImage(img_name);
    finish = clock();
    duration1 = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time Cost: " << duration1 << "s" << endl << endl;//输出这一步的时间

    //计算暗通道
    cout << "计算暗通道 ..." << endl;
    start = clock();
    dark_channel = DarkChannelPrior(img);
    // printMatInfo("dark_channel", dark_channel);
    finish = clock();
    duration3 = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time Cost: " << duration3 << "s" << endl << endl;

    // 计算全球光照值 A
    cout << "计算A值 ..." << endl;
    start = clock();
    a = Airlight(img, dark_channel);
    finish = clock();
    duration4 = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Airlight:\t" << " B:" << a[0] << " G:" << a[1] << " R:" << a[2] << endl;
    cout << "Time Cost: " << duration4 << "s" << endl << endl;

    // 计算 tx
    cout << "Reading Refine Transmission..." << endl << endl;
    
    // 这里考虑全球大气光照值(3 通道的平均值)
    dark_channel = DarkChannelPrior_(img);
    trans_refine = TransmissionMat(dark_channel);

    // 导向滤波,得到精细的透射率图
    Mat tran = guidedFilter(img, trans_refine, 60, 0.0001);

    // 去雾
    cout << "Calculating Haze Free Image ..." << endl;
    // 此处 如果用 tran 的话就是导向滤波部分
    // 如果是trans_refine 就没有用导向滤波 效果不是那么的好
    start = clock();
    free_img = hazefree(img, tran, a, 0.1);
    finish = clock();
    duration7 = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time Cost: " << duration7 << "s" << endl;
    cout << "Total Time Cost: " << duration1 + duration3 + duration4 + duration7 << "s" << endl;

    //imwrite("output.jpg", free_img * 255);
    cv::imshow("原图", img);
    cv::imshow("Dark Channel Prior", dark_channel);
    cv::imshow("去雾后", free_img);
    //cv::imshow("Refined Transmission Mat",trans_refine);
    //cv::imshow("fitler", tran);

    cv::waitKey(0);
    cout << endl;

    return 0;
}