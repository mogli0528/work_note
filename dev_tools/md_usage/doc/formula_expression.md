#  MarkDown 中插入公式  

> `$ formula $`会使 `formula` 嵌入到文本行中; `$$ formula $$`会使 `formula` 单独占用一行并居中.    

## 变量表示  
用 `$var$` 即可表示 $var$ 变量.     

## 上标表示    
1. html 格式    
上标的 HTML 标签的是<sup>，所以如果要打上标的话就用以下格式：    
<sup>xxx</sup>
其中 `xxx` 表示上标的内容，看个例子：    
n<sup>2</sup>=n+1    
n 的平方等于 n+1.     

2. Latex 格式    
用 `^` 后的内容表示上标. 例如:    

$x^{(i)}$, $y^{(i)}$    

## 下标表示   
1. html 格式    
上标的 HTML 标签的是<sub>，所以如果要打下标的话就用以下格式：    
<sub>xxx</sub>
其中 `xxx` 表示下标的内容，看个例子：    

n<sub>2</sub>=n+1    

2. Latex 格式  
用 `_` 后的内容表示上标. 例如:    

$x_{(i)}$, $y_{(i)}$    

## 分数形式   

$$ \frac{1}{2} \tfrac{1}{2} $$     

`\dfrac{}{}` 会显示大号的分式, 而 `\tfrac{}{}` 则显示小号的分式.    


## 条件偏导数    

条件偏导   

$\left.\frac{\partial f(x,y)}{\partial x}\right|_{x=0}$    

## 空白间距

$$ \, 3/18cm   \:  4/18cm  \; 5/18cm \quad 1cm \qquad 2m ! -3/18cm $$    

## 多行公式

长公式

无需对齐可使用 multline, 需要对齐使用 split，用 \\ 和 & 来分行和设置对齐的位置.   

$$ \begin{multline}
    x = a+b+c+{} \\
        d+e+f+g
    \end{multline}  $$

$$ 
\begin{split}
x = {} & a + b + c +{}\\
	&d + e + f + g
\end{split}
$$

## 公式组

不需要对齐的公式组用 gather，需要对齐使用 align:        
$$ \begin{gather} a = b+c+d\\ x=y+z \end{gather} $$ 
 
$$
\begin{align}
a &=b+c+d \\
x &=y+z
\end{align}$$

## 分支公式

分段函数通常用 cases 次环境携程分支公式:   
$$
y=\begin{cases}
-x,\quad x\leq 0\\
x, \quad x>0
\end{cases}$$