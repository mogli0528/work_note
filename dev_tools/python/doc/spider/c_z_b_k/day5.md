# 机器视觉与 Tesseract 介绍  

将图像翻译成文字一般被称为光学文字识别(Optical Character Recognition, OCR)。可以实现OCR 的底层库并不多, 目前很多库都是使用共同的几个底层 OCR 库, 或者是其上进行定制。  

## ORC 库概述

在读取和处理图像、图像相关的机器学习以及创建图像等任务中，Python 一直都是非常出色的语言。虽然有很多库可以进行图像处理，但在这里我们只重点介绍： Tesseract.   

## Tesseract  

Tesseract 是一个 OCR 库,目前由 Google 赞助(Google 也是一家以 OCR 和机器学习技术闻名于世的公司)。Tesseract 是目前公认最优秀、最精确的开源 OCR 系统。 除了极高的精确度,Tesseract 也具有很高的灵活性。它可以通过训练识别出任何字体，也可以识别出任何 Unicode 字符。

### 1 安装 Tesseract   

1) Windows 系统  

下载可执行安装文件 https://code.google.com/p/tesseract-ocr/downloads/list 安装。  

2) Linux 系统  

可以通过 apt-get 安装:  

~~~bash
$sudo apt-get tesseract-ocr   
~~~

Mac OS X 系统   

用 Homebrew(http://brew.sh/) 等第三方库可以很方便地安装:   

~~~bash
brew install tesseract
~~~

要使用 Tesseract 的功能，比如后面的示例中训练程序识别字母，要先在系统中设置一 个新的环境变量 $TESSDATA_PREFIX，让 Tesseract 知道训练的数据文件存储在哪里，然后搞一份tessdata数据文件，放到Tesseract目录下。
在大多数 Linux 系统和 Mac OS X 系统上,你可以这么设置: $export TESSDATA_PREFIX=/usr/local/share/Tesseract

在 Windows 系统上也类似,你可以通过下面这行命令设置环境变量: #setx TESSDATA_PREFIX C:\Program Files\Tesseract OCR\Tesseract

### 2. 安装 pytesseract   

Tesseract 是一个 Python 的命令行工具，不是通过 import 语句导入的库。安装之后,要用 tesseract 命令在 Python 的外面运行，但我们可以通过 pip 安装支持 Python 版本的 Tesseract 库：   

~~~bash
pip install pytesseract
~~~