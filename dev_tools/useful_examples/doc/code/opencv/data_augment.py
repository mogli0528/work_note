#coding=utf-8
'''
功能： 图片数据增广小工具
    image_augmentation.py

'''
import numpy as np
import cv2

'''
定义裁剪函数，四个参数分别是：
    左上角横坐标x0
    左上角纵坐标y0
    裁剪宽度w
    裁剪高度h
'''
crop_image = lambda img, x0, y0, w, h: img[y0:y0+h, x0:x0+w]

'''
随机裁剪
    area_ratio为裁剪画面占原画面的比例
    hw_vari是扰动占原高宽比的比例范围
'''
def random_crop(img, area_ratio, hw_vari):
    h, w = img.shape[:2]
    hw_delta = np.random.uniform(-hw_vari, hw_vari)
    hw_mult = 1 + hw_delta

    # 下标进行裁剪，宽高必须是正整数
    w_crop = int(round(w*np.sqrt(area_ratio*hw_mult)))

    # 裁剪宽度不可超过原图可裁剪宽度
    if w_crop > w:
    w_crop = w

    h_crop = int(round(h*np.sqrt(area_ratio/hw_mult)))
    if h_crop > h:
    h_crop = h

    # 随机生成左上角的位置
    x0 = np.random.randint(0, w-w_crop+1)
    y0 = np.random.randint(0, h-h_crop+1)

    return crop_image(img, x0, y0, w_crop, h_crop)

'''
定义旋转函数：
angle是逆时针旋转的角度
crop是个布尔值，表明是否要裁剪去除黑边
'''
def rotate_image(img, angle, crop):
h, w = img.shape[:2]

    # 旋转角度的周期是360°
angle %= 360

    # 用OpenCV内置函数计算仿射矩阵
M_rotate = cv2.getRotationMatrix2D((w/2, h/2), angle, 1)

    # 得到旋转后的图像
img_rotated = cv2.warpAffine(img, M_rotate, (w, h))

    # 如果需要裁剪去除黑边
if crop:
     # 对于裁剪角度的等效周期是180°
angle_crop = angle % 180

        # 并且关于90°对称
if angle_crop > 90:
angle_crop = 180 - angle_crop

        # 转化角度为弧度
theta = angle_crop * np.pi / 180.0

        # 计算高宽比
hw_ratio = float(h) / float(w)

        # 计算裁剪边长系数的分子项
tan_theta = np.tan(theta)
numerator = np.cos(theta) + np.sin(theta) * tan_theta

        # 计算分母项中和宽高比相关的项
r = hw_ratio if h > w else 1 / hw_ratio

        # 计算分母项
denominator = r * tan_theta + 1

        # 计算最终的边长系数
crop_mult = numerator / denominator

        # 得到裁剪区域
w_crop = int(round(crop_mult*w))
h_crop = int(round(crop_mult*h))
x0 = int((w-w_crop)/2)
y0 = int((h-h_crop)/2)

img_rotated = crop_image(img_rotated, x0, y0, w_crop, h_crop)

return img_rotated

'''
随机旋转
angle_vari是旋转角度的范围[-angle_vari, angle_vari)
p_crop是要进行去黑边裁剪的比例
'''
def random_rotate(img, angle_vari, p_crop):
angle = np.random.uniform(-angle_vari, angle_vari)
crop = False if np.random.random() > p_crop else True
return rotate_image(img, angle, crop)

'''
定义hsv变换函数：
hue_delta是色调变化比例
sat_delta是饱和度变化比例
val_delta是明度变化比例
'''
def hsv_transform(img, hue_delta, sat_mult, val_mult):
img_hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV).astype(np.float)
img_hsv[:, :, 0] = (img_hsv[:, :, 0] + hue_delta) % 180
img_hsv[:, :, 1] *= sat_mult
img_hsv[:, :, 2] *= val_mult
img_hsv[img_hsv > 255] = 255
return cv2.cvtColor(np.round(img_hsv).astype(np.uint8), cv2.COLOR_HSV2BGR)

'''
随机hsv变换
hue_vari是色调变化比例的范围
sat_vari是饱和度变化比例的范围
val_vari是明度变化比例的范围
'''
def random_hsv_transform(img, hue_vari, sat_vari, val_vari):
hue_delta = np.random.randint(-hue_vari, hue_vari)
sat_mult = 1 + np.random.uniform(-sat_vari, sat_vari)
val_mult = 1 + np.random.uniform(-val_vari, val_vari)
return hsv_transform(img, hue_delta, sat_mult, val_mult)

'''
定义gamma变换函数：
gamma就是Gamma
'''
def gamma_transform(img, gamma):
gamma_table = [np.power(x / 255.0, gamma) * 255.0 for x in range(256)]
gamma_table = np.round(np.array(gamma_table)).astype(np.uint8)
return cv2.LUT(img, gamma_table)

'''
随机gamma变换
gamma_vari是Gamma变化的范围[1/gamma_vari, gamma_vari)
'''
def random_gamma_transform(img, gamma_vari):
log_gamma_vari = np.log(gamma_vari)
alpha = np.random.uniform(-log_gamma_vari, log_gamma_vari)
gamma = np.exp(alpha)
return gamma_transform(img, gamma)

调用这些函数需要通过一个主程序。这个主程序里首先定义三个子模块，定义一个函数parse_arg()通过Python的argparse模块定义了各种输入参数和默认值。需要注意的是这里用argparse来输入所有参数是因为参数总量并不是特别多，如果增加了更多的扰动方法，更合适的参数输入方式可能是通过一个配置文件。然后定义一个生成待处理图像列表的函数generate_image_list()，根据输入中要增加图片的数量和并行进程的数目尽可能均匀地为每个进程生成了需要处理的任务列表。执行随机扰动的代码定义在augment_images()中，这个函数是每个进程内进行实际处理的函数，执行顺序是镜像-->裁剪-->旋转-->HSV-->Gamma。需要注意的是镜像-->裁剪，因为只是个演示例子，这未必是一个合适的顺序。最后定义一个main函数进行调用，代码如下：

import os
import argparse
import random
import math
from multiprocessing import Process
from multiprocessing import cpu_count

import cv2

# 导入image_augmentation.py为一个可调用模块
import image_augmentation as ia

# 利用Python的argparse模块读取输入输出和各种扰动参数
def parse_args():
parser = argparse.ArgumentParser(
description='A Simple Image Data Augmentation Tool',
formatter_class=argparse.ArgumentDefaultsHelpFormatter)

parser.add_argument('input_dir',
help='Directory containing images')
parser.add_argument('output_dir',
help='Directory for augmented images')
parser.add_argument('num',
help='Number of images to be augmented',
type=int)

parser.add_argument('--num_procs',
help='Number of processes for paralleled augmentation',
type=int, default=cpu_count())

parser.add_argument('--p_mirror',
help='Ratio to mirror an image',
type=float, default=0.5)

parser.add_argument('--p_crop',
help='Ratio to randomly crop an image',
type=float, default=1.0)
parser.add_argument('--crop_size',
help='The ratio of cropped image size to original image size, in area',
type=float, default=0.8)
parser.add_argument('--crop_hw_vari',
help='Variation of h/w ratio',
type=float, default=0.1)

parser.add_argument('--p_rotate',
help='Ratio to randomly rotate an image',
type=float, default=1.0)
parser.add_argument('--p_rotate_crop',
help='Ratio to crop out the empty part in a rotated image',
type=float, default=1.0)
parser.add_argument('--rotate_angle_vari',
help='Variation range of rotate angle',
type=float, default=10.0)

parser.add_argument('--p_hsv',
help='Ratio to randomly change gamma of an image',
type=float, default=1.0)
parser.add_argument('--hue_vari',
help='Variation of hue',
type=int, default=10)
parser.add_argument('--sat_vari',
help='Variation of saturation',
type=float, default=0.1)
parser.add_argument('--val_vari',
help='Variation of value',
type=float, default=0.1)

parser.add_argument('--p_gamma',
help='Ratio to randomly change gamma of an image',
type=float, default=1.0)
parser.add_argument('--gamma_vari',
help='Variation of gamma',
type=float, default=2.0)

args = parser.parse_args()
args.input_dir = args.input_dir.rstrip('/')
args.output_dir = args.output_dir.rstrip('/')

return args

'''
根据进程数和要增加的目标图片数，
生成每个进程要处理的文件列表和每个文件要增加的数目
'''
def generate_image_list(args):
# 获取所有文件名和文件总数
filenames = os.listdir(args.input_dir)
num_imgs = len(filenames)

    # 计算平均处理的数目并向下取整
num_ave_aug = int(math.floor(args.num/num_imgs))

    # 剩下的部分不足平均分配到每一个文件，所以做成一个随机幸运列表
    # 对于幸运的文件就多增加一个，凑够指定的数目
rem = args.num - num_ave_aug*num_imgs
lucky_seq = [True]*rem + [False]*(num_imgs-rem)
random.shuffle(lucky_seq)

    # 根据平均分配和幸运表策略，
    # 生成每个文件的全路径和对应要增加的数目并放到一个list里
img_list = [
(os.sep.join([args.input_dir, filename]), num_ave_aug+1 if lucky else num_ave_aug)
for filename, lucky in zip(filenames, lucky_seq)
]

    # 文件可能大小不一，处理时间也不一样，
    # 所以随机打乱，尽可能保证处理时间均匀
random.shuffle(img_list)

    # 生成每个进程的文件列表，
    # 尽可能均匀地划分每个进程要处理的数目
length = float(num_imgs) / float(args.num_procs)
indices = [int(round(i * length)) for i in range(args.num_procs + 1)]
return [img_list[indices[i]:indices[i + 1]] for i in range(args.num_procs)]

# 每个进程内调用图像处理函数进行扰动的函数
def augment_images(filelist, args):
# 遍历所有列表内的文件
for filepath, n in filelist:
img = cv2.imread(filepath)
filename = filepath.split(os.sep)[-1]
dot_pos = filename.rfind('.')

        # 获取文件名和后缀名
imgname = filename[:dot_pos]
ext = filename[dot_pos:]

print('Augmenting {} ...'.format(filename))
for i in range(n):
img_varied = img.copy()

            # 扰动后文件名的前缀
varied_imgname = '{}_{:0>3d}_'.format(imgname, i)

            # 按照比例随机对图像进行镜像
if random.random() < args.p_mirror:
             # 利用numpy.fliplr(img_varied)也能实现
img_varied = cv2.flip(img_varied, 1)
varied_imgname += 'm'

            # 按照比例随机对图像进行裁剪
if random.random() < args.p_crop:
img_varied = ia.random_crop(
img_varied,
args.crop_size,
args.crop_hw_vari)
varied_imgname += 'c'

            # 按照比例随机对图像进行旋转
if random.random() < args.p_rotate:
img_varied = ia.random_rotate(
img_varied,
args.rotate_angle_vari,
args.p_rotate_crop)
varied_imgname += 'r'

            # 按照比例随机对图像进行HSV扰动
if random.random() < args.p_hsv:
img_varied = ia.random_hsv_transform(
img_varied,
args.hue_vari,
args.sat_vari,
args.val_vari)
varied_imgname += 'h'

            # 按照比例随机对图像进行Gamma扰动
if random.random() < args.p_gamma:
img_varied = ia.random_gamma_transform(
img_varied,
args.gamma_vari)
varied_imgname += 'g'

            # 生成扰动后的文件名并保存在指定的路径
output_filepath = os.sep.join([
args.output_dir,
'{}{}'.format(varied_imgname, ext)])
cv2.imwrite(output_filepath, img_varied)

# 主函数
def main():
# 获取输入输出和变换选项
args = parse_args()
params_str = str(args)[10:-1]

    # 如果输出文件夹不存在，则建立文件夹
if not os.path.exists(args.output_dir):
os.mkdir(args.output_dir)

print('Starting image data augmentation for {}\n'
'with\n{}\n'.format(args.input_dir, params_str))

    # 生成每个进程要处理的列表
sublists = generate_image_list(args)

    # 创建进程
processes = [Process(target=augment_images, args=(x, args, )) for x in sublists]

    # 并行多进程处理
for p in processes:
p.start()

for p in processes:
p.join()

print('\nDone!')

if __name__ == '__main__':
main()

为了排版方便，并没有很遵守Python的规范（PEP8）。注意到除了前面提的三种类型的变化，还增加了镜像变化，这主要是因为这种变换太简单了，顺手就写上了。还有默认进程数用的是cpu_count()函数，这个获取的是cpu的核数。把这段代码保存为run_augmentation.py，然后在命令行输入：

>> python run_augmentation.py -h

或者

>> python run_augmentation.py --help

就能看到脚本的使用方法，每个参数的含义，还有默认值。接下里来执行一个图片增加任务：

>> python run_augmentation.py imagenet_samples more_samples 1000 --rotate_angle_vari 180 --p_rotate_crop 0.5

其中imagenet_samples为一些从imagenet图片url中随机下载的一些图片，--rotate_angle_vari设为180方便测试全方向的旋转，--p_rotate_crop设置为0.5，让旋转裁剪对一半图片生效。扰动增加后的1000张图片在more_samples文件夹下，得到的部分结果如下：

6.4 用OpenCV实现数据标注小工具

除了对图像的处理，OpenCV的图形用户界面（Graphical User Interface, GUI）和绘图等相关功能也是很有用的功能，无论是可视化，图像调试还是我们这节要实现的标注任务，都可以有所帮助。这节先介绍OpenCV窗口的最基本使用和交互，然后基于这些基础和之前的知识实现一个用于物体检测任务标注的小工具。

6.4.1OpenCV窗口循环

OpenCV显示一幅图片的函数是cv2.imshow()，第一个参数是显示图片的窗口名称，第二个参数是图片的array。不过如果直接执行这个函数的话，什么都不会发生，因为这个函数得配合cv2.waitKey()一起使用。cv2.waitKey()指定当前的窗口显示要持续的毫秒数，比如cv2.waitKey(1000)就是显示一秒，然后窗口就关闭了。比较特殊的是cv2.waitKey(0)，并不是显示0毫秒的意思，而是一直显示，直到有键盘上的按键被按下，或者鼠标点击了窗口的小叉子才关闭。cv2.waitKey()的默认参数就是0，所以对于图像展示的场景，cv2.waitKey()或者cv2.waitKey(0)是最常用的：

import cv2

img = cv2.imread('Aitutaki.png')
cv2.imshow('Honeymoon Island', img)
cv2.waitKey()

执行这段代码得到如下窗口：

cv2.waitKey()参数不为零的时候则可以和循环结合产生动态画面，比如在6.2.4的延时小例子中，我们把延时摄影保存下来的所有图像放到一个叫做frames的文件夹下。下面代码从frames的文件夹下读取所有图片并以24的帧率在窗口中显示成动画：

import os
from itertools import cycle
import cv2

# 列出frames文件夹下的所有图片
filenames = os.listdir('frames')

# 通过itertools.cycle生成一个无限循环的迭代器，每次迭代都输出下一张图像对象
img_iter = cycle([cv2.imread(os.sep.join(['frames', x])) for x in filenames])

key = 0
while key & 0xFF != 27:
cv2.imshow('Animation', next(img_iter))
key = cv2.waitKey(42)

在这个例子中我们采用了Python的itertools模块中的cycle函数，这个函数可以把一个可遍历结构编程一个无限循环的迭代器。另外从这个例子中我们还发现，cv2.waitKey()返回的就是键盘上出发的按键。对于字母就是ascii码，特殊按键比如上下左右等，则对应特殊的值，其实这就是键盘事件的最基本用法。

6.4.2鼠标和键盘事件

因为GUI总是交互的，所以鼠标和键盘事件基本使用必不可少，上节已经提到了cv2.waitKey()就是获取键盘消息的最基本方法。比如下面这段循环代码就能够获取键盘上按下的按键，并在终端输出：

while key != 27:
cv2.imshow('Honeymoon Island', img)
key = cv2.waitKey()
# 如果获取的键值小于256则作为ascii码输出对应字符，否则直接输出值
msg = '{} is pressed'.format(chr(key) if key < 256 else key)
print(msg)

通过这个程序我们能获取一些常用特殊按键的值，比如在笔者用的机器上，四个方向的按键和删除键对应的值如下：

- 上（↑）：65362

- 下（↓）：65364

- 左（←）：65361

- 右（→）：65363

- 删除（Delete）：65535

需要注意的是在不同的操作系统里这些值可能是不一样的。鼠标事件比起键盘事件稍微复杂一点点，需要定义一个回调函数，然后把回调函数和一个指定名称的窗口绑定，这样只要鼠标位于画面区域内的事件就都能捕捉到。把下面这段代码插入到上段代码的while之前，就能获取当前鼠标的位置和动作并输出：

# 定义鼠标事件回调函数
def on_mouse(event, x, y, flags, param):

# 鼠标左键按下，抬起，双击
if event == cv2.EVENT_LBUTTONDOWN:
print('Left button down at ({}, {})'.format(x, y))
elif event == cv2.EVENT_LBUTTONUP:
print('Left button up at ({}, {})'.format(x, y))
elif event == cv2.EVENT_LBUTTONDBLCLK:
print('Left button double clicked at ({}, {})'.format(x, y))

# 鼠标右键按下，抬起，双击
elif event == cv2.EVENT_RBUTTONDOWN:
print('Right button down at ({}, {})'.format(x, y))
elif event == cv2.EVENT_RBUTTONUP:
print('Right button up at ({}, {})'.format(x, y))
elif event == cv2.EVENT_RBUTTONDBLCLK:
print('Right button double clicked at ({}, {})'.format(x, y))

# 鼠标中/滚轮键（如果有的话）按下，抬起，双击
elif event == cv2.EVENT_MBUTTONDOWN:
print('Middle button down at ({}, {})'.format(x, y))
elif event == cv2.EVENT_MBUTTONUP:
print('Middle button up at ({}, {})'.format(x, y))
elif event == cv2.EVENT_MBUTTONDBLCLK:
print('Middle button double clicked at ({}, {})'.format(x, y))

# 鼠标移动
elif event == cv2.EVENT_MOUSEMOVE:
print('Moving at ({}, {})'.format(x, y))

# 为指定的窗口绑定自定义的回调函数
cv2.namedWindow('Honeymoon Island')
cv2.setMouseCallback('Honeymoon Island', on_mouse)

6.4.3代码：物体检测标注的小工具

基于上面两小节的基本使用，就能和OpenCV的基本绘图功能就能实现一个超级简单的物体框标注小工具了。基本思路是对要标注的图像建立一个窗口循环，然后每次循环的时候对图像进行一次拷贝。鼠标在画面上画框的操作，以及已经画好的框的相关信息在全局变量中保存，并且在每个循环中根据这些信息，在拷贝的图像上再画一遍，然后显示这份拷贝的图像。

基于这种实现思路，使用上我们采用一个尽量简化的设计：

- 输入是一个文件夹，下面包含了所有要标注物体框的图片。如果图片中标注了物体，则生成一个相同名称加额外后缀名的文件保存标注信息。

- 标注的方式是按下鼠标左键选择物体框的左上角，松开鼠标左键选择物体框的右下角，鼠标右键删除上一个标注好的物体框。所有待标注物体的类别，和标注框颜色由用户自定义，如果没有定义则默认只标注一种物体，定义该物体名称叫“Object”。

- 方向键的←和→用来遍历图片，↑和↓用来选择当前要标注的物体，Delete键删除一张图片和对应的标注信息。

每张图片的标注信息，以及自定义标注物体和颜色的信息，用一个元组表示，第一个元素是物体名字，第二个元素是代表BGR颜色的tuple或者是代表标注框坐标的元组。对于这种并不复杂复杂的数据结构，我们直接利用Python的repr()函数，把数据结构保存成机器可读的字符串放到文件里，读取的时候用eval()函数就能直接获得数据。这样的方便之处在于不需要单独写个格式解析器。如果需要可以在此基础上再编写一个转换工具就能够转换成常见的Pascal VOC的标注格式或是其他的自定义格式。

在这些思路和设计下，我们定义标注信息文件的格式的例子如下：

('Hill', ((221, 163), (741, 291)))
('Horse', ((465, 430), (613, 570)))

元组中第一项是物体名称，第二项是标注框左上角和右下角的坐标。这里之所以不把标注信息的数据直接用pickle保存，是因为数据本身不会很复杂，直接保存还有更好的可读性。自定义标注物体和对应标注框颜色的格式也类似，不过更简单些，因为括号可以不写，具体如下：

'Horse', (255, 255, 0)
'Hill', (0, 255, 255)
'DiaoSi', (0, 0, 255)

第一项是物体名称，第二项是物体框的颜色。使用的时候把自己定义好的内容放到一个文本里，然后保存成和待标注文件夹同名，后缀名为labels的文件。比如我们在一个叫samples的文件夹下放上一些草原的照片，然后自定义一个samples.labels的文本文件。把上段代码的内容放进去，就定义了小山头的框为黄色，骏马的框为青色，以及红色的屌丝。基于以上，标注小工具的代码如下：

import os
import cv2

# tkinter是Python内置的简单GUI库，实现一些比如打开文件夹，确认删除等操作十分方便
from tkFileDialog import askdirectory
from tkMessageBox import askyesno

# 定义标注窗口的默认名称
WINDOW_NAME = 'Simple Bounding Box Labeling Tool'

# 定义画面刷新的大概帧率（是否能达到取决于电脑性能）
FPS = 24

# 定义支持的图像格式
SUPPOTED_FORMATS = ['jpg', 'jpeg', 'png']

# 定义默认物体框的名字为Object，颜色蓝色，当没有用户自定义物体时用默认物体
DEFAULT_COLOR = {'Object': (255, 0, 0)}

# 定义灰色，用于信息显示的背景和未定义物体框的显示
COLOR_GRAY = (192, 192, 192)

# 在图像下方多出BAR_HEIGHT这么多像素的区域用于显示文件名和当前标注物体等信息
BAR_HEIGHT = 16

# 上下左右，ESC及删除键对应的cv.waitKey()的返回值
# 注意这个值根据操作系统不同有不同，可以通过6.4.2中的代码获取
KEY_UP = 65362
KEY_DOWN = 65364
KEY_LEFT = 65361
KEY_RIGHT = 65363
KEY_ESC = 27
KEY_DELETE = 65535

# 空键用于默认循环
KEY_EMPTY = 0

get_bbox_name = '{}.bbox'.format

# 定义物体框标注工具类
class SimpleBBoxLabeling:

def __init__(self, data_dir, fps=FPS, window_name=None):
self._data_dir = data_dir
self.fps = fps
self.window_name = window_name if window_name else WINDOW_NAME

#pt0是正在画的左上角坐标，pt1是鼠标所在坐标
self._pt0 = None
self._pt1 = None

# 表明当前是否正在画框的状态标记
self._drawing = False

# 当前标注物体的名称
self._cur_label = None

# 当前图像对应的所有已标注框
self._bboxes = []

# 如果有用户自定义的标注信息则读取，否则用默认的物体和颜色
label_path = '{}.labels'.format(self._data_dir)
self.label_colors = DEFAULT_COLOR if not os.path.exists(label_path) else self.load_labels(label_path)

# 获取已经标注的文件列表和还未标注的文件列表
imagefiles = [x for x in os.listdir(self._data_dir) if x[x.rfind('.') + 1:].lower() in SUPPOTED_FORMATS]
labeled = [x for x in imagefiles if os.path.exists(get_bbox_name(x))]
to_be_labeled = [x for x in imagefiles if x not in labeled]

# 每次打开一个文件夹，都自动从还未标注的第一张开始
self._filelist = labeled + to_be_labeled
self._index = len(labeled)
if self._index > len(self._filelist) - 1:
self._index = len(self._filelist) - 1

# 鼠标回调函数
def _mouse_ops(self, event, x, y, flags, param):

# 按下左键时，坐标为左上角，同时表明开始画框，改变drawing标记为True
if event == cv2.EVENT_LBUTTONDOWN:
self._drawing = True
self._pt0 = (x, y)

# 左键抬起，表明当前框画完了，坐标记为右下角，并保存，同时改变drawing标记为False
elif event == cv2.EVENT_LBUTTONUP:
self._drawing = False
self._pt1 = (x, y)
self._bboxes.append((self._cur_label, (self._pt0, self._pt1)))

# 实时更新右下角坐标方便画框
elif event == cv2.EVENT_MOUSEMOVE:
self._pt1 = (x, y)

# 鼠标右键删除最近画好的框
elif event == cv2.EVENT_RBUTTONUP:
if self._bboxes:
self._bboxes.pop()

# 清除所有标注框和当前状态
def _clean_bbox(self):
self._pt0 = None
self._pt1 = None
self._drawing = False
self._bboxes = []

# 画标注框和当前信息的函数
def _draw_bbox(self, img):

# 在图像下方多出BAR_HEIGHT这么多像素的区域用于显示文件名和当前标注物体等信息
h, w = img.shape[:2]
canvas = cv2.copyMakeBorder(img, 0, BAR_HEIGHT, 0, 0, cv2.BORDER_CONSTANT, value=COLOR_GRAY)

# 正在标注的物体信息，如果鼠标左键已经按下，则显示两个点坐标，否则显示当前待标注物体的名称
label_msg = '{}: {}, {}'.format(self._cur_label, self._pt0, self._pt1) \
if self._drawing \
else 'Current label: {}'.format(self._cur_label)

# 显示当前文件名，文件个数信息
msg = '{}/{}: {} | {}'.format(self._index + 1, len(self._filelist), self._filelist[self._index], label_msg)
cv2.putText(canvas, msg, (1, h+12),
cv2.FONT_HERSHEY_SIMPLEX,
0.5, (0, 0, 0), 1)

# 画出已经标好的框和对应名字
for label, (bpt0, bpt1) in self._bboxes:
label_color = self.label_colors[label] if label in self.label_colors else COLOR_GRAY
cv2.rectangle(canvas, bpt0, bpt1, label_color, thickness=2)
cv2.putText(canvas, label, (bpt0[0]+3, bpt0[1]+15),
cv2.FONT_HERSHEY_SIMPLEX,
0.5, label_color, 2)

# 画正在标注的框和对应名字
if self._drawing:
label_color = self.label_colors[self._cur_label] if self._cur_label in self.label_colors else COLOR_GRAY
if self._pt1[0] >= self._pt0[0] and self._pt1[1] >= self._pt0[1]:
cv2.rectangle(canvas, self._pt0, self._pt1, label_color, thickness=2)
cv2.putText(canvas, self._cur_label, (self._pt0[0] + 3, self._pt0[1] + 15),
cv2.FONT_HERSHEY_SIMPLEX,
0.5, label_color, 2)
return canvas

# 利用repr()导出标注框数据到文件
@staticmethod
def export_bbox(filepath, bboxes):
if bboxes:
with open(filepath, 'w') as f:
for bbox in bboxes:
line = repr(bbox) + '\n'
f.write(line)
elif os.path.exists(filepath):
os.remove(filepath)

# 利用eval()读取标注框字符串到数据
@staticmethod
def load_bbox(filepath):
bboxes = []
with open(filepath, 'r') as f:
line = f.readline().rstrip()
while line:
bboxes.append(eval(line))
line = f.readline().rstrip()
return bboxes

# 利用eval()读取物体及对应颜色信息到数据
@staticmethod
def load_labels(filepath):
label_colors = {}
with open(filepath, 'r') as f:
line = f.readline().rstrip()
while line:
label, color = eval(line)
label_colors[label] = color
line = f.readline().rstrip()
return label_colors

# 读取图像文件和对应标注框信息（如果有的话）
@staticmethod
def load_sample(filepath):
img = cv2.imread(filepath)
bbox_filepath = get_bbox_name(filepath)
bboxes = []
if os.path.exists(bbox_filepath):
bboxes = SimpleBBoxLabeling.load_bbox(bbox_filepath)
return img, bboxes

# 导出当前标注框信息并清空
def _export_n_clean_bbox(self):
bbox_filepath = os.sep.join([self._data_dir, get_bbox_name(self._filelist[self._index])])
self.export_bbox(bbox_filepath, self._bboxes)
self._clean_bbox()

# 删除当前样本和对应的标注框信息
def _delete_current_sample(self):
filename = self._filelist[self._index]
filepath = os.sep.join([self._data_dir, filename])
if os.path.exists(filepath):
os.remove(filepath)
filepath = get_bbox_name(filepath)
if os.path.exists(filepath):
os.remove(filepath)
self._filelist.pop(self._index)
print('{} is deleted!'.format(filename))

# 开始OpenCV窗口循环的方法，定义了程序的主逻辑
def start(self):

     # 之前标注的文件名，用于程序判断是否需要执行一次图像读取
last_filename = ''

# 标注物体在列表中的下标
label_index = 0

# 所有标注物体名称的列表
labels = self.label_colors.keys()

# 待标注物体的种类数
n_labels = len(labels)

# 定义窗口和鼠标回调
cv2.namedWindow(self.window_name)
cv2.setMouseCallback(self.window_name, self._mouse_ops)
key = KEY_EMPTY

# 定义每次循环的持续时间
delay = int(1000 / FPS)

# 只要没有按下Esc键，就持续循环
while key != KEY_ESC:

# 上下键用于选择当前标注物体
if key == KEY_UP:
if label_index == 0:
pass
else:
label_index -= 1

elif key == KEY_DOWN:
if label_index == n_labels - 1:
pass
else:
label_index += 1

# 左右键切换当前标注的图片
elif key == KEY_LEFT:
# 已经到了第一张图片的话就不需要清空上一张
if self._index > 0:
self._export_n_clean_bbox()

self._index -= 1
if self._index < 0:
self._index = 0

elif key == KEY_RIGHT:
# 已经到了最后一张图片的话就不需要清空上一张
if self._index < len(self._filelist) - 1:
self._export_n_clean_bbox()

self._index += 1
if self._index > len(self._filelist) - 1:
self._index = len(self._filelist) - 1

# 删除当前图片和对应标注信息
elif key == KEY_DELETE:
if askyesno('Delete Sample', 'Are you sure?'):
self._delete_current_sample()
key = KEY_EMPTY
continue

# 如果键盘操作执行了换图片，则重新读取，更新图片
filename = self._filelist[self._index]
if filename != last_filename:
filepath = os.sep.join([self._data_dir, filename])
img, self._bboxes = self.load_sample(filepath)

# 更新当前标注物体名称
self._cur_label = labels[label_index]

# 把标注和相关信息画在图片上并显示指定的时间
canvas = self._draw_bbox(img)
cv2.imshow(self.window_name, canvas)
key = cv2.waitKey(delay)

# 当前文件名就是下次循环的老文件名
last_filename = filename

print('Finished!')

cv2.destroyAllWindows()
# 如果退出程序，需要对当前进行保存
self.export_bbox(os.sep.join([self._data_dir, get_bbox_name(filename)]), self._bboxes)

print('Labels updated!')

if __name__ == '__main__':
dir_with_images = askdirectory(title='Where are the images?')
labeling_task = SimpleBBoxLabeling(dir_with_images)
labeling_task.start()

需要注意的是几个比较通用且独立的方法前加上了一句@staticmethod，表明是个静态方法。执行这个程序，并选择samples文件夹，标注时的画面如下图：