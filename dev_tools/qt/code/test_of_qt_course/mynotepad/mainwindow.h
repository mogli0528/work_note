#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QDebug>
// 文件打开保存
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
// 时间日期
#include <QDateTime>

// 设置字体和颜色
#include <QFontDialog>
#include <QFont>
#include <QColorDialog>
#include <QColor>

#include <QUrl>
#include <QDesktopServices>

#include <about.h>

#include <QSplashScreen>
#include <QPixmap>
#include <QLabel>

#include <QIcon>
#include <QCloseEvent>

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
    void closeEvent(QCloseEvent *event);

private:
    QString saveFileName;
    Ui::MainWindow *ui;

private slots:
    void newFileSlot();
    void openFileSlot();
    void saveFileSlot();
    void saveAsFileSlot();
    void printFileSlot();

    void currentDateTimeSlot();

    void setColorSlot();
    void setFontSlot();

    void accessNotepadWebSlot();

    void aboutNotepadSlot();
};

#endif // MAINWINDOW_H
