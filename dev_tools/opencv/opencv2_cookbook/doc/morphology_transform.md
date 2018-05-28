# 基于形态学运算的图像变换

[TOC]

形态学中最基本的元素是结构元素, 结构元素由一个像素集合的结构绘图原点(又称锚点, 一般位于中心位置)定义.  

使用形态学滤波就是对图像的每个像素引用这个结构元素, 当结构元素的原点和给定像素对齐时, 它与图像的相交部分定义了一组进行形态学运算的像素集合.   

形态学滤波定义了一系列的运算, 应用预定义的形状元素来变换一张图像. 形状元素与像素相邻点相交的方式确定了运算的结果.   

原则上说, 形态学滤波使用的结构元素可以是任意形状的, 但通常使用简单的形状. 例如方形, 圆形或菱形.    

> 形态学的惯例: 用高(白色)像素表示前景物体, 用低(黑色)像素表示背景. 因此可能需要对图像取反.    

## 腐蚀, 膨胀运算   
 
腐蚀和膨胀是最基本的形态学运算.   

腐蚀操作替换当前的像素值为像素集合中找到的最小像素值. 膨胀则是相反的运算, 替换当前像素值为像素集合中找到的最大像素值.   

```cpp
void dilate/erode(  InputArray src, 
                    OutputArray dst, 
                    InputArray kernel,   // 结构元素. 若为 cv::Mat(), 则使用 3×3 方形的结构元素
                    Point anchor=Point(-1,-1),          // 锚点.  
                    int iterations=1,                   // 腐蚀的次数
                    int borderType=BORDER_CONSTANT,     // 边界的像素外延法.
                    const Scalar& borderValue=morphologyDefaultBorderValue() 
                 );
```
对于这两个函数, OpenCV 默认使用 3 x 3 的方形结构元素, 原点在中心位置. 因此, 当第 3 个参数 kernel = cv::Mat() 时, 就会使用默认的结构元素.  

也可以指定一个固定大小(一般为奇数)的结构元素, 例如:    
```cpp
cv::Mat element(7, 7, CV_8U, cv::Scalar(1));
cv::erode(image, eroded, element);
```

另一个获得相同结果的方法是对一幅图像反复使用同一个结构元素. 这两个函数都有一个可选的参数用于指定重复迭代的次数:   
```cpp
// 腐蚀图像 3 次
cv::erode(image, eroded, cv::Mat(), cv::Point(-1, -1), 3);
```
事实上, 腐蚀一幅图像两次就像是让结构元素对自己进行膨胀后再去腐蚀同一幅图像, 这点对于膨胀运算也适用.    

## 开闭运算    

用基本的腐蚀, 膨胀运算就可以定义开闭运算:    
- 闭运算定义为: 对图像先膨胀, 再腐蚀;   
- 开运算定义为: 对图像先腐蚀, 再膨胀;   

```cpp
void morphologyEx( InputArray src, 
                   OutputArray dst, 
                   int op,  // 指定运算类型, cv::MORPH_{OPEN, CLOSE, GRADIENT, TOPHAT, BLACKHAT}  
                   InputArray kernel,  
                   Point anchor=Point(-1,-1), 
                   int iterations=1,                // 指定形态学处理的次数;
                   int borderType=BORDER_CONSTANT, 
                   const Scalar& borderValue=morphologyDefaultBorderValue() 
                 );
```
这些滤波器通常在物体检测中使用. 闭运算将误分割成碎片的物体重新连接, 而开运算将移除图像噪点引起的小像素块.  

如果相继进行闭开运算, 获得的图像将只显示场景中的主要物体.

如果想优先处理噪点, 可以先进行开运算, 在执行闭运算, 但是这样有可能去除一些分散的物体.    

需要注意的是, 对一幅图像多次使用相同的开运算(或者闭运算)时是没有效果的. 事实上, 在第一次的开运算填充了图像中的洞后, 再次应用相同的滤波并不会对图像产生任何变换. 用数学的术语将, 这些运算是等幂的.   

## 边缘检测    

形态学滤波也可用于检测图像中指定的特征.   

为了更好的理解形态学运算在灰度图上的效果, 可以将图像的灰度视作海拔高度. 明亮的区域对应的是山岭, 而灰暗的区域则是山谷, 那么边缘就意味着明亮与灰暗像素的快速过渡, 可以视作陡峭的悬崖. 如果腐蚀运算作用于这个地形, 即用相邻范围内的最小值来替换当前像素值, 于是降低了它的高度. 即悬崖被"腐蚀", 山谷被延伸. 膨胀的结果则恰恰相反, 山谷之间将竖起很多悬崖.  

cv::morphologyEx() 在参数为 cv::MORPH_GRADIENT 可以检测图像的边缘, 其作用原理就是计算膨胀后的图像和腐蚀后图像的插值. 

## 使用分水岭算法对图像进行分割   

分水岭变换用于快速分割图像为同类区域. 其原理是基于: 如果将图像视为拓扑结构的地图, 那么均质区域对应的是被陡峭边缘包围的平坦盆地.   

```cpp
void watershed(InputArray image, InputOutputArray markers);

void findContours( InputOutputArray image, 
                   OutputArrayOfArrays contours, 
                   OutputArray hierarchy, 
                   int mode, 
                   int method, 
                   Point offset=Point()
                 );
```
函数的输入是一个 32 位的有符号整数(CV_32S)标记图, 每个非零的像素表示一个标记.  

将图像中已知属于某个区域的像素进行标记. 基于这个初始标记, 分水岭算法开始确定其他像素的归属区域.  
```cpp
void setMarkers(const cv::Mat &markerImage) 
{
    markerImage.convertTo(markers, CV_32S);
}
```

可以通过预处理过程识别出目标物体的像素, 通过大幅度地腐蚀该图像可以获取仅仅属于重要物体的像素. 同时通过大幅度膨胀获取属于背景的像素, 然后组合将其作为标记.   

**重视代码实现**.   

## 使用 GrabCut 算法提取前景物体    

GrabCut 算法并非基于形态学, 但它与分水岭算法很相似. GrabCut 在计算式更复杂, 但是产生的结果要精确许多.    

对于需要从静态图像中提取前景物体的应用, GrabCut 是最佳算法.   

```cpp
void grabCut( InputArray img, 
              InputOutputArray mask, 
              Rect rect,                    // 包含前景物体的矩形框
              InputOutputArray bgdModel,    // 计算得到的被景模型 
              InputOutputArray fgdModel,    // 计算得到的前景模型
              int iterCount,                // 指定迭代次数
              int mode=GC_EVAL 
            );
```  

```
mask – Input/output 8-bit single-channel mask. The mask is initialized by the function when mode is set to GC_INIT_WITH_RECT. Its elements may have one of following values:
    GC_BGD defines an obvious background pixels.
    GC_FGD defines an obvious foreground (object) pixel.
    GC_PR_BGD defines a possible background pixel.
    GC_PR_FGD defines a possible foreground pixel.
rect – ROI containing a segmented object. The pixels outside of the ROI are marked as “obvious background”. The parameter is only used when mode==GC_INIT_WITH_RECT .
bgdModel – Temporary array for the background model. Do not modify it while you are processing the same image.
fgdModel – Temporary arrays for the foreground model. Do not modify it while you are processing the same image.
iterCount – Number of iterations the algorithm should make before returning the result. Note that the result can be refined with further calls with mode==GC_INIT_WITH_MASK or mode==GC_EVAL .
mode –
Operation mode that could be one of the following:

GC_INIT_WITH_RECT The function initializes the state and the mask using the provided rectangle. After that it runs iterCount iterations of the algorithm.
GC_INIT_WITH_MASK The function initializes the state using the provided mask. Note that GC_INIT_WITH_RECT and GC_INIT_WITH_MASK can be combined. Then, all the pixels outside of the ROI are automatically initialized with GC_BGD .
GC_EVAL The value means that the algorithm should just resume.
```
使用 cv::grabCut() 函数需要提供图像并标记出背景像素和前景像素. 一种指定局部标记的方法是定义一个矩形, 前景物体被它包围.   


## `flip`翻转      
```cpp
void flip(InputArray src, OutputArray dst, int flipCode)  

参数：    
src: 输入矩阵    
dst: 翻转后矩阵，类型与src一致    
flipCode:    
翻转模式
flipCode==0 垂直翻转（沿X轴翻转）    
flipCode>0  水平翻转（沿Y轴翻转）    
flipCode<0  水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）    
```
