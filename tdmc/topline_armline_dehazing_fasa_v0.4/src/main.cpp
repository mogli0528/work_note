#include "line_detector.h"
#include "dehazing.h"

#define DRAW_LINE

int main(int argc, char **argv)
{
    std::string filename;
    int n_frames = 1000;
    int nWid, nHei;
    cv::VideoCapture cap;

    if(argc ==2 ){
        n_frames = std::stoi(std::string(argv[1]));

    } if(argc == 3){
        n_frames = std::stoi(std::string(argv[1]));
        filename = std::string(argv[2]);
        LOG(INFO) << "video path (or a camera index)" << filename;
    } else {
        LOG(INFO) << "Use Default Test Video...";
        filename = "/home/klm/work/td_marco/images/video/coal_machine.avi";
    }

    ::google::InitGoogleLogging(argv[0]);
    // Provide a backtrace on segfault.
    ::google::InstallFailureSignalHandler();

    FLAGS_alsologtostderr = 1;
    // FLAGS_log_dir = "../logs";

    coal::TopLineDetector *td;
    cv::Mat image;
    std::string winName = "请绘制直线(先绘制顶梁, 后绘制摇臂), 重画请按 \"C\" ";

    // 根据图片或视频流选择不同的文件类型处理模式
    size_t pos = std::string(filename).rfind(".");
    std::string file_type(filename.begin()+pos, filename.end());
    // 构造保存文件名
    std::string pre_savename(filename.begin(), filename.begin()+pos);
    LOG(INFO) << "File Type: " << file_type;

    if(file_type == ".avi" || file_type == ".mp4"){
        // 处理视频流
        cap = cv::VideoCapture(filename);
        if(!cap.isOpened()){

            LOG(ERROR) << "Video Open Failed!";
            return -1;
        }
        cap.read(image);
        nWid = cap.get(CV_CAP_PROP_FRAME_WIDTH);
        nHei = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
        LOG(INFO) << "Video height: " << nHei << ", width: "  << nWid ;
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
    std::cout << "height: " << nHei << " width: "  << nWid << std::endl;

    cv::Rect topLineRoi(image.cols/2, 0, image.cols/2, image.rows/2-20);
    cv::Rect ArmLineRoi(image.cols/2 -20, image.rows/2 - 20, image.cols/2, image.rows/2);

    // td->setFps(999.);   // 只是为了测试极限帧率

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

    dehazing dehazingImg(nWid, nHei, 30, false, false, 5.0f, 1.0f, 40);
    IplImage imInput;
    IplImage *dehazed_image = cvCreateImage(cvSize(nWid, nHei),IPL_DEPTH_8U, 3);

    for (int i = 0; i< n_frames; i++) {
        cap.read(image);
        imInput = IplImage(image);
        dehazingImg.HazeRemoval(&imInput, dehazed_image, i);
        // cv::imshow("image", image);
        // cv::imshow("dehaze", cv::Mat(dehazed_image));
        // cv::waitKey(10);

        td->doDetection( coal::ENHANCE_LAPLACE,
                         cv::Mat(dehazed_image),
                         topLineRoi,
                         ArmLineRoi,
                        "output.avi"
                    );
    }

    delete td;
    td = 0;

    return 0;
}
