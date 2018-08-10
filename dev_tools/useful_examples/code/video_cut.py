# coding: utf-8

# 开始截取视频, 按下 "s" 键表示开始截取, 按下 "q" 表示停止当前截取, 按下 "ESC" 表示停止程序   

import cv2
import os
import random
import copy

filename = "/home/klm/Downloads/miners.mp4"

cap = cv2.VideoCapture(filename)

if not cap.isOpened():
    print("{} open failed!".format(filename))

# 视频参数  
width = round(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = round(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

n_frames = round(cap.get(cv2.CAP_PROP_FRAME_COUNT))
fps = round(cap.get(cv2.CAP_PROP_FPS))
fourcc = int(cap.get(cv2.CAP_PROP_FOURCC))

(filename, ext) = os.path.splitext(filename)
output_filename = "{}_out_{:>03d}{}".format(filename, random.randint(0, 999), ext) 

vw = cv2.VideoWriter(output_filename, cv2.VideoWriter_fourcc('M', 'P', '4', '2'), fps, (width, height))

# 开始截取视频, 按下 "s" 键表示开始截取, 按下 "q" 表示停止当前截取, 按下 "ESC" 表示停止程序   

grab = False 

while n_frames:
    
    ret, frame = cap.read()
    convas = copy.deepcopy(frame)
    
    if ret:
        
        if grab:
            cv2.putText(convas, "recording...", (10, 10), cv2.FONT_HERSHEY_COMPLEX, 0.3, (0, 255, 0))
            vw.write(frame)
        
        cv2.imshow("srteam", convas)
        key = cv2.waitKey(1)
        key = key & 0xFF;
        
        if(key == ord('c') or key == ord('C')):
            grab = True
        elif(key == ord('q') or key == ord('Q')):
            grab = False
        elif(key == 27):
            grab = False
            cap.release()
            vw.release()
            cv2.destroyAllWindows()
        
    n_frames -= 1
