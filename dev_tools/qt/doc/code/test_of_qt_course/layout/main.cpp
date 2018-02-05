#include "widget.h"
#include <QApplication>
/**
 * [功能]: 启动一个外部应用程序
 *   1. 打开一个程序之后清空文本输入框
 *   2. 输入完成后按回车让程序执行
 *   3. 新的程序窗口打开之后,主窗体消失
 *   4. 使用 F4 中的内置信号和槽函数
 *
 * [程序组件]:
 *   1. QProcess
 *   2. QString 的去空格方法
 *   3. 获取文本框中的文本
 *   4. 信号和槽连接函数
 *
*/


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("runCmd");
    w.show();

    return a.exec();
}
