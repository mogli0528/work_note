# 提取直线, 轮廓及连通区域   

>　为了对图像进行基于内容的分析, 需要从像素中提取有意义的特征(例如:轮廓, 直线等).　 

[TOC]

## 使用 Canny 算子检测轮廓    

简单的二值边缘图像有两个缺陷:  

- 检测到的边缘过粗, 难以实现物体的准确定位;    
- 难以找到一个阈值, 技能足够低以检测到所有重要边缘, 同时又不至于包含过多次要的边缘.   

Canny() 算法可以就上面两个缺陷提供一种解决方法.  

```cpp
void 
Canny( InputArray image, 
       OutputArray edges,    // 检测到的边缘输出矩阵, 类型为 CV_8UC1
       double threshold1,    // 连接 edges 的小阈值
       double threshold2,    // 确定 strong 边缘的阈值
       int apertureSize=3,   // Sobel() 算子的大小
       bool L2gradient=false // 是否使用 L_2 norm 计算梯度的模. 默认用 L_1 norm .  
     );
```

Canny 算子通常基于 Sobel 算子, 核心思想是使用两个不同阈值以确定哪些点属于轮廓: 一个低值和一个高值. 使用低阈值检测到的边缘比较多, 使用高阈值检测到的都是重要边缘, 然后 Canny 算法"组合"这两幅边缘图: 如果存在连续的边缘路径, 将低阈值图像中的边缘点与高阈值图像中的边相连接. 那么就保留这个低阈值图像中的边缘点. 因此, 高阈值图像中的所有边缘点都保留下来, 同时移除低阈值图像中所有孤立的点.   

只要指定合适的阈值, Canny 就可以生成高质量的轮廓.   

Canny 算法在使用磁滞化阈值前, 所有在梯度大小并非最大值的边缘点都被移除. 梯度的方向总是与边缘垂直, 因此该方向的局部梯度最大值对应的就是轮廓强度最大的点.     
   
## 使用霍夫变换检测直线    

霍夫变换(Hough Transform) 是检测直线的经典算法. 经过推广, 霍夫变换可以在图像中寻找直线, 圆以及其他简单集合形状的方法. 
 
霍夫直线变换中, 直线用以下方程表示:     

$$ \rho = x cos \theta + y sin\theta $$  
参数 $\rho$ 是直线到图像原点的距离, $\theta$ 是

OpenCV 支持两种不同形式的霍夫变换: 标准霍夫变换(SHT) 和累计概率霍夫变换(PPHT). 对应的函数分别为 cv::HoughLines() 和 cv2::HoughLinesP().   

```cpp
void HoughLines( InputArray image, 
                 OutputArray lines, // 输出是 cv::Vec2f 向量, 每个元素是表示直线的(rho, theta)  
                 double rho,        // 用于搜索直线的累加器的距离分辨率, 单位是像素.
                 double theta,      // 用于搜索直线的累加器的角度分辨率, 单位是弧度.
                 int threshold,     // 用于搜索直线的累加器的投票阈值
                 double srn=0, 
                 double stn=0, 
                 double min_theta=0, 
                 double max_theta=CV_PI 
               );
```
cv::HoughLines() 函数检测到的是直线, 而不是线段, 因此绘制的直线会穿过整个图像.   

霍夫线变换的仅仅查找边缘点的一种排列方式, 由于意外的像素排列或是多条线穿过一组像素, 很有可能带来检测错误.   

概率霍夫变换解决了上述问题, 其对应的函数是 cv2::HoughLinesP(), 它可以检测到带端点的直线.   

```cpp
void HoughLinesP( InputArray image, 
                  OutputArray lines, // 输出是 cv::Vec4i, 包含线段的起点和终点坐标.  
                  double rho,        // 用于搜索直线的累加器的距离分辨率, 单位是像素.
                  double theta,      // 用于搜索直线的累加器的角度分辨率, 单位是弧度.
                  int threshold,     // 用于搜索直线的累加器的投票阈值
                  double minLineLength=0,    // 检测直线的最小长度
                  double maxLineGap=0        // 允许组成连续线段的最大像素间隔
                );
```

**霍夫圆变换**  

霍夫圆变换和霍夫直线变化大体上是类似的.因为圆的参数化需要三维参数(x, y, r), 这会需要大量的内存且运算速度很慢.  

OpenCV 中通过一个比较灵活的霍夫梯度法来解决圆变换这一问题.  

霍夫圆变换的一些缺点:  

Sobel 算子计算局部梯度的精度问题可能会引入噪声;   
在边缘图像中的整个非 0 像素集被认为是每个中心的候选. 因此, 如果把累加器阈值设置的偏低, 算法就会消耗很长时间;  
每个中心只选择一个圆, 如果有同心圆, 则只能选择一个;  

## 用直线拟合一组点    

在一些应用中, 不仅需要检测直线, 还要获取直线位置和方向的精确估计.   

直线拟合是数学中的经典问题. OpenCV 通过最小化每个点到直线的距离之和进行求解.   

距离函数的选择也在一定程度上会影响计算效率和结果精度. 最快速的计算距离方法是欧式距离, 即 CV_DIST_L2, 即标准的最小二乘法. 

极小化过程使用 M 估计, 每一次迭代完成后会根据点到直线的距离 $\rho(r_i)$ 的反比更新权重 $w_i$, 迭代地求解加权最小二乘问题. 当不属于这条直线的异常值可能被点集包含时, 可以选择给予较远点较小的权的距离函数.    

```cpp
void fitLine( InputArray points,// 点集, vector 或 Mat
              OutputArray line, // 输出拟合后的直线
              int distType,     //  M-estimator 使用的距离计算方法
              double param,     // 某些距离公式的数值参数. 置 0 会选择一个最优参数.
              double reps,      // 距离(半径)的精度
              double aeps       // 角度的精度, 这两个值一般为 0.01
            );
```
points – Input vector of 2D or 3D points, stored in std::vector<> or Mat.
line – 输出参数.     
对应于二维点的拟合, 输出是 cv::Vec4f - (vx, vy, x0, y0), 其中 (vx, vy) 是直线的单位方向向量, (x0, y0) 是直线上的一点.   
对应于三维点的拟合, 输出是 cv::Vec6f - (vx, vy, vz, x0, y0, z0), 其中 (vx, vy, vz) 是直线的单位方向向量, (x0, y0, z0) 是直线上的一点.   

通过计算 $\sum_i \rho(r_i)$ 的最小值. 其中, $r_i$ 是第 i 个点( $i^{th}$ )和直线的距离, $\rho(r)$ 是距离函数, 可取的类型为:

distTyp:    
- CV_DIST_L2   
$$\rho (r) = \frac{r^2}{2}  \quad \text{(最小二乘法, 简单快速)}$$
- CV_DIST_L1   
$$\rho (r) = r$$
- CV_DIST_L12  
$$\rho (r) = 2  \cdot ( \sqrt{1 + \frac{r^2}{2}} - 1)$$
- CV_DIST_FAIR   
$$ \rho \left (r \right ) = C^2  \cdot \left (  \frac{r}{C} -  \log{\left(1 + \frac{r}{C}\right)} \right )  \quad \text{其中,} \quad C=1.3998$$ 
- CV_DIST_WELSCH  
$$ \rho \left (r \right ) =  \frac{C^2}{2} \cdot \left ( 1 -  \exp{\left(-\left(\frac{r}{C}\right)^2\right)} \right )  \quad \text{where} \quad C=2.9846$$ 
- CV_DIST_HUBER   
$$\rho (r)=\begin{cases}
r^2/2 & {r < C} \\
C \cdot (r-C/2) & otherwise 
\end{cases}$$

该函数可以拟合三维的点集, 输入是一组 cv::Point3i 或 cv::Point3f 点, 输出是一个 cv::Vec6f.   

cv::fitEllipse() 函数可以使用一组二维点拟合一个椭圆.   
```cpp
RotatedRect fitEllipse(InputArray points)
```
##　提取连通区域的轮廓　　　

OpenCV 提供了一个简单的函数用于提取连通区域的轮廓.   

```cpp
void findContours( InputOutputArray image, 
                   OutputArrayOfArrays contours,     // 轮廓数组
                   int mode,                         // 指定要获取轮廓的类型
                   int method,                       // 轮廓近似方法
                   Point offset=Point()
                 );
// 轮廓绘制函数   
void drawContours( InputOutputArray image, 
                   InputArrayOfArrays contours,      // 轮廓
                   int contourIdx,                   // 轮廓的 idx, 如果为 -1, 全部绘制
                   const Scalar& color,              // 轮廓线的颜色
                   int thickness=1,                  // 轮廓线的宽度
                   int lineType=LINE_8, 
                   InputArray hierarchy=noArray(), 
                   int maxLevel=INT_MAX, 
                   Point offset=Point() 
                 );

void findContours( InputOutputArray image, 
                   OutputArrayOfArrays contours, 
                   OutputArray hierarchy, 
                   int mode, 
                   int method, 
                   Point offset=Point()
                 );
```   
CV_RETR_EXTERNAL retrieves only the extreme outer contours. It sets hierarchy[i][2]=hierarchy[i][3]=-1 for all the contours.
CV_RETR_LIST retrieves all of the contours without establishing any hierarchical relationships.
CV_RETR_CCOMP retrieves all of the contours and organizes them into a two-level hierarchy. At the top level, there are external boundaries of the components. At the second level, there are boundaries of the holes. If there is another contour inside a hole of a connected component, it is still put at the top level.
CV_RETR_TREE retrieves all of the contours and reconstructs a full hierarchy of nested contours. This full hierarchy is built 

在实际的使用中, 可以使用极大值极小值来限定轮廓的周长, 通过遍历所有轮廓来去除一些无效的轮廓.   

## 计算连通区域的形状描述符　　　

连通区域通常对应于场景中的某个物体, 为了识别这个物体, 或者将它与其他图形元素比较, 需要进行一些测量以获取它的特征.  

外接矩形(也称包围盒)被定义为正好包含形状的最小矩形, 比较盒子的高度和宽度能够给出物体的垂直或水平方向(例如可以区分汽车和行人).   
```cpp
Rect boundingRect(InputArray points)  // cv::Rect boundingRect(cv::Mat(contours[0]));
```

多边形近似在需要操作与形状类似的更紧密的表现时很用用, 因此需要指定精确值参数以确定形状与它的简化多边形之间的最大可接受距离.
```cpp
void approxPolyDP( InputArray curve,        // std::vector<cv::Point> or Mat 类型的 2D 点集
                   OutputArray approxCurve, // 近似多边形的顶点集合, std::vector<cv::Point> 类型
                   double epsilon,          // 近似的精度(原始轮廓和近似多边形的最大距离), 可选取 5
                   bool closed              // 首尾相连, 即同一个点, 一般设置为 true
                 );
```
函数返回的是多边形的顶点, 为了绘制多边形, 需要遍历向量将相邻的点连接以绘制它们之间的直线.   

最小包围圆通常用于仅需要尺寸与位置的情况.    
```cpp
void minEnclosingCircle(InputArray points, Point2f& center, float& radius)
```
函数返回最小包围圆的中心和半径.   

凸包是另一种多边形近似, 是包围形状的最小凸多边, 它可以被可视化为围绕连通区域的橡皮筋形成的形状.     
```cpp
void convexHull( InputArray points,       // std::vector<cv::Point> or Mat 类型的 2D 点集
                 OutputArray hull,        // 近似多边形的顶点集合, std::vector<cv::Point> 类型
                 bool clockwise=false, 
                 bool returnPoints=true 
               );
```


力矩通常用于以数学方式进行形状的结构化分析.   
```cpp
Moments moments(InputArray array, bool binaryImage=false );

class CV_EXPORTS_W_MAP Moments
{
public:
    //! the default constructor
    Moments();
    //! the full constructor
    Moments(double m00, double m10, double m01, double m20, double m11,
            double m02, double m30, double m21, double m12, double m03 );
    //! the conversion from CvMoments
    Moments( const CvMoments& moments );
    //! the conversion to CvMoments
    operator CvMoments() const;

    //! spatial moments
    CV_PROP_RW double  m00, m10, m01, m20, m11, m02, m30, m21, m12, m03;
    //! central moments
    CV_PROP_RW double  mu20, mu11, mu02, mu30, mu21, mu12, mu03;
    //! central normalized moments
    CV_PROP_RW double  nu20, nu11, nu02, nu30, nu21, nu12, nu03;
};
```
返回值为 cv::Moments, 封装了计算出的力矩.   

$(\bar{x}, \bar{y})$ 是质心:    

$$\bar{x} = \frac{\texttt{m}_{10}}{\texttt{m}_{00}} , \; \bar{y} = \frac{\texttt{m}_{01}}{\texttt{m}_{00}}$$
   
## Others

cv::minAreaRect() 计算最小包围旋转矩形.    
cv::contourArea() 计算轮廓的面积, 即包含像素的个数.    
cv::pointPolygonTest() 确定一个点是否位于轮廓内.   
cv::matchShapes() 测量两个轮廓的相似度.    

