#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setMaximumSize(324,118);
    this->setMinimumSize(324,118);

    QObject::connect(ui->inputBox,SIGNAL(returnPressed()), this, SLOT(on_submitButton_clicked()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_submitButton_clicked()
{
//    QProcess *myProcess = new QProcess(parent);
//    myProcess->start(program, arguments);
    QProcess * process = new QProcess();
    QString startProgram = ui->inputBox->text();
    process->start(startProgram.trimmed());

//    清空命令行内容
    ui->inputBox->clear();

//    关闭当前程序窗口
    this->close();
}
