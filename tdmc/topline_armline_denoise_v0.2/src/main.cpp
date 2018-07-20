#include "line_detector.hpp"

#define DRAW_LINE

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

    FLAGS_alsologtostderr = 1;
    // FLAGS_log_dir = "../logs";   // log dir

    coal::TopLineDetector *td;
    cv::Mat image;
    std::string winName = "Draw";

    // 根据图片或视频流选择不同的文件类型处理模式
    size_t pos = std::string(filename).rfind(".");
    std::string file_type(filename.begin()+pos, filename.end());
    // 构造保存文件名
    std::string pre_savename(filename.begin(), filename.begin()+pos);
    LOG(INFO) << "File Type: " << file_type;

    if(file_type == ".avi" || file_type == ".mp4"){
        // 处理视频流
        cv::VideoCapture cap = cv::VideoCapture(filename);
        if(!cap.isOpened()){

            LOG(ERROR) << "Video Open Failed!";
            return -1;
        }
        cap.read(image);
        td = new coal::TopLineDetector(cap);
    }else if (file_type == ".jpg" || file_type == ".png"){
        // 处理单张图片
        image = cv::imread(filename);
        if (image.empty()) {

            LOG(ERROR) << "Open Image Failed!";
            return -1;
        }
        td = new coal::TopLineDetector(image);
    }

    cv::Rect topLineRoi(image.cols/2, 0, image.cols/2, image.rows/2-20);
    cv::Rect ArmLineRoi(image.cols/2 -20, image.rows/2 - 20, image.cols/2, image.rows/2);

    td->setFps(999.);   // 只是为了测试极限帧率

    // 绘制初始化直线
    cv::namedWindow(winName);
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
                     "output.avi"
                   );
    delete td;
    td = 0;

    return 0;
}
