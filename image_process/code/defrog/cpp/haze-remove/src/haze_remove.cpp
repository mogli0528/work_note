#include "guidedfilter.h"
#include "haze_remove.h"

/**
 * 计算暗通道
 *    思路很简单: prior box 内的像素值的最小值, 
 *              先计算三通道中的最小值作为暗通道图的像素值.  
 * 
 *   公式: J^{dark_channel_}(x) = 
 *                               min_{prior_box} ( min_{r,g,b}( J^c(y) ) )
*/
cv::Mat& HazeRemove::DarkChannelPrior()
{
    float *dc_ptr = (float *)(dark_channel_.data);
    float *src_ptr = (float *)(real_img_.data);
    int dc_idx = 0, src_idx = 0;

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {

            src_idx = i * width_*3 + j * 3;
            dc_idx = i * width_ + j;

            // // 求 RGB 最小值
            // dark_channel_.at<float>(i, j) = min(
            //         min(real_img_.at<cv::Vec3f>(i, j)[0], real_img_.at<cv::Vec3f>(i, j)[1]),
            //         min(real_img_.at<cv::Vec3f>(i, j)[0], real_img_.at<cv::Vec3f>(i, j)[2])
            //     );


            // 求 RGB 最小值
            dc_ptr[dc_idx] = min(
                min(src_ptr[src_idx], src_ptr[src_idx+1]),
                min(src_ptr[src_idx], src_ptr[src_idx+2])
                );
        }
    }
    
    // 腐蚀, 对应的是最小值滤波. 
    cv::erode(dark_channel_, dark_channel_, Mat::ones(prior_box_size_, prior_box_size_, CV_32FC1));
    
    return dark_channel_;
}

/**
 * 解决去雾之后天空太亮的问题, 同时计算 A 的值  
 *    思路: 
 *       1. 准备一个容器, 比如要找到 0.1% 的像素, 那么容器大小为 0.001 * size_;
 *       2. 每次从零开始挑选像素中的最大值, 如果被当做最大值挑选出来, 就标记像素为0. 
 *       3. 这样就会依次按照从高到低的亮度最高的像素.  
 * 
 * 这个步骤是最耗时的, n_bright 可以适量降低比例.   
*/
std::vector<float>& HazeRemove::refine_sky_color()
{
    int n_bright = top_brightness_ * size_;
    // n_bright = 10;
    
    // dark_1 是一个有图片像素的单行矩阵, 方便下面循环计算
    Mat dark_1 = dark_channel_.reshape(1, size_); 
    vector<int> bright_pixels;
    float max_pixel = 0.;
    unsigned int pos = 0;
   
    std::vector<cv::Vec3f> RGBPixcels(n_bright);

    for (int i = 0; i < n_bright; i++) {
        max_pixel = 0.;
        bright_pixels.push_back(0);  // 其实就是先占位, 因为后边可能会挑出一个极大值
        
        // 遍历整个 dark_channel_ 图片, 找出亮度值最高的前 0.1% 像素(即含雾的部分)
        for (float * ptr = (float *)dark_1.datastart; ptr != (float *)dark_1.dataend; ptr++) {
            
            // 找到一个较大像素
            if (*ptr - max_pixel > 0.00000000001) {
                
                max_pixel = *ptr; // 记录光照的最大值
                pos = ptr - (float *)dark_1.datastart;
                bright_pixels[i] = pos; // 替换为这个较大像素值的位置

                // 在原始图像中找出[暗通道图中亮度最高的像素位置处]的像素, A 就是这些像素的均值.
                RGBPixcels[i] = ((cv::Vec3f *)real_img_.data)[pos];
            }
        }

        ((float *)dark_1.data)[pos] = 0.; // 访问过像素值的标记为0，这样就不会重复访问
    }
 
    // 将光照值累加
    for (int j = 0; j < n_bright; j++) {
        A_[0] += RGBPixcels[j][0];
        A_[1] += RGBPixcels[j][1];
        A_[2] += RGBPixcels[j][2];
    } 

    // 除以总数, 即取所有符合的点的平均值。
    A_[0] /= n_bright;
    A_[1] /= n_bright;
    A_[2] /= n_bright; 

    return A_;
}

/**
 * 计算暗通道, 考虑进去大气光照值 A 的作用  
 *      J^{dark_channel_out_}(x)=min( min( J^c(y) ) )
*/
cv::Mat& HazeRemove::DarkChannelPrior_A()
{
    // 全球大气光照值 此处是 3 通道的平均值
    double A_mean = (A_[0] + A_[1] + A_[2]) / 3.0; 

    float *dc_ptr = (float *)(dark_channel_out_.data);
    float *src_ptr = (float *)(real_img_.data);
    int dc_idx = 0, src_idx = 0;

    for (int i = 0; i < height_; i++) {
        for (int j = 0; j < width_; j++) {

            src_idx = i * width_*3 + j * 3;
            dc_idx = i * width_ + j;
            
            // // 求 RGB 最小值
            // dark_channel_out_.at<float>(i, j) = min(
            //     min(real_img_.at<cv::Vec3f>(i, j)[0]/ A_mean, real_img_.at<cv::Vec3f>(i, j)[1]/ A_mean),
            //     min(real_img_.at<cv::Vec3f>(i, j)[0]/ A_mean, real_img_.at<cv::Vec3f>(i, j)[2]/ A_mean)
            //     );

            // 求 RGB 最小值
            dc_ptr[dc_idx] = min(
                min(src_ptr[src_idx] / A_mean, src_ptr[src_idx+1] / A_mean),
                min(src_ptr[src_idx] / A_mean, src_ptr[src_idx+2] / A_mean)
                );
        }
    }
    
    // 腐蚀, 对应的是最小值滤波 
    cv::erode(dark_channel_out_, dark_channel_out_, Mat::ones(prior_box_size_, prior_box_size_, CV_32FC1));
    
    return dark_channel_out_;
}

// Calculate Transmission Matrix
cv::Mat& HazeRemove::TransmissionMat()
{   
    double A_mean = (A_[0] + A_[1] + A_[2]) / 3.0;
    
    for (int i = 0; i < dark_channel_out_.rows; i++)
    {
        for (int j = 0; j < dark_channel_out_.cols; j++)
        {
            double temp = (dark_channel_out_.at<float>(i, j));
            double B = fabs(A_mean - temp);
            //if (B==)
            // K = 80    80/255=0.31, 这里浮点数要这样做减法才能正确的比较
            if (B - 0.3137254901960784 < 0.0000000000001) {
                trans_mat_refine_.at<float>(i, j) = (1 - w_*dark_channel_out_.at<float>(i, j))*
                    (0.3137254901960784 / (B));
            } else {
                trans_mat_refine_.at<float>(i, j) = 1 - w_*dark_channel_out_.at<float>(i, j);
            }
            
            // 保证 Tx 不失真，因为会以上除出的结果会有不对
            if (trans_mat_refine_.at<float>(i, j) <= 0.2) {
                trans_mat_refine_.at<float>(i, j) = 0.5;
            }
            // 同上
            if (trans_mat_refine_.at<float>(i, j) >= 1){
                trans_mat_refine_.at<float>(i, j) = 1.0;
            }
        }
    }

    return trans_mat_refine_;
}

cv::Mat& HazeRemove::TransmissionMat_simple()
{
    for (int i = 0; i < dark_channel_.rows; i++) {
        for (int j = 0; j < dark_channel_.cols; j++) {
            trans_mat_refine_.at<float>(i, j) = (1 - w_*dark_channel_.at<float>(i, j));
        }
    }

    return trans_mat_refine_;
}

/**
 * Calculate Haze Free Image 
 *     \@param exposure: 去雾后应该加亮的值。是用来增加亮度的， 0.1 比较好
*/
cv::Mat& HazeRemove::haze_remove(cv::Mat& t, float exposure)
{
    // 取 A 中的最大的值
    double AAA = A_[0];

    if (A_[1] > AAA)
        AAA = A_[1];
    if (A_[2] > AAA)
        AAA = A_[2];

    // reforce_img_ = Mat::zeros(height_, width_, CV_32FC3);
    real_img_.copyTo(reforce_img_);

    // 两个迭代器，这样的写法可以不用两层循环，比较快点
    cv::Vec3f * p = (cv::Vec3f *)reforce_img_.datastart;
    float * q = (float *)t.datastart;

    for (; p<(cv::Vec3f *)reforce_img_.dataend && q < (float *)t.dataend; p++, q++)
    {
        (*p)[0] = ((*p)[0] - AAA) / std::max(*q, t0_) + AAA + exposure;
        (*p)[1] = ((*p)[1] - AAA) / std::max(*q, t0_) + AAA + exposure;
        (*p)[2] = ((*p)[2] - AAA) / std::max(*q, t0_) + AAA + exposure;
    }

    return reforce_img_;
}

void printMatInfo(const string name, Mat& m)
{
    cout << name << ":" << endl;
    cout << "\t" << "cols=" << m.cols << endl;
    cout << "\t" << "rows=" << m.rows << endl;
    cout << "\t" << "channels=" << m.channels() << endl;
}
