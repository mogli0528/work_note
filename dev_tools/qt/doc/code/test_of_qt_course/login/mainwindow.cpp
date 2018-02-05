#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setMaximumSize(256, 177);
    this->setMinimumSize(256, 177);

    ui->passwdLineEdit->setEchoMode(QLineEdit::Password);
    QObject::connect(ui->loginButton, SIGNAL(clicked(bool)), this, SLOT(loginSlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loginSlot()
{
    if(ui->userNameLineEdit->text()=="klm" && ui->passwdLineEdit->text()== "while") {
        QMessageBox::information(this, "login", "登录成功");
    } else {
        QMessageBox::information(this, "login", "登录失败");
    }
}
