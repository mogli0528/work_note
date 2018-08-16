#include "mainwindow.h"
#include <QApplication>
/**
 * [功能]: 实现一个简单的加法(四则运算)计算器
 *   1. 文本输入框中内容的获取 QlineEdit->text()
 *   2. 字符串到 int 的转换 QString->toInt()
 *   3. int 到字符串的转换 QString::number()
 *   4. 设置文本框的显示内容(setText()函数)
 *   5. 获取 QComboBox 中的 item(currentIndex())
 *
 * [程序组件]:
 *   1. 获取文本框中的文本
 *   2. 自己实现的 private 槽函数
 *   3. 元对象系统-MOC(Q_OBJECT)
 *   4. QMessageBox::warning() 和 QMessageBox::infomation() 用于消息框提示
 *   5.
*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
