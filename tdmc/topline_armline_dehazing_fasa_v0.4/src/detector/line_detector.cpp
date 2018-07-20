#include "line_detector.h"
#include "fasa.h"


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
void LineDetector::adjustOffSet()
{
    int iExpTopLine = 0;
    int alphaTopLine = 1;
    int iExpArmLine = 0;
    int alphaArmLine = 1;
    // 动态调整阈值
    if(!bFindTopLine_)
        iTopLineLoseFrame_++;
    if(!bFindArmLine_)
        iArmLineLoseFrame_++;

    if(frames_ % STASTIC_FRAMES_ == 0 ){

        if( iArmLineLoseFrame_ == STASTIC_FRAMES_ 
            && iArmLineOffset_<MAX_OFFSET_ )
        {
            // 没有检测到 ArmLine
            if(!bArmLineLastDetect_)
                iExpArmLine++;
            alphaArmLine = std::pow(2, iExpArmLine);
            iArmLineOffset_+ alphaArmLine*OFFSET_STEP_ > MAX_OFFSET_ ?
                MAX_OFFSET_:iArmLineOffset_+=alphaArmLine*OFFSET_STEP_;
            
            bArmLineLastDetect_ = false;
        } else if( iArmLineLoseFrame_ <= (STASTIC_FRAMES_-1) 
                    && iArmLineOffset_>MIN_OFFSET_ )
        {
            bArmLineLastDetect_ = true;
            
            // iArmLineOffset_ > alphaArmLine*OFFSET_STEP_ ? 
                // iArmLineOffset_-=alphaArmLine*OFFSET_STEP_:MIN_OFFSET_;
            iArmLineOffset_ = MIN_OFFSET_;
            iExpArmLine = 0;
            alphaArmLine = 1;
        }

        if( iTopLineLoseFrame_ == STASTIC_FRAMES_ 
            &&  iTopLineOffset_<MAX_OFFSET_ ) {
            // 没有检测到 TopLine
            if(!bTopLineLastDetect_)
                iExpTopLine++;
            alphaTopLine = std::pow(2, iExpTopLine);
            iTopLineOffset_+ alphaTopLine*OFFSET_STEP_ > MAX_OFFSET_ ?
                MAX_OFFSET_:iTopLineOffset_+=alphaTopLine*OFFSET_STEP_;
            
            bTopLineLastDetect_ = false;
        } else if( iTopLineLoseFrame_ <= (STASTIC_FRAMES_-1) 
                    && iTopLineOffset_>MIN_OFFSET_ ) {
            bTopLineLastDetect_ = true;
            
            // iTopLineOffset_ > alphaTopLine*OFFSET_STEP_ ? 
            //     iTopLineOffset_-=alphaTopLine*OFFSET_STEP_:MIN_OFFSET_;
            iTopLineOffset_ = MIN_OFFSET_;
            iExpTopLine = 0;
            alphaTopLine = 1;
        }

        LOG(INFO) << "iTopLineOffset_ = " << iTopLineOffset_ << 
            ", iTopLineLoseFrame_ = " << iTopLineLoseFrame_ << 
            ", iExpTopLine = " << iExpTopLine;
        LOG(INFO) << "iArmLineOffset_ = " << iArmLineOffset_ << 
            ",   iArmLineLoseFrame_ = " << iArmLineLoseFrame_ << 
            ", iExpArmLine = " << iExpArmLine;
        
        iArmLineLoseFrame_ = 0;
        iTopLineLoseFrame_ = 0;
    }
}

int TopLineDetector::doDetection ( const int method, 
                                   const cv::Rect &topLineRoi, 
                                   const cv::Rect &armLineRoi, 
                                   const std::string &videoName
)
{
    if(bVideoFlag_ && !vCap_.isOpened()) {
        LOG(ERROR) << "Video open failed.";
        return -1;
    }
    
    cv::Mat topLineArea;
    cv::Mat armLineArea;
    int height, width;
    cv::Mat color;
    int fps = 0;
  
    while(mFrame_.data) {
        frames_++;
        start_ = boost::posix_time::microsec_clock::local_time();

        detectTopLine(topLineRoi);
        detectArmLine(armLineRoi);
        
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
        
        stop_ = boost::posix_time::microsec_clock::local_time();
        fElapsedMicroseconds_ = (stop_ - start_).total_milliseconds();
        fps = (int)(1000. / fElapsedMicroseconds_);
        cv::putText( mFrame_, 
                     std::to_string(fps)+" fps", 
                     cv::Point(20, mFrame_.rows - 20),
                     cv::FONT_HERSHEY_PLAIN,
                     1, cv::Scalar(255,0,0), 1, 8
                   );
        imshow("TopLineArea", topLineArea); 
        imshow("ArmLineArea", armLineArea); 
        imshow("result", mFrame_); 

        if(27 == cv::waitKey((int)(1000/dFps_)))
            break;

        adjustOffSet();

        if(bVideoFlag_){
            if(bSaveToFile_)
                saveToFile(mFrame_, videoName);
            vCap_.read(mFrame_);
        }
        else   
            break;  // 如果是图片, 仅处理一次即可
    }
}

int TopLineDetector::doDetection ( const int method,
                                   const cv::Mat &src, 
                                   const cv::Rect &topLineRoi, 
                                   const cv::Rect &armLineRoi, 
                                   const std::string &videoName
)
{
    if(!src.data) {
        LOG(ERROR) << " Invaild src image...";
        return -1;
    }

    cv::Rect_<float> bBox; 
    cv::Point pt1TopLineDetect, pt2TopLineDetect;
    cv::Point pt1ArmLineDetect, pt2ArmLineDetect;
    cv::Mat topLineArea;
    cv::Mat armLineArea;
    int height, width;
    cv::Mat color;
    int fps = 0;
    int retVal = -1;

    frames_++;
    start_ = boost::posix_time::microsec_clock::local_time();
    
    // fasa
    
    // src is the dehazed image
    retVal= do_fasa(mFrame_, bBox);
    mFrame_ = src;
    
    detectTopLine(topLineRoi);
    detectArmLine(armLineRoi);
    
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

    stop_ = boost::posix_time::microsec_clock::local_time();
    fElapsedMicroseconds_ = (stop_ - start_).total_milliseconds();
    fps = (int)(1000. / fElapsedMicroseconds_);
    cv::putText( mFrame_, 
                    std::to_string(fps)+" fps", 
                    cv::Point(20, mFrame_.rows - 20),
                    cv::FONT_HERSHEY_PLAIN,
                    1, cv::Scalar(255,0,0), 1, 8
                );

    if ( 0 == retVal ){

        cv::rectangle(mFrame_, bBox, Scalar(0,250,0), 1, CV_AA);
    }

    imshow("TopLineArea", topLineArea); 
    // imshow("ArmLineArea", armLineArea); 
    imshow("result", mFrame_); 
        
    if ( 27 == cv::waitKey(10)){

        cv::destroyAllWindows();
        exit(1);
    }

    adjustOffSet();
    saveToFile(mFrame_, videoName);
}

int TopLineDetector::detectTopLine(const cv::Rect &roi)
{
    // 预处理
    DISTANCE line_distance_map; 
    // cv::Point pt1TopLineDetect_, pt2TopLineDetect_;
    double k, distance;
    cv::Mat element3(3,3, CV_8U, cv::Scalar(1));
    cv::Mat element5(5,5, CV_8U, cv::Scalar(1));
    cv::Mat gray;
    int ksize = 5;
    int iLineWidth = 2;
    // setROI(roi);
    cv::Mat topLineRoi = mFrame_(roi);
    cv::GaussianBlur(topLineRoi, topLineRoi, cv::Size(ksize, ksize), 0, 0); 
    image_enhance(topLineRoi, topLineAreaEnhanced_, ENHANCE_LAPLACE, NULL);
    // image_enhance(topLineRoi, topLineAreaEnhanced_, ENHANCE_HIST, NULL);
    // cv::cvtColor(topLineAreaEnhanced_, gray, cv::COLOR_BGR2GRAY);
    threshold_adaptive(topLineRoi, topLineAreaThreshold_);
    // threshold_adaptive(topLineAreaEnhanced_, topLineAreaThreshold_);
    // cv::threshold(gray, topLineAreaThreshold_, 40, 255, cv::THRESH_BINARY);
    cv::dilate(topLineAreaThreshold_, topLineAreaThreshold_, element3, cv::Point(-1, -1), 1);
    cv::erode(topLineAreaThreshold_, topLineAreaThreshold_, element3, cv::Point(-1, -1), 3);
    
    // 霍夫直线检测, 注意参数的选取   
    // cv::Canny(topLineAreaThreshold_, topLineAreaEdges_, 50, 220, 3);
    cv::dilate(topLineAreaThreshold_, topLineAreaThreshold_, element3, cv::Point(-1, -1), 1);
    cv::erode(topLineAreaThreshold_, topLineAreaThreshold_, element3, cv::Point(-1, -1), 1);
    cv::Canny(topLineAreaThreshold_, topLineAreaEdges_, 50, 220, 3);
    // cv::imshow("enhanced", topLineAreaEnhanced_);
    // cv::imshow("threshold", topLineAreaThreshold_);
    // cv::waitKey(10);

    // 霍夫直线变换函数 HoughLinesP() 的参数说明:   
    cv::HoughLinesP(topLineAreaEdges_, topLineAreaLines_, 9, CV_PI/180, 20, 50, 20);
    // 轮廓的显示需要反转黑白值  
    cv::threshold(topLineAreaEdges_, topLineAreaEdges_, 128, 255, cv::THRESH_BINARY_INV);
    
    // 对直线按照离原点的距离进行排序  
    for ( auto it = topLineAreaLines_.begin(); it != topLineAreaLines_.end(); it++ ) {
    
        // 计算直线的角度
        topLineAreaLine_ = *it;
        iTopLineDeltaX_ = topLineAreaLine_[2] - topLineAreaLine_[0];
        iTopLineDeltaY_ = topLineAreaLine_[3] - topLineAreaLine_[1];
        k = (double)iTopLineDeltaY_ / iTopLineDeltaX_;

        // 计算直线的距离   
        distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);

        // constrain of slop
        if (k > 0.5 && k < 1000){
            line_distance_map[distance] = *it;
        }
    }
    topLineAreaLines_.clear();
    
    std::vector<PAIR_DISTANCE> line_distance_vec( line_distance_map.begin(),
                                                  line_distance_map.end() ); 

    std::sort( line_distance_vec.begin(), line_distance_vec.end(), CmpByKey()); 
    
    // 根据距离, 斜率筛选合适的直线, 并绘制到图上  
    bFindTopLine_ = false;
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
                
            dTopLineTheta_ = cosin_theta(pTopLineVecDetect_, pTopLineVecStd_);

        } else {

            std::cout << "!!! User should draw a top line first !!!" << std::endl;
            return -1;
        }
    
        // 距离应该满足的条件, 经验值.   
        if( (dTopLineSlopDetect_-dTopLineSlopStd_)<10*dTopLineEpsino_ // 斜率范围
            && (1.0-dTopLineTheta_)<dTopLineEpsino_ // 直线角度偏离不要太大 
            && std::abs(dTopLineYDetect_-dTopLineYStd_) < iTopLineOffset_ // y 偏移不要太大 
            ) 
        {
            LOG(INFO) << "\t\tGot a good top line" << it- line_distance_vec.begin();
            bFindTopLine_ = true;

            LOG(INFO) << "LineVecStd = (" << pTopLineVecStd_.x << ", " 
                        << pTopLineVecStd_.y << ")";
            LOG(INFO) << "Theta = " << dTopLineTheta_;
            // got_line = true;  // 找到一条合适的直线   
            pt1TopLineDetect_ = cv::Point((15 - topLineAreaLine_[1])/k + topLineAreaLine_[0], 15);
            pt2TopLineDetect_ = cv::Point(roi.width - 60, k*(roi.width - 60 - topLineAreaLine_[0]) + topLineAreaLine_[1]);


            // 更新 pLineXxxStd_: 注意这里的直线斜率是不用更新的, 只做判断
            dTopLineYStd_ = dTopLineYDetect_;   // 更新 y
            // pTopLineVecStd_.x = pTopLineVecDetect_.x;
            // pTopLineVecStd_.y = pTopLineVecDetect_.y;
            line_distance_map.clear();   // 丢弃之前的直线数据
            
            break;
        }

        
        // maybe we should go to next frame
        if(it - line_distance_vec.begin() > 35){
            line_distance_map.clear();   // 丢弃之前的直线数据
            break;
        }
    } // end of iterate line_distance_vec.
    
    // draw a line, it may be the same line as previous frame.
    // topLineRoi = mFrame_(roi);
    cv::line(topLineRoi, pt1TopLineDetect_, pt2TopLineDetect_, cv::Scalar(0,0,255), iLineWidth, CV_AA);
    // cv::rectangle(mFrame_, cv::Rect(roi.x, roi.y+5, roi.width-5, roi.height-5), cv::Scalar(0,255,0), 1, CV_AA);
}

int TopLineDetector::detectArmLine(const cv::Rect &roi)
{
    // 预处理
    DISTANCE line_distance_map; 
    double k, distance;
    cv::Mat element5(5,5, CV_8U, cv::Scalar(1));
    cv::Mat element3(3,3, CV_8U, cv::Scalar(1));
    cv::Mat gray;
    int ksize = 5;
    int iLineWidth = 2;
    // setROI(roi);
    cv::Mat armLineRoi = mFrame_(roi);
    cv::GaussianBlur(armLineRoi, armLineRoi, cv::Size(ksize, ksize), 0, 0); 
    image_enhance(armLineRoi, armLineAreaEnhanced_, ENHANCE_HIST, NULL);

    // cv::cvtColor(armLineAreaEnhanced_, gray, cv::COLOR_BGR2GRAY);
    threshold_adaptive(armLineRoi, armLineAreaThreshold_);
    // threshold_adaptive(armLineAreaEnhanced_, armLineAreaThreshold_);
    // cv::threshold(armLineAreaThreshold_, armLineAreaThreshold_, 200, 255, cv::THRESH_BINARY);
    cv::erode(armLineAreaThreshold_, armLineAreaThreshold_, element3, cv::Point(-1, -1), 1);
    
    // 霍夫直线检测, 注意参数的选取   
    cv::Canny(armLineAreaThreshold_, armLineAreaEdges_, 30, 200, 3);

    // 霍夫直线变换函数 HoughLinesP() 的参数说明:   
    cv::HoughLinesP(armLineAreaEdges_, armLineAreaLines_, 9, CV_PI/180, 20, 50, 20);
    // 轮廓的显示需要反转黑白值  
    cv::threshold(armLineAreaEdges_, armLineAreaEdges_, 128, 255, cv::THRESH_BINARY_INV);
    
    // 对直线按照离原点的距离进行排序  
    bFindArmLine_ = false;
    for ( auto it = armLineAreaLines_.begin(); it != armLineAreaLines_.end(); it++ ) {
    
        // 计算直线的角度
        armLineAreaLine_ = *it;
        iTopLineDeltaX_ = armLineAreaLine_[2] - armLineAreaLine_[0];
        iTopLineDeltaY_ = armLineAreaLine_[3] - armLineAreaLine_[1];
        k = (double)iTopLineDeltaY_ / iTopLineDeltaX_;

        distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);

        // 计算直线的距离   
        if (k < -0.5 && k > -100) {
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
        if( (dArmLineSlopDetect_-dArmLineSlopStd_)<10*dArmLineEpsino_ // 斜率范围
            && (1.0-dArmLineTheta_)<dArmLineEpsino_ // 直线角度偏离不要太大 
            && std::abs(dArmLineXDetect_-dArmLineXStd_) < iArmLineOffset_ // y 偏移不要太大 
        ) 
        {                    
            LOG(INFO) << "\t\tGot a good arm line" << it- line_distance_vec.begin();
            bFindArmLine_ = true;

            LOG(INFO) << "LineVecStd = (" << pArmLineVecStd_.x << ", " 
                        << pArmLineVecStd_.y << ")";
            LOG(INFO) << "Theta = " << dArmLineTheta_;
            pt1ArmLineDetect_ = cv::Point((10-armLineAreaLine_[1])/k+armLineAreaLine_[0], 10);
            pt2ArmLineDetect_ = cv::Point(80, k*(80-armLineAreaLine_[0])+armLineAreaLine_[1]);

            // 更新 pLineXxxStd_: 注意这里的直线斜率是不用更新的, 只做判断
            dArmLineXStd_ = dArmLineXDetect_;   // 更新 x
            // pArmLineVecStd_.x = pArmLineVecDetect_.x;
            // pArmLineVecStd_.y = pArmLineVecDetect_.y;
            line_distance_map.clear();   // 丢弃之前的直线数据
            
            break;
        } 

        // 不做太多的数据处理
        if(it - line_distance_vec.begin() > 35){
            line_distance_map.clear();   // 丢弃之前的直线数据
            break;
        }
    } // end of iterate line_distance_vec.
    
    cv::line(armLineRoi, pt1ArmLineDetect_, pt2ArmLineDetect_, cv::Scalar(0,0,255), iLineWidth, CV_AA);
    // cv::rectangle(mFrame_, roi, cv::Scalar(0,255,0), 1, CV_AA);
}


void LineDetector::threshold_adaptive(const cv::Mat &src, cv::Mat &dst)
{
    // cv::Mat gray(src.size(), CV_8UC1);
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
    // cv::threshold(gray, dst, 60, 255, cv::THRESH_BINARY);
    cv::adaptiveThreshold(gray, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 91, 15);
    // cv::adaptiveThreshold(gray, dst, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 81, 15);
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

    std::string winText = "Draw TopLine first, then the ArmLine";
    cv::putText( canvas, winText, cv::Point(10, canvas.rows-40), 
                 cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);
    cv::putText( canvas, "Press C to clear ", cv::Point(10, canvas.rows-20), 
                 cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 255, 255), 1);
    
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