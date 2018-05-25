# 基于形态学运算的图像变换

[TOC]

形态学滤波定义了一系列的运算, 应用预定义的形状元素来变换一张图像. 形状元素与像素相邻点相交的方式确定了运算的结果.   

>由于形态学滤波通常使用于二值图像。由于惯例是白色像素表示前景，黑色像素表示背景。因此还需要对图像取反。   

## 腐蚀, 膨胀运算   






1. `flip`翻转      
```cpp
void flip(InputArray src, OutputArray dst, int flipCode)  

参数：    
`src`，输入矩阵    
`dst`，翻转后矩阵，类型与src一致    
`flipCode`，翻转模式，flipCode==0垂直翻转（沿X轴翻转），flipCode>0水平翻转（沿Y轴翻转），flipCode<0水平垂直翻转（先沿X轴翻转，再沿Y轴翻转，等价于旋转180°）    
```
2. `erode`腐蚀   
```cpp
void erode( InputArray src, OutputArray dst, 
	InputArray kernel, 
	Point anchor=Point(-1,-1), 
	int iterations=1, 
	int borderType=BORDER_CONSTANT, 
	const Scalar& borderValue=morphologyDefaultBorderValue() )

src – 输入图像.
dst – 输出图像.
kernel – 用于腐蚀的结构元素。若kernel=Mat(), 则使用 3×3 长方形的结构元素
anchor – 锚点。默认值(-1, -1)表示锚点在kernel的中心.
iterations – 腐蚀的次数
borderType – 边界的像素外延法.
borderValue – BORDER_CONSTANT方法下使用的边界值.

```


