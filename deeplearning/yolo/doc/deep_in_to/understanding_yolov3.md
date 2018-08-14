# 更好的理解 YOLOv3 

## 1. YOLOv3 中的数据增广并不是在 BndBox 中完成, 而是在原图中完成  

主要涉及函数: fill_truth_detection();  

## 2. data 数据结构  

typedef struct{
    int w, h;
    matrix X;
    matrix y;
    int shallow;
    int *num_boxes;
    box **boxes;
} data;

typedef struct load_args{
    int threads;  // 
    char **paths;
    char *path;
    int n;
    int m;
    char **labels;
    int h;
    int w;
    int out_w;
    int out_h;
    int nh;
    int nw;
    int num_boxes;
    int min, max, size;
    int classes;
    int background;
    int scale;
    int center;
    int coords;
    float jitter;
    float angle;
    float aspect;
    float saturation;
    float exposure;
    float hue;
    data *d;
    image *im;
    image *resized;
    data_type type;
    tree *hierarchy;
} load_args;


## 3. subdivisions  

从 parse_net_option()函数可知，net.batch = net.batch / net.subdivision，等号右边的那个batch就是
// 网络配置文件.cfg中设置的每个batch的图片数量，但是不知道为什么多了subdivision这个参数？总之，


net.batch * net.subdivisions又得到了在网络配置文件中设定的batch值，然后乘以ngpus，是考虑多个GPU实现数据并行，
// 一次读入多个batch的数据，分配到不同GPU上进行训练。在load_threads()函数中，又将整个的n仅可能均匀的划分到每个线程上，
// 也就是总的读入图片张数为n = net.batch * net.subdivisions * ngpus，但这些图片不是一个线程读完的，而是分配到多个线程并行读入，
// 因此本函数中的n实际不是总的n，而是分配到该线程上的n，比如总共要读入128张图片，共开启8个线程读数据，那么本函数中的n为16,而不是总数128


## 4. 原始数据读取   

im.data[k*w*h + i*w + j] = data[i*step + j*c + k]/255.;

// 在data中的存储方式是三通道杂揉在一起的：rgbrgbrgb...，因此，
// src_index = k + c(i+w*j)中，i+w*j表示单通道的偏移，乘以c则包括总共3通道的偏移，
// 加上w表示要读取w通道的灰度值。
// 比如，图片原本是颜色图，因此data原本应该是rgbrgbrgb...类型的数据，
// 但如果指定的channels=1,data将是经过转换后通道数为1的图像数据，这时k=0，只能读取一个通道的数据;
// 如果channels=3，那么data保持为rgbrgbrgb...存储格式，这时w=0将读取所有r通道的数据，
// w=1将读取所有g通道的数据，w=2将读取所有b通道的数据

## 5. 数据增广函数  

load_data_detection() 函数  



## 6. 如何查看增广后的图片

在 load_data_detection() 函数中添加图片保存方式或者添加实时显示图片逻辑.   



## 7. 时刻防止内存泄露 

free()  
 
## 模型性能评价  

test_detector()  

validate_detector()

validate_detector_flip()  

validate_detector_recall()  
