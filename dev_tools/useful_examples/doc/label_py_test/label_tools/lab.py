#coding=utf-8
import os
import cv2

'''
程序,名: ssd lab v1.0

ChangeLog[2018-01-26]:
    1. 修改了 clear(按键 c ) 时不是删除所有类别的 BBOX, 而是只删除当前标注类别的框. 
    2. 增加了右键删除鼠标所在的 BBOX 

'''

# 标注多个样本时想分开标注请修改这个参数为当前标注类别
current_labeled_cls = 0
# file_name_pre = "bbbackground_1_"
file_name_pre = "bx002_0_"
file_name_offset = len(file_name_pre)

#parameters
path = "bx_mul"
wndName = "ESC close, s save, c clean, p back"
# 欧式蛋糕, 旺仔, 合味道
# 蒙牛真果粒, 维他柠檬, 黑胡椒牛排面, 太平芝麻饼干 
className = ["bx012","bx013","bx014","bx015"]
colors = [(0, 255, 0), (255, 0, 0), (100, 0, 180), (255, 180, 0), (180, 0, 100)]

#code
currentClass = current_labeled_cls
imgs = os.listdir(path)
for i in range(len(imgs)-1, -1, -1):
    if not imgs[i].lower().endswith("jpg") and not imgs[i].lower().endswith("png") and not imgs[i].lower().endswith("jpeg"):
        del imgs[i]
# 使用数字排序
for i in range(len(imgs)):
    imgs[i] = int(imgs[i][file_name_offset:file_name_offset+6]) 
imgs.sort()
## 排序后再将文件名恢复 
for i in range(len(imgs)):
    # imgs[i] = "bx002_0_{:>06d}.jpg".format(imgs[i])
    imgs[i] = "{}{:>06d}.jpg".format(file_name_pre, imgs[i])
imgs = [(imgs[i], path + "/" + imgs[i]) for i in range(len(imgs))]

show = []
cach = []
cach2 = []
dpoint = []
epoint = []
objs = []
waitSecDown = False

def refreshCurrentShow():
    cv2.resize(show, (cach.shape[1], cach.shape[0]), cach)
    cv2.putText(cach, className[currentClass], (10, 30), 1, 2, colors[currentClass], 2)
    # cv2.putText(cach, "", (30, 30), 1, 2, colors[currentClass], 2)
    drawObjs(objs, cach)

    if waitSecDown:
        cv2.circle(cach, dpoint, 5, colors[currentClass], 2)
    cv2.imshow(wndName, cach)

def onMouse(event, x, y, flag, points):
    global dpoint, waitSecDown, wndName, epoint, cach

    if event == cv2.EVENT_LBUTTONDOWN:
        if waitSecDown:
            epoint = (x, y)
            waitSecDown = False
            objs.append((dpoint, epoint, currentClass))
        else:
            dpoint = (x, y)
            waitSecDown = True

        refreshCurrentShow()
    elif event == cv2.EVENT_MOUSEMOVE:
        cv2.resize(cach, (cach.shape[1], cach.shape[0]), cach2)
        cv2.line(cach2, (0, y), (cach.shape[1], y), (0, 255, 0), 2)
        cv2.line(cach2, (x, 0), (x, cach.shape[0]), (0, 255, 0), 2)
        cv2.imshow(wndName, cach2)
    elif event == cv2.EVENT_RBUTTONDOWN:
        print len(objs)
        for i in range(len(objs)-1, -1, -1):
            # xmin xmax
            xmin = min(objs[i][0][0], objs[i][1][0])
            xmax = max(objs[i][0][0], objs[i][1][0])
            ymin = min(objs[i][0][1], objs[i][1][1])
            ymax = max(objs[i][0][1], objs[i][1][1])
            # 删除错误框
            if x > xmin and x < xmax and y > ymin and y < ymax:
                del objs[i]

        refreshCurrentShow()


def drawObjs(objs, canvas):
    for item in objs:
        d = item[0]
        e = item[1]
        c = colors[item[2]]
        cv2.rectangle(canvas, d, e, c, 5)

def loadObjs(name):
    if not os.path.exists(name):
        return 0, []

    obs = []
    cls = 0
    with open(name, "r") as txt:
        info = txt.readline().split(",")
        num = int(info[0])
        cls = int(info[1])
        for i in range(num):
            info = txt.readline().split(",")
            d = (int(info[0]), int(info[1]))
            e = (int(info[2]), int(info[3]))
            c = int(info[4])
            obs.append((d, e, c))
    return cls, obs

def saveBreakpoint(bkp):
    with open("breakpoint.txt", "w") as p:
        p.write("%d" % bkp)

def loadBreakpoint():
    p = 0

    if os.path.exists("breakpoint.txt"):
        with open("breakpoint.txt", "r") as p:
            p = int(p.read())
    return p

def saveXML(name, objs, cls, w, h):
    with open(name, "w") as xml:
        xml.write("<annotation><size><width>%d</width><height>%d</height></size>" % (w, h))
        for item in objs:
            fmt = """
            <object>
                <name>%s</name>
                <bndbox>
                    <xmin>%d</xmin>
                    <ymin>%d</ymin>
                    <xmax>%d</xmax>
                    <ymax>%d</ymax>
                </bndbox>
            </object>
            """

            d = item[0]
            e = item[1]
            pmin = (min(d[0], e[0]), min(d[1], e[1]))
            pmax = (max(d[0], e[0]), max(d[1], e[1]))

            cls = item[2]
            xml.write(fmt % (className[cls], pmin[0], pmin[1], pmax[0], pmax[1]))

        xml.write("</annotation>")

    with open(name + ".txt", "w") as txt:
        txt.write("%d,%d\n" % (len(objs), cls))
        for item in objs:
            d = item[0]
            e = item[1]
            pmin = (min(d[0], e[0]), min(d[1], e[1]))
            pmax = (max(d[0], e[0]), max(d[1], e[1]))

            cls = item[2]
            txt.write("%d,%d,%d,%d,%d,%s\n" % (pmin[0], pmin[1], pmax[0], pmax[1], cls, className[cls]))



if len(imgs) == 0:
    print 'empty imgs dir.'
    exit(1)


endOf = False;
i = loadBreakpoint()
while True:
    if i < 0:
        i = len(imgs) + i
    elif i > len(imgs) - 1:
        i = i - len(imgs)

    saveBreakpoint(i)
    show = cv2.imread(imgs[i][1])
    cach = cv2.imread(imgs[i][1])
    cach2 = cv2.imread(imgs[i][1])
    pos = imgs[i][0].rfind(".")
    currentClass, objs = loadObjs("%s/%s.xml.txt" %(path, imgs[i][0][:pos]))
    waitSecDown = False
    currentClass = current_labeled_cls
    while True:
        cv2.resize(show, (cach.shape[1], cach.shape[0]), cach)

        refreshCurrentShow()
        cv2.setMouseCallback(wndName, onMouse)
        key = cv2.waitKey()
        key = key & 0xFF;

        if key == 0x1B: #ESC
            endOf = True;
            break

        if key >= ord('1') and key <= ord('9'):
            currentClass = key - ord('1')
            currentClass = max(currentClass, 0)
            currentClass = min(len(className) - 1, currentClass)
            continue

        if key == ord('c'):
            # clear 时不是删除所有的框框,而是只删除当前标注类别的框. 
            for i in range(len(objs)-1, -1, -1):
                if currentClass == objs[i][2]:
                    del objs[i]
            continue

        if key == ord('d'):
            currentClass = 0
            objs = [] 
            continue

        if key == 82 or key == 44:   #up
            i = i - 2
            break

        if key == 84 or key == 46:   #down
            break

        if key == ord('p'):
            if len(objs)>0:
                del objs[len(objs)-1]
            continue

        if key == ord('s'): #s
            saveXML("%s/%s.xml" %(path, imgs[i][0][:pos]), objs, currentClass, show.shape[1], show.shape[0])
            break

    if endOf: break;
    i = i+1
