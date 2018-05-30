#include "line_detector.hpp"

using namespace coal;

bool gClickTwice = false;
bool gClickOnce = false;
cv::Point2i gPoint1 = cv::Point(0, 0);
cv::Point2i gPoint2 = cv::Point(0, 0);

struct CmpByKey {
  bool operator()(const PAIR_DISTANCE& lhs, const PAIR_DISTANCE& rhs) {
    return lhs.first > rhs.first;
  }
};

struct CmpByKeyReverse {
  bool operator()(const PAIR_DISTANCE& lhs, const PAIR_DISTANCE& rhs) {
    return lhs.first < rhs.first;
  }
};

// 输出处理结果到视频文件
void LineDetector::saveToFile(const cv::Mat &image, const std::string &fname)
{
    static cv::VideoWriter vw(fname, iFourcc_, dFps_, videoShape_, true);

    vw << image;
}

int TopLineDetector::doDetection ( const int method, 
                                   const cv::Rect &topLineRoi, 
                                   const cv::Rect &armLineRoi, 
                                   const std::string &videoName
)
{
    if(!vCap_.isOpened()) {
        LOG(ERROR) << "Video open failed.";
        return -1;
    }

    cv::Point pt1TopLineDetect, pt2TopLineDetect;
    cv::Point pt1ArmLineDetect, pt2ArmLineDetect;
    cv::Mat topLineArea;
    cv::Mat armLineArea;
    int height, width;
    cv::Mat color;

    while(mFrame_.data) {

        detectTopLine(topLineRoi, pt1TopLineDetect, pt2TopLineDetect);
        detectArmLine(armLineRoi, pt1ArmLineDetect, pt2ArmLineDetect);
        
        height = topLineAreaEnhanced_.rows + 20 + topLineAreaEdges_.rows;
        width = topLineAreaEnhanced_.cols + 20 + topLineAreaThreshold_.cols;
        topLineArea.create(height, width, CV_8UC3);
        topLineArea.setTo(cv::Scalar(0,0,0));
        topLineAreaEnhanced_.copyTo(topLineArea(cv::Rect(0, 0, topLineAreaEnhanced_.cols, topLineAreaEnhanced_.rows)));
        cv::cvtColor(topLineAreaEdges_, color, cv::COLOR_GRAY2BGR);
        color.copyTo(topLineArea(cv::Rect(topLineAreaEnhanced_.cols+20, 0, topLineAreaEdges_.cols, topLineAreaEdges_.rows)));
        cv::cvtColor(topLineAreaThreshold_, color, cv::COLOR_GRAY2BGR);
        color.copyTo(topLineArea(cv::Rect(0, topLineAreaEnhanced_.rows+20, topLineAreaThreshold_.cols, topLineAreaThreshold_.rows)));

        height = armLineAreaEnhanced_.rows + 20 + armLineAreaEdges_.rows;
        width = armLineAreaEnhanced_.cols + 20 + armLineAreaThreshold_.cols;
        armLineArea.create(height, width, CV_8UC3);
        armLineArea.setTo(cv::Scalar(0,0,0));
        armLineAreaEnhanced_.copyTo(armLineArea(cv::Rect(0, 0, armLineAreaEnhanced_.cols, armLineAreaEnhanced_.rows)));
        cv::cvtColor(armLineAreaEdges_, color, cv::COLOR_GRAY2BGR);
        color.copyTo(armLineArea(cv::Rect(armLineAreaEnhanced_.cols+20, 0, armLineAreaEdges_.cols, armLineAreaEdges_.rows)));
        cv::cvtColor(armLineAreaThreshold_, color, cv::COLOR_GRAY2BGR);
        color.copyTo(armLineArea(cv::Rect(0, armLineAreaEnhanced_.rows+20, armLineAreaThreshold_.cols, armLineAreaThreshold_.rows)));

        imshow("TopLineArea", topLineArea); 
        imshow("ArmLineArea", armLineArea); 
        imshow("result", mFrame_); 

        if(27 == cv::waitKey((int)(1000/dFps_)))
            break;
        
        if(bVideoFlag_){
            if(bSaveToFile_)
                saveToFile(mFrame_, videoName);

            vCap_.read(mFrame_);
        }
        else   
            break;  // 如果是图片, 仅处理一次即可
    }
    LOG(INFO) << "Video Transform Finished.";
}

int TopLineDetector::detectTopLine(const cv::Rect &roi, cv::Point &pt1Detect, cv::Point &pt2Detect)
{
    // 预处理
    DISTANCE line_distance_map; 
    // cv::Point pt1Detect, pt2Detect;
    double k, distance;
    cv::Mat element3(3,3, CV_8U, cv::Scalar(1));
    cv::Mat element5(5,5, CV_8U, cv::Scalar(1));
    int ksize = 5;
    // setROI(roi);
    cv::Mat topLineRoi = mFrame_(roi);
    cv::GaussianBlur(topLineRoi, topLineRoi, cv::Size(ksize, ksize), 0, 0); 
    // image_enhance(topLineRoi, topLineAreaEnhanced_, ENHANCE_HIST, NULL);
    image_enhance(topLineRoi, topLineAreaEnhanced_, ENHANCE_LAPLACE, NULL);
    threshold_adaptive(topLineAreaEnhanced_, topLineAreaThreshold_);
    cv::dilate(topLineAreaThreshold_, topLineAreaThreshold_, element3);
    cv::erode(topLineAreaThreshold_, topLineAreaThreshold_, element3, cv::Point(-1, -1), 4);
    
    // 霍夫直线检测, 注意参数的选取   
    cv::Canny(topLineAreaThreshold_, topLineAreaEdges_, 50, 220, 3);

    // 霍夫直线变换函数 HoughLinesP() 的参数说明:   
    cv::HoughLinesP(topLineAreaEdges_, topLineAreaLines_, 9, CV_PI/180, 20, 25, 20);
    // 轮廓的显示需要反转黑白值  
    cv::threshold(topLineAreaEdges_, topLineAreaEdges_, 128, 255, cv::THRESH_BINARY_INV);
    
    // 对直线按照离原点的距离进行排序  
    for ( auto it = topLineAreaLines_.begin(); it != topLineAreaLines_.end(); it++ ) {
    
        // 计算直线的角度
        topLineAreaLine_ = *it;
        iTopLineDeltaX_ = topLineAreaLine_[2] - topLineAreaLine_[0];
        iTopLineDeltaY_ = topLineAreaLine_[3] - topLineAreaLine_[1];
        k = (double)iTopLineDeltaY_ / iTopLineDeltaX_;

        distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);

        // 计算直线的距离   
        if (k > 0.5 && k < 1000){
            line_distance_map[distance] = *it;
        }

    }
    topLineAreaLines_.clear();
    
    std::vector<PAIR_DISTANCE> line_distance_vec( line_distance_map.begin(),
                                                  line_distance_map.end() ); 

    std::sort( line_distance_vec.begin(), line_distance_vec.end(), CmpByKey()); 

    // 根据距离, 斜率筛选合适的直线, 并绘制到图上  
    for( auto it =  line_distance_vec.begin(); 
        it != line_distance_vec.end(); 
        it++ 
    ){
        topLineAreaLine_ = it->second;
        iTopLineDeltaX_ = topLineAreaLine_[2] - topLineAreaLine_[0];
        iTopLineDeltaY_ = topLineAreaLine_[3] - topLineAreaLine_[1];
        k = iTopLineDeltaY_ / (double)iTopLineDeltaX_;
        dTopLineYDetect_ = (- topLineAreaLine_[0]) * k + topLineAreaLine_[1];
        

        // 组成向量 保证大致方向是一致的, 避免余弦的范围过大.      
        if (iTopLineDeltaX_ > 0){

            pTopLineVecDetect_.x = (double)iTopLineDeltaX_;
            pTopLineVecDetect_.y = (double)iTopLineDeltaY_;
        } else {

            pTopLineVecDetect_.x = (double)(- iTopLineDeltaX_);
            pTopLineVecDetect_.y = (double)(- iTopLineDeltaY_);
        }
        dTopLineSlopDetect_ = (double)(pTopLineVecDetect_.y) / pTopLineVecDetect_.x; 

        // 第一次使用时判断用户是否已经指定.
        if( bGotTopLine_ ) {
                
            dTheta_ = cosin_theta(pTopLineVecDetect_, pTopLineVecStd_);

        } else {

            std::cout << "!!! User should draw a top line first !!!" << std::endl;
            return -1;
        }
    
        // 距离应该满足的条件, 经验值.   
        if( (dTopLineSlopDetect_-dTopLineSlopStd_)<10*dEpsino_ // 斜率范围
            && (1.0-dTheta_)<dEpsino_ // 直线角度偏离不要太大 
            && std::abs(dTopLineYDetect_-dTopLineYStd_) < iOffset_ // y 偏移不要太大 
            ) 
        {                    
            LOG(INFO) << "\t\tGot a good top line" << it- line_distance_vec.begin();

            LOG(INFO) << "LineVecStd = (" << pTopLineVecStd_.x << ", " 
                        << pTopLineVecStd_.y << ")";
            LOG(INFO) << "Theta = " << dTheta_;
            // got_line = true;  // 找到一条合适的直线   
            pt1Detect = cv::Point((15 - topLineAreaLine_[1])/k + topLineAreaLine_[0], 15);
            pt2Detect = cv::Point(roi.width - 60, k*(roi.width - 60 - topLineAreaLine_[0]) + topLineAreaLine_[1]);
            cv::line(topLineRoi, pt1Detect, pt2Detect, cv::Scalar(0,0,255), 2, CV_AA);

            // 更新 pLineXxxStd_: 注意这里的直线斜率是不用更新的, 只做判断
            dTopLineYStd_ = dTopLineYDetect_;   // 更新 y
            // pTopLineVecStd_.x = pTopLineVecDetect_.x;
            // pTopLineVecStd_.y = pTopLineVecDetect_.y;
            line_distance_map.clear();   // 丢弃之前的直线数据
            
            break;
        } else {
            cv::line(topLineRoi, pt1Detect, pt2Detect, cv::Scalar(0,0,255), 2, CV_AA);
        }

        // 不做太多的数据处理
        if(it - line_distance_vec.begin() > 35){
            line_distance_map.clear();   // 丢弃之前的直线数据
            break;
        }
    } // end of iterate line_distance_vec.
    // cv::rectangle(mFrame_, cv::Rect(roi.x, roi.y+5, roi.width-5, roi.height-5), cv::Scalar(0,255,0), 1, CV_AA);
}

int TopLineDetector::detectArmLine(const cv::Rect &roi, cv::Point &pt1Detect, cv::Point &pt2Detect)
{
    // 预处理
    DISTANCE line_distance_map; 
    // cv::Point pt1Detect, pt2Detect;
    double k, distance;
    cv::Mat element5(5,5, CV_8U, cv::Scalar(1));
    cv::Mat element3(3,3, CV_8U, cv::Scalar(1));
    int ksize = 5;
    // setROI(roi);
    cv::Mat armLineRoi = mFrame_(roi);
    cv::GaussianBlur(armLineRoi, armLineRoi, cv::Size(ksize, ksize), 0, 0); 
    image_enhance(armLineRoi, armLineAreaEnhanced_, ENHANCE_HIST, NULL);

    threshold_adaptive(armLineAreaEnhanced_, armLineAreaThreshold_);
    cv::erode(armLineAreaThreshold_, armLineAreaThreshold_, element3, cv::Point(-1, -1), 3);
    
    // 霍夫直线检测, 注意参数的选取   
    cv::Canny(armLineAreaThreshold_, armLineAreaEdges_, 30, 200, 3);

    // 霍夫直线变换函数 HoughLinesP() 的参数说明:   
    cv::HoughLinesP(armLineAreaEdges_, armLineAreaLines_, 9, CV_PI/180, 20, 40, 20);
    // 轮廓的显示需要反转黑白值  
    cv::threshold(armLineAreaEdges_, armLineAreaEdges_, 128, 255, cv::THRESH_BINARY_INV);
    
    // 对直线按照离原点的距离进行排序  
    for ( auto it = armLineAreaLines_.begin(); it != armLineAreaLines_.end(); it++ ) {
    
        // 计算直线的角度
        armLineAreaLine_ = *it;
        iTopLineDeltaX_ = armLineAreaLine_[2] - armLineAreaLine_[0];
        iTopLineDeltaY_ = armLineAreaLine_[3] - armLineAreaLine_[1];
        k = (double)iTopLineDeltaY_ / iTopLineDeltaX_;

        distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);

        // 计算直线的距离   
        if (k < -0.5 && k > -100)
        {
            line_distance_map[distance] = *it;
        }
    }
    armLineAreaLines_.clear();
    
    std::vector<PAIR_DISTANCE> line_distance_vec( line_distance_map.begin(),
                                                  line_distance_map.end() ); 

    std::sort( line_distance_vec.begin(), line_distance_vec.end(), CmpByKeyReverse()); 

    // 根据距离, 斜率筛选合适的直线, 并绘制到图上  
    // std::vector<PAIR_DISTANCE>::iterator it;
    for( auto it =  line_distance_vec.begin(); 
        it != line_distance_vec.end(); 
        it++ 
    ){
        armLineAreaLine_ = it->second;
        iArmLineDeltaX_ = armLineAreaLine_[2] - armLineAreaLine_[0];
        iArmLineDeltaY_ = armLineAreaLine_[3] - armLineAreaLine_[1];
        k = iArmLineDeltaY_ / (double)iArmLineDeltaX_;
        dArmLineXDetect_ = (- armLineAreaLine_[1]) / k + armLineAreaLine_[0];
        

        // 组成向量 保证大致方向是一致的, 避免余弦的范围过大.      
        if (iArmLineDeltaX_ > 0){

            pArmLineVecDetect_.x = (double)iArmLineDeltaX_;
            pArmLineVecDetect_.y = (double)iArmLineDeltaY_;
        } else {

            pArmLineVecDetect_.x = (double)(- iArmLineDeltaX_);
            pArmLineVecDetect_.y = (double)(- iArmLineDeltaY_);
        }
        dArmLineSlopDetect_ = (double)(pArmLineVecDetect_.y) / pArmLineVecDetect_.x; 

        // 第一次使用时判断用户是否已经指定.
        if( bGotArmLine_ ) {
                
            dArmLineTheta_ = cosin_theta(pArmLineVecDetect_, pArmLineVecStd_);

        } else {

            std::cout << "!!! User should draw a arm line first !!!" << std::endl;
            return -1;
        }
    
        // 距离应该满足的条件, 经验值.   
        if( (dArmLineSlopDetect_-dArmLineSlopStd_)<10*dEpsino_ // 斜率范围
            && (1.0-dArmLineTheta_)<dEpsino_ // 直线角度偏离不要太大 
            && std::abs(dArmLineXDetect_-dArmLineXStd_) < iOffset_ // y 偏移不要太大 
        ) 
        {                    
            LOG(INFO) << "\t\tGot a good arm line" << it- line_distance_vec.begin();

            LOG(INFO) << "LineVecStd = (" << pArmLineVecStd_.x << ", " 
                        << pArmLineVecStd_.y << ")";
            LOG(INFO) << "Theta = " << dArmLineTheta_;
            pt1Detect = cv::Point((10-armLineAreaLine_[1])/k+armLineAreaLine_[0], 10);
            pt2Detect = cv::Point(80, k*(80-armLineAreaLine_[0])+armLineAreaLine_[1]);
            cv::line(armLineRoi, pt1Detect, pt2Detect, cv::Scalar(0,0,255), 2, CV_AA);
            // cv::line(armLineRoi, cv::Point(0,0), cv::Point(roi.width/2,roi.height/2), cv::Scalar(0,0,255), 2, CV_AA);

            // 更新 pLineXxxStd_: 注意这里的直线斜率是不用更新的, 只做判断
            dArmLineXStd_ = dArmLineXDetect_;   // 更新 x
            // pArmLineVecStd_.x = pArmLineVecDetect_.x;
            // pArmLineVecStd_.y = pArmLineVecDetect_.y;
            line_distance_map.clear();   // 丢弃之前的直线数据
            
            break;
        } else {
            cv::line(armLineRoi, pt1Detect, pt2Detect, cv::Scalar(0,0,255), 2, CV_AA);
        }

        // 不做太多的数据处理
        if(it - line_distance_vec.begin() > 35){
            line_distance_map.clear();   // 丢弃之前的直线数据
            break;
        }
    } // end of iterate line_distance_vec.
    // cv::rectangle(mFrame_, roi, cv::Scalar(0,255,0), 1, CV_AA);
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
            LOG(ERROR) << "Not supported Method: " << method; 
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
    LOG(INFO) << "Line: " << vec1[0] << ", " << vec1[1] << ", " 
            << vec2[0] << ", " << vec2[1];

    return cosin_theta(&vec1[0], &vec2[0], vec1.size());
}



void TopLineDetector::drawInitTopLine(const std::string &win_name, const cv::Rect &topLineRoi, const cv::Rect &armLineRoi)
{
    cv::Mat canvas = mFrame_.clone();
    
    while(1)
    {
        // 获取第一条线  
        if(!bGotTopLine_ && gClickTwice && gClickOnce) {
            pTopLineVecStd_.x = (double)(gPoint2.x - gPoint1.x);
            pTopLineVecStd_.y = (double)(gPoint2.y - gPoint1.y);
            double y = (double)(topLineRoi.x - gPoint1.x) * ((double)pTopLineVecStd_.y / pTopLineVecStd_.x) +  gPoint1.y;
            setTopLineYStd(y);
            dTopLineSlopStd_ = (double)(pTopLineVecStd_.y) / pTopLineVecStd_.x;

            if(pTopLineVecStd_.x < 0){
                pTopLineVecStd_.x = - pTopLineVecStd_.x;
                pTopLineVecStd_.y = - pTopLineVecStd_.y;
            }

            cv::line(canvas, gPoint1, gPoint2, cv::Scalar(0, 0, 255), 2);
            bGotTopLine_ = true;
            
            gClickTwice = false;
            gClickOnce = false;
        } 

        if(!bGotArmLine_ && gClickTwice && gClickOnce) {
            pArmLineVecStd_.x = (double)(gPoint2.x - gPoint1.x);
            pArmLineVecStd_.y = (double)(gPoint2.y - gPoint1.y);
            double x = (double)(armLineRoi.y - gPoint1.y) / ((double)pArmLineVecStd_.y / pArmLineVecStd_.x ) + gPoint1.x;
            
            setArmLineXStd(x - armLineRoi.x);

            dArmLineSlopStd_ = (double)(pArmLineVecStd_.y) / pArmLineVecStd_.x;

            if(pArmLineVecStd_.x < 0){
                pArmLineVecStd_.x = - pArmLineVecStd_.x;
                pArmLineVecStd_.y = - pArmLineVecStd_.y;
            }

            cv::line(canvas, gPoint1, gPoint2, cv::Scalar(0, 0, 255), 2);
            bGotArmLine_ = true;
        } 

        int key = cv::waitKey(10);
        if (key == 27)//按esc跳出  
            break;  
        else if(99 == key){
            canvas = mFrame_.clone();
            gClickTwice = false;
            gClickOnce = false;
            bGotArmLine_ = false;
            bGotTopLine_ = false;
        }

        cv::imshow(win_name, canvas); 

    }
    cv::destroyAllWindows();

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

                LOG(INFO) << "ClickTwice";
                LOG(INFO) << "x2 = " << x << ",  y2 = " << y;
                gClickTwice = true;
                gPoint2.x = x;
                gPoint2.y = y;
            }

            // 点击一次
            if (!gClickTwice && !gClickOnce){
                
                LOG(INFO) << "ClickOnce";
                LOG(INFO) << "x1 = " << x << ",  y1 = " << y;
                gClickOnce = true;
                gPoint1.x = x;
                gPoint1.y = y;
            }
        break;  

        case cv::EVENT_LBUTTONUP: //鼠标左键抬起  
        break;  
    }  
} 