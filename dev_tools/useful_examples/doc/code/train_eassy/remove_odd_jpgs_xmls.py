#!/usr/bin/env python
#coding=utf-8
'''
功能： 删除多余的 jpg 或 xml 文件
'''

import os
import sys

jpg_path = "JPEGImages"
xml_path = "Annotations"


xmls = os.listdir(xml_path)
jpgs = os.listdir(jpg_path)

if len(xmls) < len(jpgs):
    print "jpgs is odd..."  

    for jpg in jpgs:
        pos = jpg.find(".jpg")
        xml_right = jpg[:pos]+".xml"
        if xml_right not in xmls:
            print "remove %s..." % xml_right 
            os.remove(os.path.join(xml_path, xml_right))
elif len(xmls) > len(jpgs):
    print "xmls is odd..."  
     for xml in xmls:
        pos = xml.find(".xml")
        jpg_right = xml[:pos]+".jpg"
        if jpg_right not in jpgs:
            print "remove %s..." % xml 
            os.remove(os.path.join(jpg_path, jpg_right))
