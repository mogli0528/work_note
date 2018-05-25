# 直方图统计    

[TOC]

## 0. 概述

一个图像由不同颜色值的像素组成. 像素值在图像中的分布情况就是这幅图像的一个重要特征.   

- 使用直方图修改图像外观.   
- 检测图像中特定的对象或纹理.    

## 1. 计算图像的直方图   

使用 cv::calcHist() 计算直方图, 这是一个通用函数, 可以计算任意像素类型的多通道图像.   

```cpp
void calcHist(  const Mat* images, 
                int nimages,            // 计算直方图的图像张数
                const int* channels,    // 图像的通道数量
                InputArray mask,        // 掩模
                OutputArray hist,       // 返回的直方图, 还有个 SparseMat& hist,
                int dims,               // 直方图维度
                const int* histSize,    // 直方图中项的个数
                const float** ranges,   // 像素值的范围
                bool uniform=true,      // 归一化
                bool accumulate=false   // 累积多个直方图计算的结果
            );
```
返回的直方图存储在一个 cv::MatND 类型的实例(直方图中的值为浮点数)中, 这是一个通用类, 可以操作 N 维矩阵.   

对于 3 通道图像的之直方图计算, 返回的是三维的 cv::Mat 实例.    

在许多应用程序中, 在计算一个大型直方图前最好能够减小颜色的数量(即缩小表示颜色的范围).      

或者可以考虑使用下面的稀疏矩阵数据结构, 稀疏矩阵表示矩阵拥有少量的非零元素.   

```cpp
void calcHist(  const Mat* images, 
                int nimages,            // 计算直方图的图像张数
                const int* channels,    // 图像的通道数量
                InputArray mask,        // 掩模
                SparseMat hist,       // 返回的直方图, 还有个 SparseMat& hist,
                int dims,               // 直方图维度
                const int* histSize,    // 直方图中项的个数
                const float** ranges,   // 像素值的范围
                bool uniform=true,      // 归一化
                bool accumulate=false   // 累积多个直方图计算的结果
            );


cv::SparseMat hist(3, histSize, CV_32F);
cv::calcHist(...);
return hist;
```

## 2. 使用查找表修改图像外观    

查找表是一个简单的一对一(或多对一)的函数, 定义了如何将像素值转换为新的值.   

查找表本质上是一个一维数组, 对于常规绘图图像而言有 256 个条目, 表的第 i 项表示相应灰度的新值.   

使用 cv::LUT 对图像应用查找表以生成新的图像.   

注意 lut 查找表函数的定义方式.    
```cpp

...

// lookup 反转图像像素强度值
int dim[1] = {256};
cv::Mat lut(1, dim, CV_8U);
for(int i = 0; i < 256; i++){
    lut.at<uchar>(i) = 255 - i;
}

cv::Mat lutRet = h.applyLookUp(image, lut);
```

## 3. 直方图均衡化   

通过拉伸直方图, 使它覆盖所有的取值范围. 这个策略确实可以简单有效地提升图像质量.  然而, 大多数情况下, 图像在视觉的缺陷并非源于使用过窄的强度范围, 而是由于某些颜色值的出现频率高于另一些.   

事实上, 我们可以认为一幅高质量的图像应该平均使用所有的像素强度. 即使得图像的直方图尽可能的平坦.   

使用 cv::equalizeHist() 可以获取直方图均衡化后的图像.   
```cpp
void equalizeHist(InputArray src, OutputArray dst);
```  

`一般而言`, 直方图均衡化能大幅度改善图像的外观. 当然最终的结果质量是和图像本身有关的, 可能好也可能坏.   

## 4. 直方图反投影以检测特定的图像内容    

如果一幅图像的区域中显示的是一种独特的纹理或是一个独特的物体, 那么这个区域的直方图可以看作是一个概率函数, 它给出的是某个像素属于该纹理或该物体的概率.   

使用 cv::calBackProject() 函数可以替换一个输入图像中的每个像素值使其变成归一化直方图中对应的概率值. 因此, 使用 calcHist() 得到的直方图还需要经过 cv::normalize() 函数进行归一化之后才能被 cv::calBackProject() 函数使用.   

使用该方法对灰度图的内容进行检测时, 结果也许会令人失望. 因为概率函数是从一张简单的灰度直方图中提取的, 图像中不可避免的会有许多其他像素与云层像素的强度是相同的, 这样在反投影时相同强度的像素就被替换成相同的概率值.    

提高检测质量的一个方法是使用颜色信息, 即采用三通道直方图.   

```cpp
void calcBackProject(   const Mat* images,         // 需要反投影的图像张数
                        int nimages,               // 图片的数量
                        const int* channels,       // 通道个数
                        InputArray hist,           // 对应于 calcHist, 也可以是 const SparseMat& hist, 
                        OutputArray backProject,   // 生成的反投影图像
                        const float** ranges,      // 每个维度像素值的范围
                        double scale=1,            // 缩放银子
                        bool uniform=true 
                    );
```

使用多张图计算直方图可以提高泛化性吗? 因为多张图使得统计的样本数量增加, 符合正态分布.   

## 5. 使用均值漂移查找物体 - mean-shift.    

直方图反投影的结果是一个概率映射, 体现了已知的图像内容出现在图像中特定位置的概率. 

假设我们现在知道物体的近似位置, 概率映射可用于找到对象的确切位置. 最有可能的位置是得到和已知窗口区域像素最大概率的位置.   

因此, 如果我们从最初的位置开始, 并且迭代移动, 便可以找到精确的位置. 这边是均值漂移算法的核心思想.   

```cpp
int meanShift(InputArray probImage, Rect& window, TermCriteria criteria);
```

The function implements the iterative object search algorithm. It takes the input back projection of an object and the initial position. The mass center in window of the back projection image is computed and the search window center shifts to the mass center. The procedure is repeated until the specified number of iterations criteria.maxCount is done or until the window center shifts by less than criteria.epsilon . The algorithm is used inside CamShift() and, unlike CamShift() , the search window size or orientation do not change during the search. 

You can simply pass the output of calcBackProject() to this function. But better results can be obtained if you pre-filter the back projection and remove the noise. For example, you can do this by retrieving connected components with findContours() , throwing away contours with small area ( contourArea() ), and rendering the remaining contours with drawContours() .

- **Note**:
```
A mean-shift tracking sample can be found at opencv_source_code/samples/cpp/camshiftdemo.cpp   
```

## 6. 通过比较直方图检索相似图片   

直方图比较是通过简单的比较两个直方图来测量两个图像的相似性, 关键点在于如何定义一个评估两个直方图多么不同或者多么相似的测量函数.   

cv::compareHist() 函数实现了两个直方图的比较.   

```cpp
double compareHist(InputArray H1, InputArray H2, int method)
double compareHist(const SparseMat& H1, const SparseMat& H2, int method)
```
OpenCV 中实现的比较方法:   

- CV_COMP_CORREL 归一化互相关    
- CV_COMP_CHISQR 卡方测量: 对容器间插值的归一化平方和   
- CV_COMP_CHISQR_ALT Alternative Chi-Square   
- CV_COMP_INTERSECT 交叉测量法   
- CV_COMP_BHATTACHARYYA 巴氏测量:评估两个概率分布间相似性   
- CV_COMP_HELLINGER 和巴氏测量类似  
- CV_COMP_KL_DIV Kullback-Leibler divergence  

大多数直方图比较是逐个容器进行比较, 并不考虑相邻容器的值. 因此, 测量相似度之前减少颜色空间是很重要的, 其他颜色空间也可以这么使用.   

