#include "image_process.h"

using namespace cv;  
using namespace std;  

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

    // Main:  完成直线检测操作
    do_detection(image, pre_savename, &cap);

    return 0;
}