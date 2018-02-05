#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QColor>

#include <QMouseEvent>
#include <iostream>


#include <opencv2/opencv.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void mouseMoveEvent(QMouseEvent *ev);    // QLabel 鼠标移动事件
    void mousePressEvent(QMouseEvent *ev);   // QLabel 鼠标按键事件
    void keyPressEvent(QKeyEvent *ev);       // 键盘事件

private:
    Ui::MainWindow *ui;
    // 初始化参数
    bool initalized_;
    QString dataSetName_;
    QImage img_;
    QImage cacheLine_;
    QImage cacheRect_;

    // imageLabel 的光标
    int cursorX_;
    int cursorY_;
    // init once
//    QPainter painter;
    bool secondLeftButtonClick_;
    bool finishOne_;

    // Rect 的角点
    QPoint leftUpConner_;
    QPoint rightBottomConner_;

    // 所有图片的文件名
    QStringList fileNames_;
    // 当前标注进度
    int index_;

    void refreshWindow(int i);


private slots:
    void getDataSetNameSlot();
    void showImageSlot();
};

#endif // MAINWINDOW_H
