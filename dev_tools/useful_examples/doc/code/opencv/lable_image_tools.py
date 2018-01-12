    
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