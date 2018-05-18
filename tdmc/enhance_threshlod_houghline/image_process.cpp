#include "image_process.h"

/***
 * 液压支架顶板检测   
 * 
 *   多尺度是否会有效?
 * 
 * 约束条件: 区域约束, 距离约束
 * 时间连续性
 * 
 * 对二值化的图进行膨胀后, 使用大一点的邻域来查找直线.   
 * 
 * 用户提前根据顶板指定一条线, 这样就会有先验知识;    
 * 之后的检测取舍都会以此为参考, 缓慢过渡.   
*/
typedef pair<float, Vec4i> PAIR_DISTANCE; 
typedef map<float, Vec4i> DISTANCE;    

struct CmpByKey {    
  bool operator()(const PAIR_DISTANCE& lhs, const PAIR_DISTANCE& rhs) {    
    return lhs.first < rhs.first;    
  }    
};  

int main(int argc, char **argv)
{   
    bool got_line = false;
    static bool video = false;
    VideoCapture cap;
    Mat image;

    if(argc < 2){
        std::cout << "Usage: image_process filename " << std::endl;
        return -1;
    }

    // 构造保存文件名
    string filename(argv[1]);
    size_t pos = string(filename).rfind(".");
    string file_type(filename.begin()+pos, filename.end());
    string pre_savename(filename.begin(), filename.begin()+pos);

    std::cout << "file_type: " << file_type << std::endl;
    if(file_type == ".avi" || file_type == ".mp4"){
        // 处理视频流
        video = true;
        cap = VideoCapture(filename);
        if(cap.isOpened())
            cap.read(image);
    }else if (file_type == ".jpg" || file_type == ".png"){
        // 处理单张图片
        image = imread(filename);
        if (image.empty()) {
            std::cout << "Open Image Failed!" << std::endl;

            return -1;
        }

    }

    // 保存处理结果为视频文件
    float fpsInput=24; // 帧率  
    Size video_size = Size(cap.get(CV_CAP_PROP_FRAME_WIDTH),cap.get(CV_CAP_PROP_FRAME_HEIGHT)); 
    int fourcc = CV_FOURCC('M', 'P', '4', '2');
    VideoWriter vw(pre_savename+"_out.avi", fourcc, fpsInput, video_size, true);
    
    /// 保存图像增强后的结果  
    // imwrite(pre_savename+"_hist.jpg", enhanced);
    // imwrite(pre_savename+"_laplace.jpg", enhanced);
    // imwrite(pre_savename+"_log.jpg", enhanced);
    // imwrite(pre_savename+"_gamma.jpg", enhanced);

   
    do {
        if(video){
            cap.read(image);
        }

        got_line = false;  

        // 图像增强处理   
        Mat enhanced, roi;
        roi = image(Rect(image.cols/2, 0, image.cols/2, image.rows/2));
        // 开运算
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
        // erode(enhanced, enhanced, element7);
        dilate(enhanced, enhanced, element3);

        // 霍夫直线检测   
        Mat edges;
        // Canny(threshold, edges, 60, 250, 3);
        Canny(enhanced, edges, 50, 220, 3);
        Mat result(roi);
        // cvtColor(enhanced, gray, COLOR_BGR2GRAY);


        /***
         * 参数说明:   
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

        // 直线拟合  
        // Mat one_line(edges.size(), CV_8U, Scalar(0));

        // 绘制直线   
        for (vector<Vec4i>::iterator it = lines.begin(); it != lines.end(); it++) {

            // 计算直线的角度
            k = ((*it)[3]- (*it)[1]+0.0) / ((*it)[2] - (*it)[0]);
            std::cout <<  (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << ", " << (*it)[3] << std::endl;

            distance = std::abs(k * (*it)[1] - (*it)[0]) / std::sqrt(k * k + 1);
            // line_distance_map_[*it] = distance;

            std::cout << k << std::endl;

            // 计算直线的距离   
            if (k > 0.5 && k < 1000){
                line_distance_map[distance] = *it;
                // line(gray, Point((*it)[0], (*it)[1]), Point((*it)[2], (*it)[3]), Scalar(0,0,255), 2, CV_AA);

            }
        }
        vector<PAIR_DISTANCE> line_distance_vec(line_distance_map.begin(), line_distance_map.end()); 
        sort(line_distance_vec.begin(), line_distance_vec.end(), CmpByKey()); 

        // 筛选合适的直线:    
        Vec4i line;
        
        for(vector<PAIR_DISTANCE>::iterator it =  line_distance_vec.begin(); it != line_distance_vec.end(); it++){
            
            line = it->second;
            if(it->first > 0.8 && it->first < 100 && abs(line[2] - line[0]) > 10){
                got_line = true;
                
                // 绘制直线  
                // cv::line(result, Point(line[0], line[1]), Point(line[2], line[3]), Scalar(0,0,255), 2, CV_AA);

                k = (line[3]- line[1]+0.0) / (line[2] - line[0]);
                cv::line(result, Point(line[0]-line[1]/k, 0), Point(line[2], line[3]), Scalar(0,0,255), 2, CV_AA);
                break;
            }
        }
        
        // for (size_t i = 0; i < lines_scale.size(); i++) {
        //     Vec4i (*it) = lines_scale[i];
        //     std::cout <<  (*it)[0] << ", " << (*it)[1] << ", " << (*it)[2] << ", " << (*it)[3] << std::endl;
        //     line(gray, Point((*it)[0], (*it)[1]), Point((*it)[2], (*it)[3]), Scalar(0,255,0), 4);
        // }
        // image(Rect(image.cols/2, 0, image.cols/2, image.rows/2)) = result;
        imshow("laplace 图像增强效果", enhanced);
        imshow("edges", edges); 
        imshow("with lines", image); 
        vw << image;
        // imshow("threshold_", threshold_); 

        if(27 == waitKey(150))
            break;


    }while(video);

    return 0;
}

/***
 * 自适应阈值处理 
 * 
*/
void threshold_adaptive(Mat &src, Mat &dst)
{
    Mat gray(src.size(), CV_8UC1);
    cvtColor(src, gray, COLOR_BGR2GRAY);
    adaptiveThreshold(gray, dst, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 81, 15);

    return;
}

/***
 * 
 * TODO: param 用于对应每种算法的特定参数
*/
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