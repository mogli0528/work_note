#coding=utf-8
'''
功能： 使用标注得到的保存在.xml中的BBOX提取出ROI区域
      然后在原jpg上标出ROI框并保存
注意： 
      ROI 区域的切片规则是：[row_s:row_e，column_s:column_e]

changelog:
2017-12-29
    修改了bounding box为浮点数表示的形式,另外对xml中各个域的检查做了严格判断    
2018-01-03
    修改图片大小为 0kb 的数据.
    解决了由于自动标注工具造成的 xmin < 0 的情况.  
'''


import os
import cv2
import time
from xml.etree import ElementTree as ET  

# 保存.jpg和.xml文件的目录
# test
jpgPath = "jpg"
xmlPath = "xml"

# Standard directory
#jpgPath = "JPEGImages"
#xmlPath = "Annotations"

colors = [(0, 255, 0), (255, 0, 0)]

#code
imgs = os.listdir(jpgPath)
imgs = [(imgs[i], jpgPath + "/" + imgs[i]) for i in range(len(imgs))]
for i in range(len(imgs)-1, -1, -1):
    if not imgs[i][1].lower().endswith("jpg") and not imgs[i][1].lower().endswith("png") and not imgs[i][1].lower().endswith("jpeg"):
        del imgs[i]

xmls = os.listdir(xmlPath)
xmls = [(xmls[i], xmlPath + "/" + xmls[i]) for i in range(len(xmls))]
for i in range(len(xmls)-1, -1, -1):
    if not xmls[i][1].lower().endswith("xml"):
        del xmls[i]

if(len(imgs) != len(xmls)):
    print "error: imgs is not equals to xmls counts."
    exit(1)

# for i in range(len(imgs)-1, -1, -1):
#     print i,
# while 1:
#     time.sleep(10)


show = []
dpoint = []
epoint = []


def load_bbox_info_from_xml(path):
    # per=ET.parse('testXml.xml')  
    # path = path + '.xml'
    print path
    per=ET.parse(path) 

    roi = []
    rois = []           # 存放本 xml 文件中所有的 bounding box 
    class_name = []     # 存放本 xml 文件中所有的 name
    xmin = 0
    ymin = 0
    xmax = 0
    ymax = 0 

    # 找出 xml 中包含的所有的 name, 和下面的 bndbox 意义对应
    name_node = per.getiterator("name") 
    if len(name_node) > 0:
        for name in name_node:
            class_name.append(name.text)

        # 找出 xml 中包含的所有的 bndbox
        lst_node = per.getiterator("bndbox")  
        if len(lst_node) > 0:
            for oneper in lst_node:   
                for child in oneper.getchildren(): # 找出 bndbox 节点的子节点  
                    
                    if child.tag == 'xmin':
                        xmin = int(round(float(child.text)))
                        
                    elif child.tag == 'ymin':
                        ymin = int(round(float(child.text)))
                        
                    elif child.tag == 'xmax':
                        xmax = int(round(float(child.text)))

                    elif child.tag == 'ymax':
                        ymax = int(round(float(child.text)))

                    roi=[[xmin, ymin],[xmax,ymax]] # 此时解析得到矩形框的两个角点坐标
                
                rois.append(roi)  # end of get a rectangle corrd
               
    print len(rois), len(class_name)

    if len(rois) == len(class_name):
        return rois, class_name
    else:
        print "class_name's number need to equals rois' number, %d vs %d" %(len(class_name), len(rois))
        return [], []

# 保存为ROI图片        
def save_roi_with_bbox(name, class_name, rois, img):
    index = 0

    if(not os.path.exists('roi')):
        os.makedirs('roi')

    if len(rois):

        for roi in rois:
            class_path = 'roi/'+ class_name[index]
            print class_path
            if(not os.path.exists(class_path)):
                os.makedirs(class_path)
            
            full_name = 'roi/' + class_name[index] + '/' + name 

            d = roi[0]
            e = roi[1]
            pmin = (min(d[0], e[0]), min(d[1], e[1]))
            pmax = (max(d[0], e[0]), max(d[1], e[1]))

            colStart = pmin[0]
            colEnd = pmax[0]
            rowStart = pmin[1]
            rowEnd = pmax[1]

            # roiImg = np.zeros((height,width,3), np.uint8)
            # roiImg = (colStart, rowStart), (colEnd, rowEnd)
            # roiImg = ( rowStart:rowEnd, colStart:colEnd)

            # 剔除异常的 bounding box 
            if (rowEnd - rowStart) * (colEnd - colStart) > 5:
                # 这种 xmin < 0 的情况一般是由于自动标注工具造成的.  
                if rowEnd > 0 and rowStart > 0 and colEnd > 0 and colStart > 0:
                    cv2.imwrite(full_name, img[rowStart:rowEnd, colStart:colEnd], [int(cv2.IMWRITE_JPEG_QUALITY), 100])   # 100 is the highest quality.
            else:
                print "waring: %s, roi:" % full_name
                print colStart, colEnd, rowStart, rowEnd
            # cv2.imwrite(full_name, img[colStart:colEnd, rowStart:rowEnd], [int(cv2.IMWRITE_JPEG_QUALITY), 100])   # 100 is the highest quality.

            index = index + 1


# 将 bounding box 画在jpg上并保存  
def draw_bbox_on_image(name, class_name, rois, img):
    index = 0
    class_path = 'roi/'+ class_name[index]

    if(not os.path.exists('roi')):
        os.makedirs('roi')
    if(not os.path.exists(class_path)):
        os.makedirs(class_path)

    if len(rois):
        for roi in rois:
            full_name = 'roi/' + class_name[index] + '/' + name 

            d = roi[0]
            e = roi[1]
            pmin = (min(d[0], e[0]), min(d[1], e[1]))
            pmax = (max(d[0], e[0]), max(d[1], e[1]))

            colStart = pmin[0]
            colEnd = pmax[0]
            rowStart = pmin[1]
            rowEnd = pmax[1]

            # roiImg = np.zeros((height,width,3), np.uint8)
            # roiImg = (colStart, rowStart), (colEnd, rowEnd)
            # roiImg = ( rowStart:rowEnd, colStart:colEnd)
            cv2.rectangle(img, (colStart, rowStart), (colEnd, rowEnd), (255,0,0), 2)
            cv2.imwrite(full_name, img,[int(cv2.IMWRITE_JPEG_QUALITY), 100])   # 100 is the highest quality.

            index = index + 1


if len(imgs) == 0:  
    print "empty imgs dir."
    exit(1)


# Debug
# for i in range(len(imgs)-1, -1, -1):
#     print i
# while 1:
#     time.sleep(10)


while i < len(imgs):

    show = cv2.imread(imgs[i][1])
    pos = imgs[i][0].rfind(".")   # 文件扩展名的前缀
    rois, class_name = load_bbox_info_from_xml("%s/%s.xml" %(xmlPath, imgs[i][0][:pos]))
    # currentClass, objs = loadObjs("%s/%s.xml.txt" %(path, imgs[i][0][:pos]))
    # print rois
    save_roi_with_bbox("%s.jpg" %(imgs[i][0][:pos]), class_name, rois, show)
    #draw_bbox_on_image("%s.jpg" %(imgs[i][0][:pos]), class_name, rois, show)
    i = i+1
