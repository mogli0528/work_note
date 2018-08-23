# 深度学习通用部分   

数据集, 深度学习概念等.  

## code  

image_augmentation: 几种常用的图像增广方法.  

## 图像分割  

### 1. cvlsegmentation

团队主页:  

http://www.vision.ee.ethz.ch/~cvlsegmentation/

1) OSVOS: One-Shot Video Object Segmentation   

https://github.com/scaelles/OSVOS-TensorFlow   

2) Frequency-tuned Salient Region Detection

Radhakrishna Achanta, Sheila Hemami, Francisco Estrada, and Sabine Susstrunk

http://ivrlwww.epfl.ch/supplementary_material/RK_CVPR09/  

### 2. 程明明 南开大学实验室   

团队主页  

https://mmcheng.net/  

1) 项目代码和论文  

https://mmcheng.net/code-data/   

### 3. 

1) ImageSegmentation   

https://github.com/congve1/ImageSegmentation  

Image segmentation first get the saliency map and set a threshold to segment the image

### 4. 语义分割+视频分割开源代码集合  

来源：CSDN博客（http://blog.csdn.net/zhangjunhit/article/details/78190283）

1) 语义分割 

Global Deconvolutional Networks [BMVC 2016]    
https://github.com/DrSleep/GDN  


The One Hundred Layers Tiramisu: Fully Convolutional DenseNets for Semantic Segmentation CVPRW 2017 
Theano/Lasagne code Implementation   

https://github.com/0bserver07/One-Hundred-Layers-Tiramisu  



Full-Resolution Residual Networks for Semantic Segmentation in Street Scenes CVPR2017 
https://github.com/TobyPDE/FRRN  



Awesome Semantic Segmentation   
https://github.com/mrgloom/awesome-semantic-segmentation  



Semantic Segmentation Algorithms Implemented in PyTorch 这个很好！   
https://github.com/meetshah1995/pytorch-semseg  



Learning Deconvolution Network for Semantic Segmentation   
https://github.com/HyeonwooNoh/DeconvNet  



Fully Convolutional Instance-aware Semantic Segmentation   
https://github.com/daijifeng001/TA-FCN  



Fully Convolutional Networks for Semantic Segmentation   
https://github.com/shelhamer/fcn.berkeleyvision.org  


PixelNet: Representation of the pixels, by the pixels, and for the pixels   
https://github.com/aayushbansal/PixelNet   
http://www.cs.cmu.edu/~aayushb/pixelNet/  



ICNet for Real-Time Semantic Segmentation on High-Resolution Images   
https://hszhao.github.io/projects/icnet/ https://github.com/hszhao/ICNet  



SegNet: A Deep Convolutional Encoder-Decoder Architecture for Semantic Pixel-Wise Labelling 
https://arxiv.org/pdf/1511.00561.pdf   
PAMI-2017 https://github.com/alexgkendall/caffe-segnet  



DeepLab: Semantic Image Segmentation with Deep Convolutional Nets, Atrous Convolution, and Fully Connected CRFs   
https://bitbucket.org/aquariusjay/deeplab-public-ver2/overview  



DeconvNet : Learning Deconvolution Network for Semantic Segmentation ICCV2015   
https://github.com/HyeonwooNoh/DeconvNet http://cvlab.postech.ac.kr/research/deconvnet/  



Pyramid Scene Parsing Network CVPR2017   
https://github.com/hszhao/PSPNet  



Fully Convolutional Instance-aware Semantic Segmentation CVPR2017   
https://github.com/msracver/FCIS  



ParseNet: Looking Wider to See Better   
https://github.com/weiliu89/caffe/tree/fcn  



其他 
半监督语义分割 

Mix-and-Match Tuning for Self-Supervised Semantic Segmentation   
AAAI Conference on Artificial Intelligence (AAAI) 2018   
http://mmlab.ie.cuhk.edu.hk/projects/M&M/   
https://github.com/XiaohangZhan/mix-and-match/  

deconvnet analysis   
Salient Deconvolutional Networks ECCV2016   
https://github.com/aravindhm/deconvnet_analysis  



Instance Segmentation 分水岭+CNN   
Deep Watershed Transform for Instance Segmentation CVPR2017   
https://github.com/min2209/dwt  



Instance Segmentation   
End-to-End Instance Segmentation with Recurrent Attention CVPR2017   
https://github.com/renmengye/rec-attend-public  



基于单张训练样本的视频运动物体分割   
Video Object Segmentation Without Temporal Information One-Shot Video Object Segmentation   
http://www.vision.ee.ethz.ch/~cvlsegmentation/osvos/  



图像语义匹配   

SCNet: Learning Semantic Correspondence ICCV2017   
Matlab code: https://github.com/k-han/SCNet  



同时检测和分割，类似 Mask R-CNN 
BlitzNet: A Real-Time Deep Network for Scene Understanding   
ICCV2017 

https://github.com/dvornikita/blitznet   



目标候选区域分割   

FastMask: Segment Multi-scale Object Candidates in One Shot CVPR2017   
https://github.com/voidrank/FastMask  



视频分割  

Learning to Segment Instances in Videos with Spatial Propagation Network CVPRW2017   
https://github.com/JingchunCheng/Seg-with-SPN  




### 6. 王井东  

https://jingdongwang2017.github.io/   

[11]   Part-Aligned Bilinear Representations for Person Re-identification. Yumin Suh, Jingdong Wang, Siyu Tang, Tao Mei, and Kyoung Mu Lee. ECCV 2018.  [pdf] [Code]
[10]   IGCV3: Interleaved Low-Rank Group Convolutions for Efficient Deep Neural Networks. Ke Sun, Mingjie Li, Dong Liu, and Jingdong Wang BMVC 2018.  [pdf] [Code]
[9]   Composite Quantization. Jingdong Wang, and Ting Zhang. TPAMI 2018.  [pdf] [Code]
[8]   Deep Convolutional Neural Networks with Merge-and-Run Mappings. Liming Zhao, Mingjie Li, Depu Meng, Xi Li, Zhaoxiang Zhang, Yueting Zhuang, Zhuowen Tu, and Jingdong Wang. IJCAI 2018.  [pdf] [Code]
[7]   IGCV2: Interleaved Structured Sparse Convolutional Neural Networks. Guotian Xie, Jingdong Wang, Ting Zhang, Jianhuang Lai, Richang Hong, and Guo-Jun Qi. CVPR 2018.  [pdf] [Code]
[6]   IGCV1: Interleaved Group Convolutions. Ting Zhang, Guo-Jun Qi, Bin Xiao, and Jingdong Wang. ICCV 2017.  [pdf]  [code]  [related papers]  [Zhihu]  [blog]
[5]   Deeply-Learned Part-Aligned Representations for Person Re-Identification. Liming Zhao, Xi Li, Yueting Zhuang, and Jingdong Wang. ICCV 2017.  [pdf]
[4]   Human Pose Estimation using Global and Local Normalization. Ke Sun, Cuiling Lan, Junliang Xing, Dong Liu, Wenjun Zeng, and Jingdong Wang. ICCV 2017.  [pdf]
[3]   Ensemble Diffusion for Retrieval. Song Bai, Zhichao Zhou, Jingdong Wang, Xiang Bai, Longin Jan Latecki, and Qi Tian. ICCV 2017.  [pdf]
[2]   A Survey on Learning to Hash. Jingdong Wang, Ting Zhang, Jingkuan Song, Nicu Sebe, and Heng Tao Shen. TPAMI, Accepted 2017.  [pdf v2]  [[pdf v1]]
[1]   Deeply-Fused Nets. Jingdong Wang, Zhen Wei, and Ting Zhang. arXiv.  [pdf]


### 一个简单的前景提取算法  

Simple Image saliency detection from histogram backprojection  

https://jacobgil.github.io/computervision/saliency-from-backproj   
