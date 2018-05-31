#include "image_process.h"
#include "line_detector.hpp"


#define DRAW_LINE

/***
 * 液压支架顶板检测
 *
 * 约束条件: 区域约束, 距离约束
 *
 * 用户提前根据顶板指定一条线, 这样就会有先验知识;
 * 之后的检测取舍都会以此为参考, 缓慢过渡.
 *
 * TODO:
 *    使用多线程来分割图像结果保存, 图像正常处理
 *
 * 　　./topline /home/klm/work/td_marco/images/video/coal_machine.avi
*/

int main(int argc, char **argv)
{
    std::string filename;

    if(argc == 2){
        filename = std::string(argv[1]);

        LOG(INFO) << "Specify a video path or a camera index";
    } else {
        LOG(INFO) << "Use Default Test Video...";
        filename = "/home/klm/work/td_marco/images/video/coal_machine.avi";
    }

    ::google::InitGoogleLogging(argv[0]);
    // Provide a backtrace on segfault.
    ::google::InstallFailureSignalHandler();
    FLAGS_log_dir = "../logs";

    coal::TopLineDetector *td;
    cv::Mat image;
    std::string winName = "请绘制直线(先绘制顶梁, 后绘制摇臂), 重画请按 \"C\" ";

    // 根据图片或视频流选择不同的文件类型处理模式
    size_t pos = std::string(filename).rfind(".");
    string file_type(filename.begin()+pos, filename.end());
    // 构造保存文件名
    string pre_savename(filename.begin(), filename.begin()+pos);
    LOG(INFO) << "File Type: " << file_type;

    if(file_type == ".avi" || file_type == ".mp4"){
        // 处理视频流
        VideoCapture cap = cv::VideoCapture(filename);
        if(!cap.isOpened()){

            LOG(ERROR) << "Video Open Failed!";
            return -1;
        }
        cap.read(image);
        td = new coal::TopLineDetector(cap);
    }else if (file_type == ".jpg" || file_type == ".png"){
        // 处理单张图片
        image = imread(filename);
        if (image.empty()) {

            LOG(ERROR) << "Open Image Failed!";
            return -1;
        }
        td = new coal::TopLineDetector(image);
    }

    cv::Rect topLineRoi(image.cols/2, 0, image.cols/2, image.rows/2-20);
    cv::Rect ArmLineRoi(image.cols/2 -20, image.rows/2 - 20, image.cols/2, image.rows/2);

    // td->setFps(999.);   // 只是为了测试极限帧率

    // 绘制初始化直线
    namedWindow(winName);
    cv::setMouseCallback(winName, on_mouse, (void*)&image);   //鼠标操作回调函数
    td->drawInitTopLine(winName, topLineRoi, ArmLineRoi);

    // 设置 ROI 区域并检测
    td->setToSaveVideo(true);
    td->setTopLineEpsino(0.0015);
    td->setTopLineOffset(2);
    td->setArmLineEpsino(0.0015);
    td->setArmLineOffset(2);
    td->doDetection( coal::ENHANCE_LAPLACE,
                     topLineRoi,
                     ArmLineRoi,
                     "/home/klm/work/td_marco/images/video/coal_machine_output.avi"
                   );
    delete td;
    td = 0;

    return 0;
}
