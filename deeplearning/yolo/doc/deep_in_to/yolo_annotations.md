# YOLOv3 的标注数据格式转化与使用   

> coco API   

> darknet/scripts/voc_label.py  

## YOLOv3 的标注数据格式转化

### 1. VOC 的标注数据格式

我们熟知的像 VOC 的标注格式, 给出标注目标的类别, 标注目标的位置是以矩形的两个角点给出. 数据以 xml 格式存放.   

下面是 VOC 的 Annotations 数据, 即 xml 文件中的内容.  

~~~xml
<annotation>
	<folder>VOC2007</folder>
	<filename>000001.jpg</filename>
	...
	<size>
		<width>353</width>
		<height>500</height>
		<depth>3</depth>
	</size>
	<segmented>0</segmented>
	<object>
		<name>dog</name>
		<pose>Left</pose>
		<truncated>1</truncated>
		<difficult>0</difficult>
		<bndbox>
			<xmin>48</xmin>
			<ymin>240</ymin>
			<xmax>195</xmax>
			<ymax>371</ymax>
		</bndbox>
	</object>
	<object>
		<name>person</name>
		<pose>Left</pose>
		<truncated>1</truncated>
		<difficult>0</difficult>
		<bndbox>
			<xmin>8</xmin>
			<ymin>12</ymin>
			<xmax>352</xmax>
			<ymax>498</ymax>
		</bndbox>
	</object>
</annotation>
~~~

上面的 xml 文件中, 有两个 object, 说明在标注的图片中存在两个目标. 目标的边界框在 <bndbox> </bndbox> 之间给出.  

### 2. YOLOv3 训练需要的标注数据格式   

YOLOv3 的标注格式:

~~~text
<object-class> <x> <y> <width> <height>
~~~

其中, <x> <y> <width> <height> 表示的是标注得到的边界框参数所占原图宽/高的比例(数据类型为浮点数), 范围是(0.0 to 1.0].   

注意: <x> <y> - 是标注框的中心, 而不是左上角点.  

如: 1 0.716797 0.395833 0.216406 0.147222   

### 3. 将 VOC 标注数据格式转换为 YOLOv3 格式  

转换的思路其实也很简单, 就是按照 YOLOv3 的格式对标注框参数进行表示转换. VOC 标注数据通过下面的脚本进行转换.   

~~~python

def convert(size, box):
    '''
        将标准的 VOC xml 标注样式转换为 yolo 格式:
            <object-class> <x> <y> <width> <height>
        <x> <y> <width> <height> 表示的是标注框参数所占原图宽/高比例的浮点数, 范围是(0.0 to 1.0]. 
        注意: <x> <y> - 是标注框的中心, 而不是左上角点.  

        如: 
            1 0.716797 0.395833 0.216406 0.147222
        
        pram@size: 原图的宽和高  
        pram@box: (xmin,xmax), (ymin,ymax) 表示的边界框  
    '''
    dw = 1./(size[0])
    dh = 1./(size[1])

    x = (box[0] + box[1])/2.0 - 1
    y = (box[2] + box[3])/2.0 - 1
    w = box[1] - box[0]
    h = box[3] - box[2]

    x = x*dw
    w = w*dw
    y = y*dh
    h = h*dh

    return (x,y,w,h)
~~~

## YOLOv3 是如何找到标注数据并在训练中使用   

上一步中是为 YOLOv3 的训练做前期的数据准备工作, 之后就可以开始训练了.    

但是在此之前还需要弄明白一个问题: YOLOv3 是如何保存这些标注数据的? 又是在训练时如何找到这些标注数据的?  

### 1. YOLOv3 是如何保存标注数据的?  

在标注数据格式转换完毕之后, 应该将其写入文件中供训练时使用. VOC 标注文件通过下面的脚本实现保存:  

~~~python
def convert_annotation(year, image_id):
    in_file = open('VOCdevkit/VOC%s/Annotations/%s.xml'%(year, image_id))
    out_file = open('VOCdevkit/VOC%s/labels/%s.txt'%(year, image_id), 'w')
    
    tree=ET.parse(in_file)
    root = tree.getroot()
    size = root.find('size')
    w = int(size.find('width').text)
    h = int(size.find('height').text)

    for obj in root.iter('object'):
        difficult = obj.find('difficult').text
        cls = obj.find('name').text
        if cls not in classes or int(difficult)==1:
            continue
        cls_id = classes.index(cls)
        xmlbox = obj.find('bndbox')
        b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text), float(xmlbox.find('ymax').text))
        bb = convert((w,h), b)
        out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')
~~~

其中, image_id 是训练数据集中每张图唯一的图片编号, 根据一定的比例(训练测试集 9:1)提前生成相应的 train.txt;  

标注数据被保存到 labels/ 目录下的对应的 $(image_id).txt 文件中.  

### 2. YOLOv3 在训练是如何找到这些标注数据的?  

load_data() 负责加载训练所需的数据到内存/显存中, 主要的逻辑在 fill_truth_detection() 函数中.  

~~~cpp
void fill_truth_detection(char *path, int num_boxes, float *truth, int classes, int flip, float dx, float dy, float sx, float sy)
{
    char labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    int count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    if(count > num_boxes) count = num_boxes;
    float x,y,w,h;
    int id;
    int i;
    int sub = 0;

    for (i = 0; i < count; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        if ((w < .001 || h < .001)) {
            ++sub;
            continue;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    free(boxes);
}
~~~

find_replace() 函数的作用是字符串查找替换.  
