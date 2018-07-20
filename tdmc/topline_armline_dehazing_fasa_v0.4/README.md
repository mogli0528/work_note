# 液压支架顶梁和摇臂边缘线检测 - 去雾算法+显著目标检测   

在去雾算法的基础上增加了显著目标检测.       

## 算法   

Fast 显著特征检测论文:   

Yildirim G., Susstrunk S. FASA: Fast, Accurate, and Size-Aware Salient Object Detection [C].In Proceedings of the 12th Asian Conference on Computer Vision, Singapore, November 1-5, 2014.   

## Run

./topline nFrames  

## 结果  

由于计算的是视野中最为显著的一个, 因此, 采煤机一直会被作为显著特征检测出来.   
