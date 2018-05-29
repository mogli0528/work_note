#include "image_process.h"
#include "line_detector.hpp"

#define DRAW_LINE

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

bool draw_line = true;   // 绘制  
bool click_once = false;   // 绘制  
bool click_twice = false;   // 绘制  
vector<float> vec_std(2, 0.0);      // 标准的直线方向: 起始值由用户指定.  
Point2i point1, point2; 
double y_std = 0.0;

int main_(int argc, char **argv)
{   
    bool got_line = false;
    static bool video = false;
    VideoCapture cap;
    Mat image, drawLine;
    string winName = "绘制初始线";

    if(argc < 2){
        std::cout << "Usage: image_process filename " << std::endl;
        return -1;
    }

    // 构造保存文件名
    string filename(argv[1]);
    size_t pos = string(filename).rfind(".");
    string file_type(filename.begin()+pos, filename.end());
    string pre_savename(filename.begin(), filename.begin()+pos);

    // 根据图片或视频流选择不同的文件类型处理模式   
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

    namedWindow(winName); 
    setMouseCallback(winName, on_mouse, (void*)&image);   //鼠标操作回调函数  
    image.copyTo(drawLine);
    
#ifdef DRAW_LINE
    // 用户绘制
    while(1)
    {
        if(click_twice && click_once) {
            vec_std[0] = (float)(point2.x - point1.x);
            vec_std[1] = (float)(point2.y - point1.y);
            if(vec_std[0] < 0){
                vec_std[0] = - vec_std[0];
                vec_std[1] = - vec_std[1];
            }

            cv::line(drawLine, point1, point2, Scalar(0, 0, 255), 2);
        } 

        imshow(winName, drawLine); 
        if (waitKey(10) == 27)//按esc跳出  
            break;  
    }
    destroyAllWindows();
#else
    // hard code
    vec_std[0] = 103;
    vec_std[1] = 72;
#endif
    y_std = (double)(vec_std[1]) / vec_std[0];

    // Main:  完成直线检测操作
    do_detection(image, vec_std, y_std, pre_savename, &cap);

    return 0;
}


int main(int argc, char **argv)
{
    if(argc < 2){
        std::cout << "Usage: image_process filename " << std::endl;
        return -1;
    }

    coal::TopLineDetector *td;
    cv::Mat image;
    std::string winName = "请绘制直线";

    // 根据图片或视频流选择不同的文件类型处理模式   
    std::string filename(argv[1]);
    size_t pos = std::string(filename).rfind(".");
    string file_type(filename.begin()+pos, filename.end());
    // 构造保存文件名
    string pre_savename(filename.begin(), filename.begin()+pos);
    std::cout << "file_type: " << file_type << std::endl;

    if(file_type == ".avi" || file_type == ".mp4"){
        // 处理视频流
        VideoCapture cap = cv::VideoCapture(filename);
        if(!cap.isOpened()){

            std::cout << "Video Open Failed!" << std::endl;
            return -1;
        }
        cap.read(image);
        td = new coal::TopLineDetector(cap);
    }else if (file_type == ".jpg" || file_type == ".png"){
        // 处理单张图片
        image = imread(filename);
        if (image.empty()) {

            std::cout << "Open Image Failed!" << std::endl;
            return -1;
        }
        td = new coal::TopLineDetector(image);
    }

    cv::Rect roi(image.cols/2, 0, image.cols/2, image.rows/2);
   
    // 绘制初始化直线
    namedWindow(winName); 
    cv::setMouseCallback(winName, on_mouse, (void*)&image);   //鼠标操作回调函数  
    td->drawInitLine(winName, roi);

    // 设置 ROI 区域并检测
    td->setToSaveVideo(true);
    
    td->doDetection(coal::ENHANCE_LAPLACE, roi);

    delete td;
    td = 0;

    return 0;
}