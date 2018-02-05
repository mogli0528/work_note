#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 清空文本输入框中的内容
    ui->fisrtValLineEdit->clear();
    ui->secondValLineEdit->clear();
    ui->resultLineEdit->clear();

    this->setMaximumSize(270, 170);
    this->setMinimumSize(270, 170);

    QObject::connect(ui->calcButton, SIGNAL(clicked()), this, SLOT(calcSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::calcSlot()
{
    int firstVal = ui->fisrtValLineEdit->text().toInt();
    int secondVal = ui->secondValLineEdit->text().toInt();
    int result = 0;

    int index = ui->operatorComboBox->currentIndex();

    switch (index) {
    case 0:
        result = firstVal + secondVal;
        break;
    case 1:
        result = firstVal - secondVal;
        break;
    case 2:
        result = firstVal * secondVal;
        break;
    case 3:
        if(secondVal == 0) {
            QMessageBox::warning(this, "error", "divider can't be zero!!!");
            return;
        }
        result = firstVal / secondVal;
        break;
    default:
        break;
    }
    // 设置显示文本
    ui->resultLineEdit->setText(QString::number(result));
    QMessageBox::information(this,"result", QString::number(result));

}
