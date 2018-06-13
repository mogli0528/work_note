

#include "integer_image.hpp"
#include "int_integral_image.hpp"
#include <boost/date_time/posix_time/posix_time.hpp>

/**
 * Usage Example:
 * 
 *      ./integer_image /home/klm/Desktop/a_sa.jpg 2 9 
*/

using namespace std;


int main__(int argc, char* argv[])
{
    Mat src = imread(argv[1]);
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    Mat dst = src.clone();

    IntegerImage ii;
    int radius = 1;
    ii.init(src.cols, src.rows, radius, radius);
    ii.integerImage((uchar*)src.data);

    int x = 20, y = 50;
    float a = ii.getMean(x, y);    // 求出以(x,y)为中心的矩形的均值
    float b = ii.getVar(x, y);

    cout << "mean:       "        << a << endl;
    cout << "var:        "        << b << endl;

    cv::imshow("src", src);
    cv::imshow("output", dst);

    if(27 == cv::waitKey(0))
        cv::destroyAllWindows();

    return 0;
}

int SIZE = 9; 
uchar CLIP255(int x)  {if(x < 0) return 0; else if(x > 255) return 255;}    

void lee_filter_1_chs(const cv::Mat &src, cv::Mat &dst, int sigmma);
void lee_filter_3_chs(const cv::Mat &src, cv::Mat &dst, int sigmma);
void lee_filter_1_chs_ii(const cv::Mat &src, cv::Mat &dst, int sigmma, IntIntegralImage &ii);
void lee_filter_3_chs_ii(const cv::Mat &src, cv::Mat &dst, int sigmma, IntIntegralImage &ii);

int main(int argc, char* argv[])
{
    boost::posix_time::ptime start_cpu;
    boost::posix_time::ptime stop_cpu;

    if (argc < 4){
        std::cout << "Usage: integra_image input.png patch_size DenoiseLevel "<< std::endl;
        return -1;
    }

    Mat src = imread(argv[1]);
    cv::resize(src, src, cv::Size(600, 480));
    cv::cvtColor(src, src, COLOR_BGR2GRAY);
    Mat dst = src.clone();
    uchar* src_data = src.data;
    uchar* dst_data = dst.data;
    
    int DenoiseLevel = atoi(argv[2]);
    int sigmma = 100 + DenoiseLevel * DenoiseLevel * 5;
    SIZE = atoi(argv[3]);

    // 获取当前时间
    IntIntegralImage ii(src);

    start_cpu = boost::posix_time::microsec_clock::local_time();
    ii.calculate_sum_and_squaresum();
    float elapsed = (boost::posix_time::microsec_clock::local_time() - 
                        start_cpu).total_milliseconds();
    // cout << "takes " << elapsed << "ms"<< endl;

    // start_cpu = boost::posix_time::microsec_clock::local_time();
    if ( 1 == src.channels() )
        lee_filter_1_chs_ii(src, dst, sigmma, ii);
    else if( 3 == src.channels() )
        lee_filter_3_chs_ii(src, dst, sigmma, ii);

    elapsed = (boost::posix_time::microsec_clock::local_time() - 
                        start_cpu).total_milliseconds();
    cout << "takes " << elapsed << "ms"<< endl;

    cv::imshow("src", src);
    cv::imshow("output", dst);

    if(27 == cv::waitKey(0))
        cv::destroyAllWindows();

    return 0;
}


void lee_filter_1_chs_ii(const cv::Mat &src, cv::Mat &dst, int sigmma, IntIntegralImage &ii)
{
    int height = src.rows;
    int width = src.cols;
    int lineByte = src.step;
    uchar* src_data = src.data;
    uchar* dst_data = dst.data;
    double Sum, Sum2;
    int index;
    double Mean, Var, k;
    int size = SIZE*SIZE;
    cout << "II,  Size = " << SIZE << ", sigmma = " << sigmma << endl;

    for ( int y = SIZE/2;y < height-SIZE/2; y++ ) {  
        for ( int x = SIZE/2;x < width-SIZE/2; x++ ) {  

            Sum = 0, Mean = 0, Sum2 = 0;
            index = y * lineByte + x;  

            Sum = ii.getBlockSum( x, y, SIZE, SIZE )[0]; 
            Sum2 = ii.getBlockSquareSum( x, y, SIZE, SIZE )[0]; 

            Mean = Sum / size;
            Var = (Sum2-(Sum*Sum)/size)/size;  
            k = Var/(Var+sigmma);  

            dst_data[index] = CLIP255((int)((1.0-k)*Mean + k*src_data[index]));  
        }  
    }  
}

void lee_filter_1_chs(const cv::Mat &src, cv::Mat &dst, int sigmma)
{
    int height = src.rows;
    int width = src.cols;
    int lineByte = src.step;
    uchar* src_data = src.data;
    uchar* dst_data = dst.data;
    double Sum, Sum2;
    int index;
    double Mean, Var, k;
    int size = SIZE*SIZE;
    cout << "Size = " << SIZE << ", sigmma = " << sigmma << endl;

    for ( int x = SIZE/2;x < height-SIZE/2; x++ ) {  
        for ( int y = SIZE/2;y < width-SIZE/2; y++ ) {  

            Sum = 0, Mean = 0, Sum2 = 0;
            index = x * lineByte + y;  
    
            for( int i = -SIZE/2; i<= SIZE/2; i++ ) {  
                for( int j = -SIZE/2; j<=SIZE/2; j++ ) {

                    uchar tmp = src_data[index + i*lineByte + j];  
                    Sum += tmp;  
                    Sum2 += tmp*tmp;  
                }  
            }  

            Mean = Sum/size;  
            Var = (Sum2-(Sum*Sum)/size)/size;  
            k = Var/(Var+sigmma);  

            dst_data[index] = CLIP255((int)((1.0-k)*Mean + k*src_data[index]));  
        }  
    }  
}

/**
 * 注意: OpenCV 默认的图像通道顺序是 BGR.  
*/
void lee_filter_3_chs_ii(const cv::Mat &src, cv::Mat &dst, int sigmma, IntIntegralImage &ii)
{
    int height = src.rows;
    int width = src.cols;
    int lineByte = src.step;
    uchar* src_data = src.data;
    uchar* dst_data = dst.data;
    float SumR2 = 0, SumG2 = 0, SumB2 = 0;
    float SumR = 0, SumG = 0, SumB = 0;
    float MeanR = 0, MeanG = 0, MeanB = 0;
    float VarR = 0,  VarG = 0, VarB = 0;
    float kr, kg, kb; 
    int index;
    int size = SIZE * SIZE;
    int* sum;
    float* ssum;

    for( int y = SIZE/2; y < height-SIZE/2; y++ ) {
        for( int x = SIZE/2; x < width-SIZE/2; x++ ) {

            index = y * lineByte + 3 * x;   // 3 channels

            sum = ii.getBlockSum( x, y, SIZE, SIZE );
            ssum = ii.getBlockSquareSum( x, y, SIZE, SIZE );
            // if(sum.size() == 3 && ssum.size() == 3) 
            {

                SumB = sum[0]; 
                SumR = sum[2]; 
                SumG = sum[1]; 

                SumB2 = ssum[0]; 
                SumR2 = ssum[2]; 
                SumG2 = ssum[1]; 
            }
            
            MeanR = SumR/size;  // BGR 的通道顺序在这里要反过来.  
            MeanG = SumG/size;
            MeanB = SumB/size;

            // 计算方差
            VarR = (SumR2-(SumR*SumR)/size)/size;
            VarG = (SumG2-(SumG*SumG)/size)/size;
            VarB = (SumB2-(SumB*SumB)/size)/size;

            // 计算系数
            kr = VarR /(VarR+sigmma);
            kg = VarG /(VarG+sigmma);
            kb = VarB /(VarB+sigmma);
            
            dst_data[index]   = CLIP255((int)((1-kb)*MeanB+kb*src_data[index]));
            dst_data[index+1] = CLIP255((int)((1-kg)*MeanG+kg*src_data[index+1]));
            dst_data[index+2] = CLIP255((int)((1-kr)*MeanR+kr*src_data[index+2]));
        }
    }
}

/**
 * lee_filter 过程
 *  注意和图像的 BGR 顺序还是 RGB 顺序无关.  
*/
void lee_filter_3_chs(const cv::Mat &src, cv::Mat &dst, int sigmma)
{
    int height = src.rows;
    int width = src.cols;
    int lineByte = src.step;
    uchar* src_data = src.data;
    uchar* dst_data = dst.data;
    float SumR2 = 0, SumG2 = 0, SumB2 = 0;
    float SumR = 0, SumG = 0, SumB = 0;
    float MeanR = 0, MeanG = 0, MeanB = 0;
    float VarR = 0,  VarG = 0, VarB = 0;
    float kr, kg, kb; 
    int index;
    int size = SIZE * SIZE;

    for( int i = SIZE/2; i < height-SIZE/2; i++ ) {
        for( int j = SIZE/2; j < width-SIZE/2; j++ ) {

            SumR2 = 0, SumG2 = 0, SumB2 = 0;
            SumR = 0, SumG = 0, SumB = 0; 
            MeanR = 0, MeanG = 0, MeanB = 0;
            VarR = 0, VarG = 0, VarB = 0;
            index = i * lineByte + 3 * j;   // 3 channels

            for( int m = -SIZE/2; m<= SIZE/2; m++ ) {
                for( int n = -SIZE/2; n<=SIZE/2; n++ ) {
                    SumB  += src_data[index+m*lineByte+n*3];
                    SumB2 += src_data[index+m*lineByte+n*3]*src_data[index+m*lineByte+n*3];

                    SumG  += src_data[index+1+m*lineByte+n*3];
                    SumG2 += src_data[index+1+m*lineByte+n*3]*src_data[index+1+m*lineByte+n*3];

                    SumR  += src_data[index+2+m*lineByte+n*3];
                    SumR2 += src_data[index+2+m*lineByte+n*3]*src_data[index+2+m*lineByte+n*3];
                }
            } 
            
            // 计算平均值
            MeanR = SumR/size;
            MeanG = SumG/size;
            MeanB = SumB/size;

            // 计算方差
            VarR = (SumR2-(SumR*SumR)/size)/size;
            VarG = (SumG2-(SumG*SumG)/size)/size;
            VarB = (SumB2-(SumB*SumB)/size)/size;

            // 计算系数
            kr = VarR /(VarR+sigmma);
            kg = VarG /(VarG+sigmma);
            kb = VarB /(VarB+sigmma);
            
            dst_data[index]   =  CLIP255((int)((1-kb)*MeanB+kb*src_data[index]));
            dst_data[index+1] =  CLIP255((int)((1-kg)*MeanG+kg*src_data[index+1]));
            dst_data[index+2] =  CLIP255((int)((1-kr)*MeanR+kr*src_data[index+2]));
        }
    }
}



