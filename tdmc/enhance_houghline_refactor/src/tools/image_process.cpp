#include "image_process.h"

using namespace cv;  
using namespace std;  

// 用于排序  
typedef pair<float, Vec4i> PAIR_DISTANCE;
typedef map<float, Vec4i> DISTANCE;

struct CmpByKey {
  bool operator()(const PAIR_DISTANCE& lhs, const PAIR_DISTANCE& rhs) {
    return lhs.first < rhs.first;
  }
};

void threshold_adaptive(Mat &src, Mat &dst)
{
    Mat gray(src.size(), CV_8UC1);
    cvtColor(src, gray, COLOR_BGR2GRAY);
    adaptiveThreshold(gray, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 81, 15);

    return;
}

void image_enhance(Mat& image, Mat &dst, int method, void * param)
{
    Mat image_rbg[3];
    float scale = 3;
    Mat kernel = (Mat_<float>(3, 3) << 0, -1*scale, 0, -1*scale, 5*scale, 0, 0, -1*scale, -1*scale); 
    Mat image_log(image.size(), CV_32FC3);
    Mat image_gamma(image.size(), CV_32FC3); 
    switch(method){
        case ENHANCE_HIST:
            // 直方图均衡化   
            split(image, image_rbg);
            for (int i = 0; i < 3; i++) {
                equalizeHist(image_rbg[i], image_rbg[i]);
            }
            merge(image_rbg, 3, dst);
            
            break;
        case ENHANCE_LAPLACE:
            // 拉普拉斯算子, 随着缩放因子的增大, 图像亮度越高
            // Mat kernel = (Mat_<float>(3, 3) << 0, -2, 0, 0, 10, 0, 0, -2, 0); 
            // Mat kernel = (Mat_<float>(3, 3) << 0, -3, 0, 0, 15, 0, 0, -3, 0); 
            filter2D(image, dst, -1, kernel);
            break;
        case ENHANCE_LOG:
            // log 变换   
            for (int i = 0; i < image.rows; i++) {
                for (int j = 0; j < image.cols; j++) {
                    image_log.at<Vec3f>(i, j)[0] = log(1 + image.at<Vec3b>(i, j)[0]);
                    image_log.at<Vec3f>(i, j)[1] = log(1 + image.at<Vec3b>(i, j)[1]);
                    image_log.at<Vec3f>(i, j)[2] = log(1 + image.at<Vec3b>(i, j)[2]);
                }
            } 
            // 归一化到 0 ~ 255
            normalize(image_log, image_log, 0, 255, CV_MINMAX);
            // 之前计算对数的时候转换成 float, 现在转换为 8 位图像显示
            convertScaleAbs(image_log, dst);
            break;
        case ENHANCE_GAMMA:
            // gamma 变换(用于过曝光的场景), 如果 gamma > 1, 图像变暗.  如果gamma < 1, 图像变亮   
            for (int i = 0; i < image.rows; i++) {
                for (int j = 0; j < image.cols; j++) {
                    image_gamma.at<Vec3f>(i, j)[0] = pow(float(image.at<Vec3b>(i, j)[0]), 0.5);
                    image_gamma.at<Vec3f>(i, j)[1] = pow(float(image.at<Vec3b>(i, j)[1]), 0.5);
                    image_gamma.at<Vec3f>(i, j)[2] = pow(float(image.at<Vec3b>(i, j)[2]), 0.5);
                }
            }
            // 归一化到0~255    
            normalize(image_gamma, image_gamma, 0, 255, CV_MINMAX);  
            // 转换成 8 bit图像显示    
            convertScaleAbs(image_gamma, dst); 
            break;

        default:
            std::cout << "Not supported Method: " << method << std::endl; 
            break;
    }
}


// vector<float> vec_std(0.0, 2);      // 标准的直线方向: 起始值由用户指定.   
void do_detection(Mat &image, vector<float> &vec_std, double y_std, string pre_savename, VideoCapture *cap)
{
    bool got_line = false;
    string win_name = "Result";
    
    // 输出处理结果到视频文件
    float fpsInput=15; // 帧率  
    Size video_size = Size((*cap).get(CV_CAP_PROP_FRAME_WIDTH), (*cap).get(CV_CAP_PROP_FRAME_HEIGHT)); 
    int fourcc = CV_FOURCC('M', 'P', '4', '2');
    VideoWriter vw(pre_savename+"_out.avi", fourcc, fpsInput, video_size, true);
    
    // 保存检测出来的直线
    Vec4i line;
    int i = 0;
    int delta_x, delta_y; 

    // 根据斜率取舍直线   
    vector<float> vec_detect(2, 0.0);    // 检测出的直线方向
    double y_detect = 0.0;
    double epsino = 0.002;              // 阈值
    double theta = 0.0;               // 角度余弦
    int offset = 2;
    Point pt1_detect, pt2_detect;      // 保证直线显示不间断
    // vector<float> vec2_curr(0.0, 2);    // 标准的直线方向

    do {
        if(cap){
            (*cap).read(image);
        }

        got_line = false;  

        /***
         *  图像增强处理   
         *     高斯模糊 -> 图像增强 -> 自适应二值化 -> 膨胀处理
         * 
        */
        Mat enhanced, roi;
        roi = image(Rect(image.cols/2, 0, image.cols/2, image.rows/2));
        // 
        int ksize = 5;
        Mat element9(9,9, CV_8U, Scalar(1));
        Mat element7(7,7, CV_8U, Scalar(1));
        Mat element5(5,5, CV_8U, Scalar(1));
        Mat element3(3,3, CV_8U, Scalar(1));
        // morphologyEx(roi, roi, MORPH_CLOSE, element5);
        // cv::GaussianBlur(roi, roi, cv::Size(ksize, ksize), 0, 0); 
        cv::GaussianBlur(roi, roi, cv::Size(ksize, ksize), 0, 0); 
        // morphologyEx(enhanced, enhanced, MORPH_CLOSE, element5);
        // image_enhance(roi, enhanced, ENHANCE_LAPLACE, NULL);
        // image_enhance(roi, enhanced, ENHANCE_GAMMA, NULL);
        image_enhance(roi, enhanced, ENHANCE_LAPLACE, NULL);
        // image_enhance(enhanced, enhanced, ENHANCE_HIST, NULL);
   
        /// 自适应阈值(去掉, 因为canny 的轮廓提取边缘的效果更好)    
        Mat threshold_;
        threshold_adaptive(enhanced, enhanced);
        dilate(enhanced, enhanced, element3);
        erode(enhanced, enhanced, element3);

        // 霍夫直线检测   
        Mat edges;
        // Canny(threshold, edges, 60, 250, 3);
        Canny(enhanced, edges, 50, 220, 3);
        Mat result(roi);
        // cvtColor(enhanced, gray, COLOR_BGR2GRAY);

        /***
         * 霍夫直线变换函数 HoughLinesP() 的参数说明:   
         *  累加器的分辨率; 
         *  投票数;
         *  最小直线长度;
         *  最小的 gap
        */
        vector<Vec4i> lines;
        float k, distance;
        DISTANCE line_distance_map; 
        HoughLinesP(edges, lines, 9, CV_PI/180, 20, 30, 20);
        // 轮廓的显示需要反转黑白值  
        threshold(edges, edges, 128, 255, THRESH_BINARY_INV);
        
        // 对直线按照离原点的距离进行排序  
        for (vector<Vec4i>::iterator it = lines.begin(); it != lines.end(); it++) {
        
            // 计算直线的角度
            line = *it;
            delta_x = line[2] - line[0];
            delta_y = line[3] - line[1];
            k = delta_y / (float)delta_x;

            distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);

            // 计算直线的距离   
            if (k > 0.5 && k < 1000){
                line_distance_map[distance] = *it;
            }
        }
        vector<PAIR_DISTANCE> line_distance_vec(line_distance_map.begin(), line_distance_map.end()); 
        sort(line_distance_vec.begin(), line_distance_vec.end(), CmpByKey()); 

        // 根据距离, 斜率筛选合适的直线, 并绘制到图上    
        // 绘制 5 条  
        for(vector<PAIR_DISTANCE>::iterator it =  line_distance_vec.begin(); it != line_distance_vec.end(); it++){
            
            line = it->second;
            delta_x = line[2] - line[0];
            delta_y = line[3] - line[1];
            k = delta_y / (float)delta_x;
            y_detect = (- line[0]) * k + line[1];
            
            std::cout << "y_detect = " << y_detect << std::endl;
            std::cout << "k = " << k << std::endl;
            std::cout << "Line: " << line[0] << ", " << line[1] << ", " 
                      << line[2] << ", " << line[3] << ", " << std::endl;

            
            // 组成向量 保证大致方向是一致的, 避免余弦的范围过大.      
            if (delta_x > 0){

                vec_detect[0] = (float)delta_x;
                vec_detect[1] = (float)delta_y;
            } else {

                vec_detect[0] = (float)(- delta_x);
                vec_detect[1] = (float)(- delta_y);
            }
            if( 2 == vec_std.size() ) {
                  
                theta = cosin_theta(vec_detect, vec_std);

                cout << "std_vec = " << vec_std[0] << ", " << vec_std[1] << endl;
                cout << "theta = " << theta << endl;
            } else {

                cout << "!!! User should draw a line first !!!" << endl;
                return;
            }
        
            // 距离应该满足的条件, 经验值.   
            if(it->first > 0.8 && it->first < 100       // 斜率范围
                && theta > 1- epsino                    // 直线角度偏离不要太大 
                && std::abs(y_std-y_detect) < offset    // y 偏移不要太大 
             ) 
            {                    
                cout << "Got a line" << endl;

                // got_line = true;  // 找到一条合适的直线   
                pt1_detect = Point(line[0]-line[1]/k, 0);
                pt2_detect = Point(image.cols/2 - 40, k*(image.cols/2  - 40 - line[0]) + line[1]);
                // if(got_line) 
                {

                    // got_line = false;

                    cv::line(result, pt1_detect, pt2_detect, Scalar(0,0,255), 2, CV_AA);
                    y_std = y_detect;   // 更新 y
                    break;
                }
                vec_std.swap(vec_detect);  // 更新 vec_std
            } else {
                cv::line(result, pt1_detect, pt2_detect, Scalar(0,0,255), 2, CV_AA);
            }
            
            

        } // end of iterate line_distance_vec.
        
        imshow("laplace 图像增强效果", enhanced);
        imshow("edges", edges); 
        imshow("result", image); 
        vw << image;

        if(27 == waitKey((int)(1000/fpsInput)))
            break;
    }while(cap);
}

/***
 * 计算两个向量(直线方向)的距离 - 夹角余弦
 * 
*/
double cosin_theta(float *vec1, float *vec2, int n)
{
    double dot = cblas_sdot(2, vec1, 1, vec2, 1);

    double norm = sqrt(std::pow(vec1[0], 2) + std::pow(vec1[1], 2)) *
                  sqrt(std::pow(vec2[0], 2) + std::pow(vec2[1], 2));
   
    return dot / norm;
}

double cosin_theta(vector<float> &vec1, vector<float> &vec2)
{  
    std::cout << "Line: " << vec1[0] << ", " << vec1[1] << ", " 
              << vec2[0] << ", " << vec2[1] << ", " << std::endl;

    return cosin_theta(&vec1[0], &vec2[0], vec1.size());
}

