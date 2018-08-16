#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::about)
{
    ui->setupUi(this);

    // 显示 movie
    this->movie = new QMovie("/home/klm/work/gitwork/work_note/dev_tools/qt/doc/code/test_of_qt_course/mynotepad/images/cool.gif");
    ui->movieLabel->setMovie(this->movie);
    this->movie->start();
    // 按钮控制
    QObject::connect(ui->startButton, SIGNAL(clicked(bool)), this, SLOT(startMovieSlot()));
    QObject::connect(ui->stopButton, SIGNAL(clicked(bool)), this, SLOT(stopMovieSlot()));
}

about::~about()
{
    delete ui;
    delete movie;
}

void about::startMovieSlot()
{
    this->movie->start();
}

void about::stopMovieSlot()
{
    this->movie->stop();
}
