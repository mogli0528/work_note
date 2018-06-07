#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

int main(int argc, char **argv)
{
	cv::Mat image = cv::imread(argv[1]);
    cv::Mat out_bf, out_bl, out_gs, out_df;
    int ksize = 15; 
	int d = 18;

    cv::boxFilter(image, out_bf, -1, cv::Size(ksize, ksize)); 
    cv::blur(image, out_bl, cv::Size(ksize, ksize)); 
    cv::GaussianBlur(image, out_gs, cv::Size(ksize, ksize), 0, 0); 
    cv::bilateralFilter(image, out_df, d, d * 2, d / 2);  

	cv::imshow("Origin", image);
	cv::imshow("boxFilter", out_bf);
	cv::imshow("blur", out_bl);
	cv::imshow("Gaussian", out_gs);
	cv::imshow("bilateralFilter", out_df);
	cv::waitKey();

	return 0;
}