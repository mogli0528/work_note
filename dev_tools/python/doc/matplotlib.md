# python matplot 图像绘制库的使用    
## 1. 绘制简单的折线图    
1.1 线型   

|character|description|character|description|
|---------|--------|---------|--------
|``'-'``  |solid line style|``'--'`` |dashed line style|
|``'-.'`` |dash-dot line style|``':'``  |dotted line style|
|``'.'``  |point marker|``','``  |pixel marker|
|``'o'``  |circle marker|``'v'``  |triangle_down marker|
|``'^'``  |triangle_up marker|``'<'``  |triangle_left marker|
|``'>'``  |triangle_right marker|``'1'``  |tri_down marker|
|``'2'``  |tri_up marker|``'3'``  |tri_left marker|
|``'4'``  |tri_right marker|``'s'``  |square marker|
|``'p'``  |pentagon marker|``'*'``  |star marker|
|``'h'``  |hexagon1 marker|``'H'``  |hexagon2 marker|
|``'+'``  |plus marker|``'x'``  |x marker|
|``'D'``  |diamond marker|``'d'``  |thin_diamond marker|
|``'|'``  |vline marker|``'_'``  |hline marker|

1.2 颜色
|character  | color|character  | color|
|---------|-------|---------|--------
|'b'      |  blue|'g'      |  green|
|'r'      |  red|'c'      |  cyan|
|'m'      |  magenta|'y'      |  yellow|
|'k'      |  black|'w'      |  white|

1.3 示例代码   
```python
import matplotlib.pyplot as plt

input_values = list(range(1,101))
squares = [x**2 for x in input_values]
# 修改标签文字和线条粗细
plt.plot(input_values, squares, 'go', label='line 1', linewidth = 5)
# 设置图标标题,并给坐标轴加上标签
plt.title('squares numbers', fontsize=14)
plt.xlabel('Value', fontsize=14)
plt.ylabel('squares of values', fontsize=14)
# 设置刻度标记的大小-labelsize
plt.tick_params(axis='both', labelsize=14)
# 绘制坐标轴
plt.axis([0,110, 0, 11000])

plt.show()

```
## 2. scatter()绘制散点图   
```python
import matplotlib.pyplot as plt

# 给定x,y 绘制这个离散点
input_values = list(range(1,101))
squares = [x**2 for x in input_values]

# 删除数据点的轮廓: edgecolor = 'none'
# 设置数据点的颜色为red: c = 'r', c = 'red',c = (0, 0, 0.8)-[rgb],
plt.scatter(input_values, squares, c = (0, 0, 0.8), s = 20, edgecolor = 'none')

plt.title("Scatter numbers", fontsize = 14)
plt.xlabel("x values", fontsize = 14)
plt.ylabel("y values", fontsize = 14)

plt.tick_params(axis='both', labelsize=14)

plt.axis([0,110, 0, 11000])

plt.show()
```
## 3. 使用颜色映射   
用较浅的颜色表示较小的值,用较深的颜色表示较大的值.    
```python
import matplotlib.pyplot as plt

input_values = list(range(1,101))
squares = [x**2 for x in input_values]

# 将参数 c 设置为 纵坐标squares的列表   
# 并使用 cmap 告诉 pyplot 使用哪个颜色映射   
plt.scatter(input_values, squares, c = squares, \
            cmap = plt.cm.Reds, s = 20, edgecolor = 'none')

plt.title("Scatter numbers", fontsize = 14)
plt.xlabel("x values", fontsize = 14)
plt.ylabel("y values", fontsize = 14)

plt.tick_params(axis='both', labelsize=14)

plt.axis([0,110, 0, 11000])

plt.show()
```
## 4. 自动保存图表   
`plt.show() ->  plt.savefig()`   
```python
import matplotlib.pyplot as plt

input_values = list(range(1,101))
squares = [x**2 for x in input_values]

# 将参数 c 设置为 纵坐标squares的列表   
# 并使用 cmap 告诉 pyplot 使用哪个颜色映射   
plt.scatter(input_values, squares, c = squares, \
            cmap = plt.cm.Reds, s = 20, edgecolor = 'none')

plt.title("Scatter numbers", fontsize = 14)
plt.xlabel("x values", fontsize = 14)
plt.ylabel("y values", fontsize = 14)

plt.tick_params(axis='both', labelsize=14)

plt.axis([0,110, 0, 11000])

# 第一个参数: 保存的文件名
# 第二个参数: 将图表多余的空白区域裁剪掉 
plt.savefig('squares.png', bbox_inches ='tight')
```
