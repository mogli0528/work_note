#include "integer_image.hpp"

/**
 * 两个点将图像分割成 4 块  
*/
int* IntIntegralImage::getBlockSum(int x1, int y1, int block_width, int block_height) {

    int tl, tr, bl, br, s;

    // 右下角点  
    int y2 = y1 + block_height;
    int x2 = x1 + block_width;

    for (int i = 0; i < channels_; i++){

        int tl = sum_[i][y1*width_+x1];
        int tr = sum_[i][y2*width_+x1];
        int bl = sum_[i][y1*width_+x2];
        int br = sum_[i][y2*width_+x2];
        int s = (br - bl - tr + tl);

        blockSum_[i] = s;
    }
    
    return blockSum_;
}

int* IntIntegralImage::getBlockSquareSum(int x1, int y1, int block_width, int block_height) {
    
    int tl, tr, bl, br, var;

    // 右下角点  
    int y2 = y1 + block_height;
    int x2 = x1 + block_width;
    
    for (int i = 0; i < channels_; i++){

        tl = squaresum_[i][y1*width_+x1];
        tr = squaresum_[i][y2*width_+x1];
        bl = squaresum_[i][y1*width_+x2];
        br = squaresum_[i][y2*width_+x2];
        var = (br - bl - tr + tl);
        
        ssum_[i] = var;
    }

    return ssum_;
}

void IntIntegralImage::calculate_sum_and_squaresum() {

    int lineStep = this->image_.step;

    sum_.resize( channels_ ); 
    squaresum_.resize( channels_ );

    std::vector<std::vector<int> >::iterator it = sum_.begin();
    std::vector<std::vector<int> >::iterator itsq = squaresum_.begin();
    for( ; it != sum_.end(), itsq != squaresum_.end(); it++, itsq++ ) {
        it->resize(width_ * height_, 0);
        itsq->resize(width_ * height_, 0.0);
    }

    uchar* image_data = image_.data;

    // 1 channel and 3 channnels
    int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    int sp2 = 0, sp3 = 0, sp4 = 0;

    for(int row=1; row<height_; row++ ) {
        for(int col=1; col<width_; col++) {

            // 计算和查找表
            for ( int i = 0; i < channels_; i++ ) {
                
                // 计算和查找表
                p1 = image_data[(row-1)*lineStep + (col-1)*channels_ + i] & 0xff;// p(x, y)
                p2 = sum_[i][row*width_+col-1]; // p(x-1, y)
                p3 = sum_[i][(row-1)*width_+col]; // p(x, y-1);
                p4 = sum_[i][(row-1)*width_+col-1]; // p(x-1, y-1);
                sum_[i][row*width_+col]= p1+p2+p3-p4;
            
                // 计算平方查找表
                sp2=squaresum_[i][row*width_+col-1]; // p(x-1, y)
                sp3=squaresum_[i][(row-1)*width_+col]; // p(x, y-1);
                sp4=squaresum_[i][(row-1)*width_+col-1]; // p(x-1, y-1);
                squaresum_[i][row*width_+col]=p1*p1+sp2+sp3-sp4;
            }
        }
    }
}

void IntIntegralImage::calculate_sum() {
    // 初始化积分图
    this->width_ = this->image_.cols;
    this->height_ = this->image_.rows;
    this->channels_ = this->image_.channels();
    
    sum_.resize( channels_ ); 
    squaresum_.resize( channels_ );

    std::vector<std::vector<int> >::iterator it = sum_.begin();
    std::vector<std::vector<int> >::iterator itsq = squaresum_.begin();
    for( ; it != sum_.end(), itsq != squaresum_.end(); it++, itsq++ ) {

        it->resize(width_ * height_, 0);
        itsq->resize(width_ * height_, 0);
    }

    uchar* image_data = image_.data;

    // 1 channel and 3 channnels
    int p1 = 0, p2 = 0, p3 = 0, p4 = 0;
    int sp2 = 0, sp3 = 0, sp4 = 0;

    for(int row=1; row<height_; row++ ) {
        for(int col=1; col<width_; col++) {

            // 计算和查找表
            for (int i = 0 ; i < channels_; i++) {
                
                // 计算和查找表
                p1 = image_data[(row-1)*width_+col*channels_+i-1] & 0xff;// p(x, y)
                p2 = sum_[i][row*width_+col-1]; // p(x-1, y)
                p3 = sum_[i][(row-1)*width_+col]; // p(x, y-1);
                p4 = sum_[i][(row-1)*width_+col-1]; // p(x-1, y-1);

                sum_[i][row*width_+col]= p1+p2+p3-p4;
            }
        }
    }
}
