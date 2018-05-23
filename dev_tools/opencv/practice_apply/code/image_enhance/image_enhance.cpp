#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;  

int main(int argc, char **argv)
{
	Mat image = imread(argv[1]);
    if (image.empty()) {
        std::cout << "Open Image Failed!" << std::endl;

        return -1;
    }
    imshow("Origin", image);
    // int ksize = 9; 
    // GaussianBlur(image, image, Size(ksize, ksize), 0, 0); 
    // imshow("Origin_denoise", image);
    // 直方图均衡化   
    Mat image_rbg[3], image_hist;
    split(image, image_rbg);
    for (int i = 0; i < 3; i++) {
        equalizeHist(image_rbg[i], image_rbg[i]);
    }
    merge(image_rbg, 3, image_hist);
    add()

    // 拉普拉斯算子, 随着缩放因子的增大, 图像亮度越高
    Mat image_enhanced;
    Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0); 
    // Mat kernel = (Mat_<float>(3, 3) << 0, -2, 0, 0, 10, 0, 0, -2, 0); 
    // Mat kernel = (Mat_<float>(3, 3) << 0, -3, 0, 0, 15, 0, 0, -3, 0); 
    filter2D(image, image_enhanced, -1, kernel);

    // log 变换   
    Mat image_log(image.size(), CV_32FC3);
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
    convertScaleAbs(image_log, image_log);

    // gamma 变换(用于过曝光的场景), 如果 gamma > 1, 图像变暗.  如果gamma < 1, 图像变亮   
    Mat image_gamma(image.size(), CV_32FC3); 
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
    convertScaleAbs(image_gamma, image_gamma);  
	imshow("直方图均衡化图像增强效果", image_hist);
	imshow("拉普拉斯算子图像增强效果", image_enhanced);
	imshow("Log 变换图像增强效果", image_log);
	imshow("Gamma 变换图像增强效果", image_gamma);


	imshow("直方图均衡化图像增强效果", image_hist);
	imshow("拉普拉斯算子图像增强效果", image_enhanced);
	imshow("Log 变换图像增强效果", image_log);
	imshow("Gamma 变换图像增强效果", image_gamma);
    
	waitKey();

	return 0;
}