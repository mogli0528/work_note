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
2018-01-08
    解决了 xml 文件为空的情况. 一旦出现空的 xml 文件,就需要将对应的 jpg 文件删除
     
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


def loadROI(jpgPath, xml_file):
    # per=ET.parse('testXml.xml')  
    # xml_file = xml_file + '.xml'
    roi = []
    rois = []
    xmin = 0
    ymin = 0
    xmax = 0
    ymax = 0 
    class_name = []
    print xml_file

    # 确保 xml 文件不为空文件
    if os.path.getsize(xml_file) == 0:
        # 一旦出现空的 xml 文件, 就需要将对应的 jpg 文件一并删除   
        basename = os.path.basename(xml_file)
        pre_file, ext = os.path.splitext(basename)
        jpg_file = pre_file + ".jpg"
        jpg_file_fullpath = os.path.join(jpgPath, jpg_file)
        os.remove(jpg_file_fullpath)
        os.remove(xml_file)
    else:
        per=ET.parse(xml_file) 

        name_node = per.getiterator("name") 
        if len(name_node) > 0:
            for name in name_node:
                #print "node.text:%s" % name.text   
                class_name.append(name.text)

            lst_node = per.getiterator("bndbox")  
            if len(lst_node) > 0:
                for oneper in lst_node:  # 找出 bndbox 节点  
                    for child in oneper.getchildren(): # 找出 bndbox 节点的子节点  
                        
                        if child.tag == 'xmin':
                            # xmin = int(child.text)
                            xmin = int(round(float(child.text)))
                            
                        elif child.tag == 'ymin':
                            # ymin = int(child.text)
                            ymin = int(round(float(child.text)))
                            
                        elif child.tag == 'xmax':
                            # xmax = int(child.text)
                            xmax = int(round(float(child.text)))

                        elif child.tag == 'ymax':
                            # ymax = int(child.text)
                            ymax = int(round(float(child.text)))

                        roi=[[xmin, ymin],[xmax,ymax]] # 此时解析完一个框,矩形框的两个角点
                    
                    rois.append(roi)
                    # print xmin, ymin, xmax, ymax

    print len(rois), len(class_name)
    if len(rois) == len(class_name):
        return rois, class_name
    else:
        return [], []

# 保存为ROI图片        
def saveROI(name, class_name, rois, img):
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


# 在图片上将ROI区域框选出来  
def saveRect(name, class_name, rois, img):
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
    rois, class_name = loadROI(jpgPath, "%s/%s.xml" %(xmlPath, imgs[i][0][:pos]))
    # currentClass, objs = loadObjs("%s/%s.xml.txt" %(path, imgs[i][0][:pos]))
    # print rois
    saveROI("%s.jpg" %(imgs[i][0][:pos]), class_name, rois, show)
    #saveRect("%s.jpg" %(imgs[i][0][:pos]), class_name, rois, show)
    i = i+1
