## cv2.accumulate  
```
Python: cv2.accumulate(src, dst[, mask]) → dst
C: void cvAcc(const CvArr* image, CvArr* sum, const CvArr* mask=NULL )
Parameters:	
src – Input image as 1- or 3-channel, 8-bit or 32-bit floating point.
dst – Accumulator image with the same number of channels as input image, 32-bit or 64-bit floating-point.
mask – Optional operation mask.
The function adds src or some of its elements to dst :

\texttt{dst} (x,y)  \leftarrow \texttt{dst} (x,y) +  \texttt{src} (x,y)  \quad \text{if} \quad \texttt{mask} (x,y)  \ne 0

The function supports multi-channel images. Each channel is processed independently.

The functions accumulate* can be used, for example, to collect statistics of a scene background viewed by a still camera and for the further foreground-background segmentation.

See also accumulateSquare(), accumulateProduct(), accumulateWeighted()
```

## cv2.add    

Calculates the per-element sum of two arrays or an array and a scalar.   
```
Python: cv2.add(src1, src2[, dst[, mask[, dtype]]]) → dst
C++: void add(InputArray src1, InputArray src2, OutputArray dst, InputArray mask=noArray(), int dtype=-1)
C: void cvAdd(const CvArr* src1, const CvArr* src2, CvArr* dst, const CvArr* mask=NULL)
C: void cvAddS(const CvArr* src, CvScalar value, CvArr* dst, const CvArr* mask=NULL)
Parameters:	
src1 – first input array or a scalar.
src2 – second input array or a scalar.
src – single input array.
value – scalar value.
dst – output array that has the same size and number of channels as the input array(s); the depth is defined by dtype or src1/src2.
mask – optional operation mask - 8-bit single channel array, that specifies elements of the output array to be changed.
dtype – optional depth of the output array (see the discussion below).
The function add calculates:

Sum of two arrays when both input arrays have the same size and the same number of channels:

\texttt{dst}(I) =  \texttt{saturate} ( \texttt{src1}(I) +  \texttt{src2}(I)) \quad \texttt{if mask}(I) \ne0

Sum of an array and a scalar when src2 is constructed from Scalar or has the same number of elements as src1.channels():

\texttt{dst}(I) =  \texttt{saturate} ( \texttt{src1}(I) +  \texttt{src2} ) \quad \texttt{if mask}(I) \ne0

Sum of a scalar and an array when src1 is constructed from Scalar or has the same number of elements as src2.channels():

\texttt{dst}(I) =  \texttt{saturate} ( \texttt{src1} +  \texttt{src2}(I) ) \quad \texttt{if mask}(I) \ne0

where I is a multi-dimensional index of array elements. In case of multi-channel arrays, each channel is processed independently.

The first function in the list above can be replaced with matrix expressions:

dst = src1 + src2;
dst += src1; // equivalent to add(dst, src1, dst);
The input arrays and the output array can all have the same or different depths. For example, you can add a 16-bit unsigned array to a 8-bit signed array and store the sum as a 32-bit floating-point array. Depth of the output array is determined by the dtype parameter. In the second and third cases above, as well as in the first case, when src1.depth() == src2.depth(), dtype can be set to the default -1. In this case, the output array will have the same depth as the input array, be it src1, src2 or both.

Note Saturation is not applied when the output array has the depth CV_32S. You may even get result of an incorrect sign in the case of overflow.
See also subtract(), addWeighted(), scaleAdd(), Mat::convertTo(), Matrix Expressions
```

## cv2.addWeighted   

Calculates the weighted sum of two arrays.    
```
Python: cv2.addWeighted(src1, alpha, src2, beta, gamma[, dst[, dtype]]) → dst
C++: void addWeighted(InputArray src1, double alpha, InputArray src2, double beta, double gamma, OutputArray dst, int dtype=-1)
C: void cvAddWeighted(const CvArr* src1, double alpha, const CvArr* src2, double beta, double gamma, CvArr* dst)
Parameters:	
src1 – first input array.
alpha – weight of the first array elements.
src2 – second input array of the same size and channel number as src1.
beta – weight of the second array elements.
dst – output array that has the same size and number of channels as the input arrays.
gamma – scalar added to each sum.
dtype – optional depth of the output array; when both input arrays have the same depth, dtype can be set to -1, which will be equivalent to src1.depth().
The function addWeighted calculates the weighted sum of two arrays as follows:

\texttt{dst} (I)= \texttt{saturate} ( \texttt{src1} (I)* \texttt{alpha} +  \texttt{src2} (I)* \texttt{beta} +  \texttt{gamma} )

where I is a multi-dimensional index of array elements. In case of multi-channel arrays, each channel is processed independently.

The function can be replaced with a matrix expression:

dst = src1*alpha + src2*beta + gamma;
Note Saturation is not applied when the output array has the depth CV_32S. You may even get result of an incorrect sign in the case of overflow.
See also add(), subtract(), scaleAdd(), Mat::convertTo(), Matrix Expressions
```
## 

Python: cv2.filter2D(src, ddepth, kernel[, dst[, anchor[, delta[, borderType]]]]) → dst
C: void cvFilter2D(const CvArr* src, CvArr* dst, const CvMat* kernel, CvPoint anchor=cvPoint(-1,-1) )
Parameters:	
src – input image.
dst – output image of the same size and the same number of channels as src.
ddepth –
desired depth of the destination image; if it is negative, it will be the same as src.depth(); the following combinations of src.depth() and ddepth are supported:
src.depth() = CV_8U, ddepth = -1/CV_16S/CV_32F/CV_64F
src.depth() = CV_16U/CV_16S, ddepth = -1/CV_32F/CV_64F
src.depth() = CV_32F, ddepth = -1/CV_32F/CV_64F
src.depth() = CV_64F, ddepth = -1/CV_64F
when ddepth=-1, the output image will have the same depth as the source.

kernel – convolution kernel (or rather a correlation kernel), a single-channel floating point matrix; if you want to apply different kernels to different channels, split the image into separate color planes using split() and process them individually.
anchor – anchor of the kernel that indicates the relative position of a filtered point within the kernel; the anchor should lie within the kernel; default value (-1,-1) means that the anchor is at the kernel center.
delta – optional value added to the filtered pixels before storing them in dst.
borderType – pixel extrapolation method (see borderInterpolate for details).
The function applies an arbitrary linear filter to an image. In-place operation is supported. When the aperture is partially outside the image, the function interpolates outlier pixel values according to the specified border mode.

The function does actually compute correlation, not the convolution:

\texttt{dst} (x,y) =  \sum _{ \stackrel{0\leq x' < \texttt{kernel.cols},}{0\leq y' < \texttt{kernel.rows}} }  \texttt{kernel} (x',y')* \texttt{src} (x+x'- \texttt{anchor.x} ,y+y'- \texttt{anchor.y} )

That is, the kernel is not mirrored around the anchor point. If you need a real convolution, flip the kernel using flip() and set the new anchor to (kernel.cols - anchor.x - 1, kernel.rows - anchor.y - 1) .

The function uses the DFT-based algorithm in case of sufficiently large kernels (~``11 x 11`` or larger) and the direct algorithm for small kernels.

## cv2.HoughLines() 

