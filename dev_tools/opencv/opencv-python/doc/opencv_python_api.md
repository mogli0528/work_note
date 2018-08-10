# OpenCV 的 Python API  

## 图像深拷贝   

~~~python
import copy

convas = copy.deepcopy(frame)
~~~

## Video 相关  

cv2.VideoCapture(filename)    
VideoCapture.isOpened()   
VideoCapture.get()   
ret, frame = VideoCapture.read()
VideoCapture.release()

cv2.VideoWriter(filename, fourcc, fps, (width, height))
cv2.VideoWriter_fourcc('M', 'P', '4', '2')   
VideoWriter.write(frame)
VideoWriter.release()

* cv2.CAP_PROP_FRAME_WIDTH;    
* cv2.CAP_PROP_FRAME_HEIGHT;    
* cv2.CAP_PROP_FRAME_COUNT;  // 获取视频总的帧数, 可用于遍历视频流  
* cv2.CAP_PROP_FPS;    
* cv2.CAP_PROP_FOURCC;    


## 图像中显示文字   

cv2.putText(img, text, orgin, fontFace, fontScale, color[, thickness[, lineType[, bottomLeftOrigin]]]) -> img

eg:  

cv2.putText(frame, capture, (10, 10), cv2.FONT_HERSHEY_COMPLEX, 0.3, (0, 255, 0))


## 监听用户按键动作   

cv2.waitKey(1)   
key == ord('c')
cv2.destroyAllWindows()   


