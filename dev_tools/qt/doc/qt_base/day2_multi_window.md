# QDialog

QDialog 运行 exec() 函数会显示对话框， 如果运行了 accept() 函数, 那么返回值应该是 Accepted.  

~~~cpp
...
LoginDlg dlg;                        // 建立自己新建的LoginDlg类的实例dlg
if(dlg.exec() == QDialog::Accepted) // 利用Accepted返回值判断按钮是否被按下
{
    w.show();                      // 如果被按下，显示主窗口
    return a.exec();              // 程序一直执行，直到主窗口关闭
}
...
~~~


## Login  


如果你的信号只是简单的触发一个窗体的动作, 那么直接使用信号连接到窗体动作对应的槽函数; 如果你想触发的除了窗体的动作之外, 还有别的动作, 那么应该自己实现槽函数, 在完成了自定义动作后显式调用窗体动作的槽函数.  

## QMessageBox

注意还要添加该类的头文件包含，即：#include <QMessageBox>   

~~~cpp
QMessageBox::warning(   this, 
                        tr("警告！"),
                        tr("用户名或密码错误！"),
                        QMessageBox::Ok );
~~~

Qt 中的 QMessageBox 类提供了多种常用的对话框类型， 比如这里的警告对话框，提示对话框、问题对话框等。  

这里使用了静态函数来设置了一个警告对话框， 这种方式很便捷， 其中的参数依次是：this 表明父窗口是登录对话框、窗口标题、界面显示的内容和最后要显示的按钮， 这里使用了一个 Ok 按钮。   



## LineEdit  

1) 隐藏输入密码 

对于输入的密码，我们常见的是显示成小黑点的样式。   

点击 logindialog.ui 文件进入设计模式， 然后选中界面上的密码行编辑器，在属性编辑器中将 echoMode 属性选择为 Password。  

当然，除了在属性编辑器中进行更改，也可以在loginDialog类的构造函数中使用setEchoMode(QLineEdit::Password)函数来设置。  

2) 设置行编辑器的文本提示信息   

在行编辑器的属性栏中还可以设置占位符， 就是没有输入信息前的一些提示语句。  

例如将密码行编辑器的 placeholderText 属性更改为“请输入密码”，将用户名行编辑器的更改为“请输入用户名”，运行效果如下图所示。  

3) 去除行编辑器中的空格  

对于行编辑器，还有一个问题就是，比如我们输入用户名，在前面添加了一个空格，这样也可以保证输入是正确的，这个可以使用QString类的trimmed()函数来实现，它可以去除字符串前后的空白字符。  

4) 清空行编辑器的内容  

~~~cpp
ui->usrLineEdit->clear();
ui->usrLineEdit->setFocus();
~~~


## 代码中手动关联信号和槽函数  

下面来看下怎么使用代码自定义槽，然后手动进行关联。   


