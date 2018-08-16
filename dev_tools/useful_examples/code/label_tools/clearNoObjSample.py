#coding=utf8
import os
import argparse

ap = argparse.ArgumentParser()
ap.add_argument("--path", "-p", required=True)
args = vars(ap.parse_args())
path = args["path"]

txtFile = []

files = os.listdir(path)
for i in range(len(files)):
    if files[i].lower().endswith("txt"):
        txtFile.append(files[i])

for i in range(len(txtFile)):
    fileName = path + '/' + txtFile[i]
    input = open(fileName, 'r')
    num = input.read(1)
    input.close()
    if num == '0':
        os.remove(fileName)

        name = txtFile[i].split('.',1)[0]
        #print name
        jpgName = path + '/' + "".join(name) + ".jpg"
        if os.path.exists(jpgName):
            os.remove(jpgName)

        xmlName = path + '/' + "".join(name) + ".xml"
        if os.path.exists(xmlName):
            os.remove(xmlName)

