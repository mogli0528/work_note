#include "line_detector.hpp"

using namespace coal;

bool gClickTwice = false;
bool gClickOnce = false;
cv::Point2i gPoint1 = cv::Point(0, 0);
cv::Point2i gPoint2 = cv::Point(0, 0);

struct CmpByKey {
  bool operator()(const PAIR_DISTANCE& lhs, const PAIR_DISTANCE& rhs) {
    return lhs.first < rhs.first;
  }
};

// 输出处理结果到视频文件
void LineDetector::saveToFile(const cv::Mat &image, const std::string &fname)
{
    static cv::VideoWriter vw(fname, iFourcc_, fFps_, videoShape_, true);

    vw << image;
}

int TopLineDetector::doDetection(const int method, const cv::Rect &roi)
{
    cv::Mat enhanced, mRoi;
    cv::Mat edges;
    cv::Mat threshold_;
    cv::Mat element9(9,9, CV_8U, cv::Scalar(1));
    cv::Mat element7(7,7, CV_8U, cv::Scalar(1));
    cv::Mat element5(5,5, CV_8U, cv::Scalar(1));
    cv::Mat element3(3,3, CV_8U, cv::Scalar(1));
    std::vector<cv::Vec4i> lines;
    cv::Vec4i line;
    float k, distance;
    DISTANCE line_distance_map; 
    cv::Point pt1_detect, pt2_detect;
    
    while(mFrame_.data) {
        if(!vCap_.isOpened()) {
            LOG_INFO("Video open failed.");
            return -1;
        }

        // 预处理
        int ksize = 5;
        // setROI(roi);
        mRoi = mFrame_(roi);
        cv::GaussianBlur(mRoi, mRoi, cv::Size(ksize, ksize), 0, 0); 
        image_enhance(mRoi, enhanced, ENHANCE_LAPLACE, NULL);
        threshold_adaptive(enhanced, enhanced);
        cv::dilate(enhanced, enhanced, element3);
        cv::erode(enhanced, enhanced, element3);
        
        // 霍夫直线检测, 注意参数的选取   
        cv::Canny(enhanced, edges, 50, 220, 3);
        cv::Mat result(mRoi);

        // 霍夫直线变换函数 HoughLinesP() 的参数说明:   
        cv::HoughLinesP(edges, lines, 9, CV_PI/180, 20, 30, 20);
        // 轮廓的显示需要反转黑白值  
        cv::threshold(edges, edges, 128, 255, cv::THRESH_BINARY_INV);
        
        // 对直线按照离原点的距离进行排序  
        // std::vector<cv::Vec4i>::iterator it;
        for ( auto it = lines.begin(); it != lines.end(); it++ ) {
        
            // 计算直线的角度
            line = *it;
            iDeltaX = line[2] - line[0];
            iDeltaY = line[3] - line[1];
            k = iDeltaY / (float)iDeltaX;

            distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);

            // 计算直线的距离   
            if (k > 0.5 && k < 1000){
                line_distance_map[distance] = *it;
            }
        }
        std::vector<PAIR_DISTANCE> line_distance_vec( line_distance_map.begin(),
                                                      line_distance_map.end() ); 
        // auto line_distance_vec( line_distance_map.begin(), 
                                // line_distance_map.end() ); 
        std::sort( line_distance_vec.begin(), 
              line_distance_vec.end(), 
              CmpByKey() ); 

        // 根据距离, 斜率筛选合适的直线, 并绘制到图上  
        // std::vector<PAIR_DISTANCE>::iterator it;
        for( auto it =  line_distance_vec.begin(); 
            it != line_distance_vec.end(); 
            it++ 
        ){
            line = it->second;
            iDeltaX = line[2] - line[0];
            iDeltaY = line[3] - line[1];
            k = iDeltaY / (float)iDeltaX;
            dLineYDetect_ = (- line[0]) * k + line[1];
            
            std::cout << "dLineYDetect_ = " << dLineYDetect_ << std::endl;
            std::cout << "k = " << k << std::endl;
            std::cout << "Line: " << line[0] << ", " << line[1] << ", " 
                      << line[2] << ", " << line[3] << ", " << std::endl;

            // 组成向量 保证大致方向是一致的, 避免余弦的范围过大.      
            if (iDeltaX > 0){

                pLineVecDetect_.x = (float)iDeltaX;
                pLineVecDetect_.y = (float)iDeltaY;
            } else {

                pLineVecDetect_.x = (float)(- iDeltaX);
                pLineVecDetect_.y = (float)(- iDeltaY);
            }

            // 第一次使用时判断用户是否已经指定.
            if( bGotLine_ ) {
                  
                dTheta_ = cosin_theta(pLineVecDetect_, pLineVecStd_);

                std::cout << "LineVecStd = " << pLineVecStd_.x << ", " << pLineVecStd_.y << std::endl;
                std::cout << "Theta = " << dTheta_ << std::endl;
            } else {

                std::cout << "!!! User should draw a line first !!!" << std::endl;
                return -1;
            }
        
            // 距离应该满足的条件, 经验值.   
            if(it->first > 0.8 && it->first < 100  // 斜率范围
                && dTheta_ > 1- dEpsino_    // 直线角度偏离不要太大 
                && std::abs(dLineYStd_-dLineYDetect_) < iOffset_ // y 偏移不要太大 
             ) 
            {                    
                std::cout << "Got a good line" << std::endl;

                // got_line = true;  // 找到一条合适的直线   
                pt1_detect = cv::Point(line[0]-line[1]/k, 0);
                pt2_detect = cv::Point(mFrame_.cols/2 - 40, k*(mFrame_.cols/2  - 40 - line[0]) + line[1]);
                cv::line(mRoi, pt1_detect, pt2_detect, cv::Scalar(0,0,255), 2, CV_AA);

                dLineYStd_ = dLineYDetect_;   // 更新 y
                // 更新 pLineVecStd_
                pLineVecStd_.x = pLineVecDetect_.x;
                pLineVecStd_.y = pLineVecDetect_.y;
                
                break;
            } else {
                std::cout << "Keep.." << std::endl;
                cv::line(mRoi, pt1_detect, pt2_detect, cv::Scalar(0,0,255), 2, CV_AA);
            }
            
            

        } // end of iterate line_distance_vec.
        
        imshow("laplace 图像增强效果", enhanced);
        imshow("edges", edges); 
        imshow("result", mFrame_); 


        if(27 == cv::waitKey((int)(1000/fFps_)))
            break;
        
        if(bVideoFlag_){
            vCap_.read(mFrame_);

            if(bSaveToFile_)
                saveToFile(mFrame_, "coal.avi");
        }
        else   
            break;  // 如果是图片, 仅处理一次即可
    }
}

void LineDetector::threshold_adaptive(const cv::Mat &src, cv::Mat &dst)
{
    cv::Mat gray(src.size(), CV_8UC1);
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    cv::adaptiveThreshold(gray, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 81, 15);
}

void LineDetector::image_enhance(cv::Mat& image, cv::Mat &dst, int method, void * param)
{
    cv::Mat image_rbg[3];
    float scale = 3;
    cv::Mat kernel = (cv::Mat_<float>(3, 3) << 0, -1*scale, 0, -1*scale, 5*scale, 0, 0, -1*scale, -1*scale); 
    cv::Mat image_log(image.size(), CV_32FC3);
    cv::Mat image_gamma(image.size(), CV_32FC3); 
    switch(method){
        case ENHANCE_HIST:
            // 直方图均衡化   
            cv::split(image, image_rbg);
            for (int i = 0; i < 3; i++) {
                cv::equalizeHist(image_rbg[i], image_rbg[i]);
            }
            cv::merge(image_rbg, 3, dst);
            
            break;
        case ENHANCE_LAPLACE:
            // 拉普拉斯算子, 随着缩放因子的增大, 图像亮度越高
            // Mat kernel = (Mat_<float>(3, 3) << 0, -2, 0, 0, 10, 0, 0, -2, 0); 
            // Mat kernel = (Mat_<float>(3, 3) << 0, -3, 0, 0, 15, 0, 0, -3, 0); 
            cv::filter2D(image, dst, -1, kernel);
            break;
        case ENHANCE_LOG:
            // log 变换   
            for (int i = 0; i < image.rows; i++) {
                for (int j = 0; j < image.cols; j++) {
                    image_log.at<cv::Vec3f>(i, j)[0] = log(1 + image.at<cv::Vec3b>(i, j)[0]);
                    image_log.at<cv::Vec3f>(i, j)[1] = log(1 + image.at<cv::Vec3b>(i, j)[1]);
                    image_log.at<cv::Vec3f>(i, j)[2] = log(1 + image.at<cv::Vec3b>(i, j)[2]);
                }
            } 
            // 归一化到 0 ~ 255
            cv::normalize(image_log, image_log, 0, 255, CV_MINMAX);
            // 之前计算对数的时候转换成 float, 现在转换为 8 位图像显示
            cv::convertScaleAbs(image_log, dst);
            break;
        case ENHANCE_GAMMA:
            // gamma 变换(用于过曝光的场景), 如果 gamma > 1, 图像变暗.  如果gamma < 1, 图像变亮   
            for (int i = 0; i < image.rows; i++) {
                for (int j = 0; j < image.cols; j++) {
                    image_gamma.at<cv::Vec3f>(i, j)[0] = pow(float(image.at<cv::Vec3b>(i, j)[0]), 0.5);
                    image_gamma.at<cv::Vec3f>(i, j)[1] = pow(float(image.at<cv::Vec3b>(i, j)[1]), 0.5);
                    image_gamma.at<cv::Vec3f>(i, j)[2] = pow(float(image.at<cv::Vec3b>(i, j)[2]), 0.5);
                }
            }
            // 归一化到0~255    
            cv::normalize(image_gamma, image_gamma, 0, 255, CV_MINMAX);  
            // 转换成 8 bit图像显示    
            cv::convertScaleAbs(image_gamma, dst); 
            break;

        default:
            std::cout << "Not supported Method: " << method << std::endl; 
            break;
    }
}

double LineDetector::cosin_theta(float *vec1, float *vec2, int n)
{
    double dot = cblas_sdot(2, vec1, 1, vec2, 1);

    double norm = sqrt(std::pow(vec1[0], 2) + std::pow(vec1[1], 2)) *
                sqrt(std::pow(vec2[0], 2) + std::pow(vec2[1], 2));

    return dot / norm;
}

double  LineDetector::cosin_theta(cv::Point2i &pt1, cv::Point2i &pt2)
{
    float vec1[2], vec2[2];
    vec1[0] = pt1.x;
    vec1[1] = pt1.y;

    vec2[0] = pt2.x;
    vec2[1] = pt2.y;

    double dot = cblas_sdot(2, vec1, 1, vec2, 1);

    double norm = sqrt(std::pow(vec1[0], 2) + std::pow(vec1[1], 2)) *
                sqrt(std::pow(vec2[0], 2) + std::pow(vec2[1], 2));

    return dot / norm;
}

double LineDetector::cosin_theta(std::vector<float> &vec1, std::vector<float> &vec2)
{  
    std::cout << "Line: " << vec1[0] << ", " << vec1[1] << ", " 
            << vec2[0] << ", " << vec2[1] << ", " << std::endl;

    return cosin_theta(&vec1[0], &vec2[0], vec1.size());
}



void LineDetector::drawInitLine(const std::string &win_name, const cv::Rect &roi)
{
    cv::Mat canvas(mFrame_);
    
    while(1)
    {
        if(gClickTwice && gClickOnce) {
            pLineVecStd_.x = (float)(gPoint2.x - gPoint1.x);
            pLineVecStd_.y = (float)(gPoint2.y - gPoint1.y);
            dLineYStd_ = (- (gPoint1.x - roi.x)) * pLineVecStd_.y / pLineVecStd_.x +  gPoint1.y;
            if(pLineVecStd_.x < 0){
                pLineVecStd_.x = - pLineVecStd_.x;
                pLineVecStd_.y = - pLineVecStd_.y;
            }

            cv::line(canvas, gPoint1, gPoint2, cv::Scalar(0, 0, 255), 2);
        } 
        cv::imshow(win_name, canvas); 
        if (cv::waitKey(10) == 27)//按esc跳出  
            break;  
    }
    cv::destroyAllWindows();

    bGotLine_ = true;
}


void on_mouse(int event, int x, int y, int flags, void *param)  
{  
    cv::Mat& image = *(cv::Mat*) param;  

    switch (event)  
    {  
        case cv::EVENT_MOUSEMOVE: //鼠标移动  
        
        break;  

        case cv::EVENT_LBUTTONDOWN://点击鼠标左键  
            // 点击两次
            if (gClickOnce && !gClickTwice) {

                std::cout << " ClickTwice" << std::endl;
                std::cout << "x_ = " << x << ",  y_ = " << y << std::endl;
                gClickTwice = true;
                gPoint2.x = x;
                gPoint2.y = y;
            }

            // 点击一次
            if (!gClickTwice && !gClickOnce){
                
                std::cout << " ClickOnce" << std::endl;
                std::cout << "x_ = " << x << ",  y_ = " << y << std::endl;
                gClickOnce = true;
                gPoint1.x = x;
                gPoint1.y = y;
            }
            
        break;  

        case cv::EVENT_LBUTTONUP: //鼠标左键抬起  
        break;  
    }  
} 