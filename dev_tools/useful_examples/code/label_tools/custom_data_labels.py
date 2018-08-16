#coding=utf-8
#!/usr/bin/env python3

import os
import shutil
import argparse
from xml.etree import ElementTree as ET  

classes = ["person", "bicycle", "car", "motorcycle", "airplane", "bus", "train", "truck", "boat", "traffic", "light", "fire", "hydrant", "stop", "sign", "parking", "meter", "bench", "bird", "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe", "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard", "sports", "ball", "kite", "baseball", "bat", "baseball", "glove", "skateboard", "surfboard", "tennis", "racket", "bottle", "wine", "glass", "cup", "fork", "knife", "spoon", "bowl", "banana", "apple", "sandwich", "orange", "broccoli", "carrot", "hot", "dog", "pizza", "donut", "cake", "chair", "couch", "potted", "plant", "bed", "dining", "table", "toilet", "tv", "laptop", "mouse", "remote", "keyboard", "cell", "phone", "microwave", "oven", "toaster", "sink", "refrigerator", "book", "clock", "vase", "scissors", "teddy", "bear", "hair", "drier", "toothbrush"]

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


def convert_annotation(dataset):
    '''
        生成 train.txt 文件和 labels/ 文件 

        labels 文件内容是通过将 xml 文件中的标注信息转换后得到  

        param@dataset: 要处理的数据集目录 

    '''
    global classes
    cwd = os.getcwd()

    xml_path = '{}/Annotations/'.format(dataset)
    xml_files = os.listdir(xml_path)

    labels_path = '{}/labels/'.format(dataset)
    if not os.path.exists(labels_path):
        os.makedirs(labels_path)
    else:
        shutil.rmtree(labels_path)
        os.makedirs(labels_path)

    train_file = open('{}/train.txt'.format(dataset), "a")
    for xml_file in xml_files:

        train_file.write('{}/{}/JPEGImages/{}.jpg\n'.format(cwd, dataset,os.path.splitext(xml_file)[0]))

        in_file = open(xml_path+xml_file)
        out_file = open('{}{}.txt'.format(labels_path, os.path.splitext(xml_file)[0]), 'w')
        print('{}{}.txt'.format(labels_path, os.path.splitext(xml_file)[0]))
        
        tree=ET.parse(in_file)
        root = tree.getroot()
        size = root.find('size')
        w = int(size.find('width').text)
        h = int(size.find('height').text)

        for obj in root.iter('object'):
            cls = obj.find('name').text
            if cls not in classes:
                print('error: {}/JPEGImages/{}.jpg'.format(dataset, os.path.splitext(xml_file)[0]))
                os.remove('{}/JPEGImages/{}.jpg'.format(dataset, os.path.splitext(xml_file)[0]))
                continue
            cls_id = classes.index(cls)
            xmlbox = obj.find('bndbox')
            b = (float(xmlbox.find('xmin').text), float(xmlbox.find('xmax').text), float(xmlbox.find('ymin').text), float(xmlbox.find('ymax').text))
            bb = convert((w,h), b)
            out_file.write(str(cls_id) + " " + " ".join([str(a) for a in bb]) + '\n')

        in_file.close()
        out_file.close()
    train_file.close()

if __name__ == "__main__":

    ap = argparse.ArgumentParser()
    ap.add_argument('--dataset', '-d', required=True)
    args = vars(ap.parse_args())

    dataset = args["dataset"]
    pos = dataset.find('/')
    if -1 != pos:
        dataset = dataset[:pos]

    convert_annotation(dataset)