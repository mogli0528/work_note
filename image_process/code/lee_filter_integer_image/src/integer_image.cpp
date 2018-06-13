#include "integer_image.hpp"


int* buff = 0;
int* buff2 = 0;
int boxwidth;
int boxheight;

IntegerImage::IntegerImage(void)    
{    
    f_sum = 0;
    f_sum2 = 0;
}

IntegerImage::~IntegerImage(void)
{
    if(f_sum)    delete[] f_sum;
    if(f_sum2)    delete[] f_sum2;
    if(buff)    delete[] buff;
    if(buff2)    delete[] buff2;
}

void IntegerImage::init(int width, int height, int mwidth, int mheight)
{
    this->mwidth = mwidth;
    this->mheight = mheight;
    this->width = width;
    this->height = height;

    boxwidth = width - mwidth;
    boxheight = height - mheight;
    f_sum = new int[boxwidth *boxheight];
    f_sum2 = new int[boxwidth *boxheight];

    // 2 个中间变量, 保存滑窗中的每列元素之和
    buff = new int[width];
    buff2= new int[width];
}

void IntegerImage::integerImage (unsigned char* img)
{
    int j, x, y;

    memset(buff, 0, width * sizeof(int));
    memset(buff2, 0, width * sizeof(int));
    memset(f_sum, 0, boxwidth * boxheight);
    memset(f_sum2, 0, boxwidth * boxheight);

    // 计算滑窗中每列元素之和, 即滑窗高度覆盖的所有列  
    for( y = 0; y < mheight; y++ )
    {
        for( x = 0; x < width; x++ )
        {
            uchar pixel = img[y * width + x];
            buff[x] += pixel;
            buff2[x] += pixel * pixel;
        }
    }
    
    // 填充积分图   
    // 按行数进行迭代.  
    for( y = 0; y < height - mheight; y++ )
    {
        int Xsum = 0;
        int Xsum2 = 0;

        // 提取第一个滑窗的累积和
        for( j = 0; j < mwidth; j++ )
        {
            Xsum += buff[j];
            Xsum2 += buff2[j];
        }

        // 开始填充积分图: 更新方法是:移除第一个, 加入最后一个;
        for( x = 0; x < width - mwidth; x++ )
        {
            if( x != 0 )
            {
                Xsum -= buff[x-1] + buff[x-1+mwidth]; // 移除第一个, 加入最后一个;
                Xsum2 -= buff2[x-1] + buff2[x-1+mwidth];
            }

            f_sum[y * (width - mwidth) + x] = (float) Xsum ;
            f_sum2[y * (width - mwidth) + x] = Xsum2;            
        }

        // 更新临时变量数组(元素个数等于列数): 移除第一个, 加入最后一个;   
        for( x = 0; x < width; x++ )
        {
            uchar pixel = img[y * width + x];    
            uchar pixel2 = img[(y+mheight) * width + x];    
            // 
            buff[x] = buff[x] - pixel + pixel2;    
            buff2[x] = buff2[x] - pixel * pixel + pixel2 * pixel2;    
        }
    }
}

float IntegerImage::getMean(int x, int y)
{
    // return getSum(x,y) / (float)(mwidth*mheight);
    return getSum(x,y) / (float)(mwidth*mheight);
}

float IntegerImage::getVar(int x, int y)
{
    float mean = getMean(x, y);
    return (float)getSquareSum(x, y)/(mwidth * mheight) - mean*mean;
}

int IntegerImage::getSquareSum(int x, int y)
{
    if(y>mheight/2 && y<height - mheight/2 && x>mwidth/2 && x<width - mwidth/2)
        return f_sum2[(y - mheight/2) *boxwidth + (x - mwidth/2)];
    else
        return -1;
}

int IntegerImage::getSum(int x, int y)
{
    if(y>mheight/2 && y<height - mheight/2 && x>mwidth/2 && x<width - mwidth/2)
        return f_sum[(y - mheight/2) * boxwidth + (x - mwidth/2)];
    else
        return -1;
}

int IntegerImage::getLocalSize()
{
    return mwidth > mheight ? mwidth : mheight;
}
