# Dehazing

## Author: Jin Hwan, Kim

论文的详细介绍和结果展示可以看[这里]("http://mcl.korea.ac.kr/projects/dehazing").    

E-mail: arite AT korea.ac.kr   

## Install:

The program was tested using windows 7(Visual studio 2010) and ubunut 14.04.    

## Usage:

```bash
# video  
dehazing input_filename output_filename [frames]  

# image   
dehazing input_filename output_filename  
```
you may modify the some parameters in dehazing constructor or modify the code to add parameters to excutable file.   

## Example: 

```bash
dehazing "sample/cross.avi" "outputs/cross_out.avi" 100    

dehazing /home/klm/work/td_marco/images/video/DSC_0056.MOV "out.avi" 100 
```

## License:
 
The guided filter is He et al.'s guided image filtering, and we implement in C.  

The other part will be useful but WITHOUT ANY WARRANTY; without even the implied 
warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. Permission is 
granted for research and educational use.  

If you want to include this software in a commercial product, please contact 
the author.   

## Citation:  

[1]: "Temporally coherent real-time video dehazing," J.-H. Kim, W.-D. Jang, Y. Park,  
   D.-H. Lee, J.-Y. Sim, C.-S. Kim, in Proc. IEEE ICIP, 2012.
[2] "Optimized Contrast Enhancement for Real-Time Image and Video Dehazing," J.-H.  
   Kim, W.-D. Jang, J.-Y. Sim, C.-S. Kim, J. Vis. Commun. Image R. Vol. 24, No.3,  
   pp.410-425, Apr, 2013.   

## Version History:
  1.0   12-Mar-2013    Initial Release  


## 代码中变量命名方式约定   

p: 代表图片;   
n: int 类型;   
f: float 类型;  
P: Previous, 上一张图;   
R: Refined Transmission;  

例如: 
pn_ 就是图像数据类型为 int;  
pf_ 就是图像数据类型为 float;  
 

疑问?  
透射率初始值 427 是怎么来的?   
