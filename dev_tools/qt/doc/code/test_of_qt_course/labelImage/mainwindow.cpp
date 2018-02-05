#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    initalized_(false),
    dataSetName_(""),
    secondLeftButtonClick_(false),
    finishOne_(true),
    fileNames_(""),
    index_(0)
{
    ui->setupUi(this);

    QObject::connect(ui->loadDataSetPushButton, SIGNAL(clicked(bool)),this, SLOT(getDataSetNameSlot()));
    QObject::connect(ui->showPushButton, SIGNAL(clicked(bool)),this, SLOT(showImageSlot()));

//    ui->imageLabel->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getDataSetNameSlot()
{
    this->dataSetName_ = QFileDialog::getExistingDirectory(this,"Choose a data set directory", QDir::currentPath());

    if(this->dataSetName_.isEmpty()) {
        QMessageBox::information(this,"Error", "Invaild directory");
    }

    qDebug() << this->dataSetName_ << endl;
}

void MainWindow::showImageSlot()
{
    // 获取目录下的所有图片文件
//    QStringList QDir::entryList(const QStringList &nameFilters, Filters filters = NoFilter, SortFlags sort = NoSort) const
    QDir * dataSetName = new QDir(this->dataSetName_);
    if(!dataSetName->exists()) {
        qDebug() << "File Not Exists" << endl;
        return;
    }

    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.bmp";   // 设置过滤类型
    dataSetName->setNameFilters(filters);       // 设置文件名的过滤
    this->fileNames_ = dataSetName->entryList();
    delete dataSetName;

    int size = this->fileNames_.size();
    if(size == 0) {
        qDebug() << "error" << endl;
        return;
    }

    for(int index = 0; index < size; index++) {
        if(this->fileNames_.at(index) == "." || this->fileNames_.at(index) == "..") {
            continue;
        }

        qDebug() << this->fileNames_.at(index) << endl;

    }
    QString name = this->dataSetName_ + "/" + this->fileNames_.at(this->index_);

    this->img_ = QImage(name);
    this->cacheLine_ = this->img_;

    ui->imageLabel->clear();
    ui->imageLabel->setPixmap(QPixmap::fromImage(this->img_));
    ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
    ui->imageLabel->show();

#if 0
    QByteArray nameByte = name.toLatin1(); // must
    char * ch = nameByte.data();
    Mat image = imread("/home/klm/data/frame/c1192.jpg");
    if(!image.empty()) {

    }
        cv::cvtColor(image,image,CV_BGR2RGB);
        // RGB 通道转换
        this->img = QImage((const unsigned char*)(image.data), image.cols, image.rows,
                           image.cols*image.channels(),  QImage::Format_RGB888);

        ui->imageLabel->clear();
        ui->imageLabel->setPixmap(QPixmap::fromImage(this->img));
        ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
        ui->imageLabel->show();
    }
#endif
}

void MainWindow::mouseMoveEvent(QMouseEvent *ev)
{
    qDebug() << "pos move " << ev->pos().x() << ev->pos().y();
    if(!this->dataSetName_.isEmpty())
    {
        // TODO: 每标完一张图换一次
        this->cacheLine_ = this->img_;
        if(!this->cacheRect_.isNull()){
            qDebug() << " copy cacheRect_ ";
            this->cacheLine_ = this->cacheRect_;
        }

        QPainter painter(&(this->cacheLine_));
        painter.begin(this);

        painter.setPen(QColor(0,255,0));

        this->cursorX_ = ev->pos().x();
        this->cursorY_ = ev->pos().y() - ui->mainToolBar->geometry().height();

        // 绘制两条追踪线
        if(this->cursorX_ > 0
                && this->cursorX_ < ui->imageLabel->width()
                && this->cursorY_ > 0
                && this->cursorY_ < ui->imageLabel->height()
          )
        {
            painter.drawLine(std::max(this->cursorX_, 0), 0, std::max(this->cursorX_, 0), ui->imageLabel->height()-1);
            painter.drawLine(0, std::max(this->cursorY_, 0), ui->imageLabel->width()-1, std::max(this->cursorY_, 0));
            painter.end();
        }

        // 刷新屏幕
        if(this->finishOne_)
            this->refreshWindow(3);

        this->refreshWindow(2);
    }
}

void MainWindow::mousePressEvent(QMouseEvent *ev)
{
    if(!this->dataSetName_.isEmpty()){

        if(ev->button() == Qt::LeftButton){
            if(!this->initalized_ && this->finishOne_){
                this->cacheRect_ = this->img_;
                this->cacheLine_ = this->img_;
                this->finishOne_ = false;
                this->initalized_ = true;
            }

            QPainter painter(&(this->cacheRect_));
            painter.begin(this);

            QPen pen;  // creates a default pen
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(3);
            pen.setBrush(Qt::red);

            qDebug() << "Left " << ev->x() << ev->y();
            if(this->secondLeftButtonClick_) {
                this->secondLeftButtonClick_ = false;
                qDebug() << "Left 2 click " << ev->x() << ev->y();
                this->rightBottomConner_.setX(this->cursorX_);
                this->rightBottomConner_.setY(this->cursorY_);


//                pen.setCapStyle(Qt::RoundCap);
//                pen.setJoinStyle(Qt::RoundJoin);
//                painter.setPen(QColor(255, 0, 0));  // R,G,B

                painter.setPen(pen);
                painter.drawPoint(this->rightBottomConner_);
                painter.drawRect(QRect(this->leftUpConner_, this->rightBottomConner_));

                // TODO: 读入下一张图
                // ....
            } else {
                qDebug() << "Left 1 click " << ev->x() << ev->y();
                this->leftUpConner_.setX(this->cursorX_);
                this->leftUpConner_.setY(this->cursorY_);

                painter.setPen(pen);
                painter.drawPoint(this->leftUpConner_);
                painter.end();

                this->secondLeftButtonClick_ = true;
            }

        } else if (ev->button() == Qt::RightButton) {
            qDebug() << "Right " << ev->x() << ev->y();
        }

        // refresh
        this->refreshWindow(1);
    }
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
    if(!this->dataSetName_.isEmpty()){

        switch(ev->key()){
        case Qt::Key_S:
            qDebug() << "finish one ";
            this->finishOne_ = true;
            this->index_++;
            if(this->index_ == this->fileNames_.size()){
                QMessageBox::information(this,"Congratulation", "You have finished a dataset");
                this->index_ = 0;
                break;
            }
            this->img_ = QImage(this->dataSetName_ + "/" + this->fileNames_.at(this->index_));
            this->cacheLine_ = this->img_;
            this->cacheRect_ = this->img_;

            this->secondLeftButtonClick_ = false; // 丢弃之前的点
            refreshWindow(3);

            break;
        }

    }
}
void MainWindow::refreshWindow(int i)
{
    ui->imageLabel->clear();
    if(i == 1){
        ui->imageLabel->setPixmap(QPixmap::fromImage(this->cacheRect_));

    }else if(i == 2){
        ui->imageLabel->setPixmap(QPixmap::fromImage(this->cacheLine_));

    }else if(i == 3){
        ui->imageLabel->setPixmap(QPixmap::fromImage(this->img_));


    }
    ui->imageLabel->resize(ui->imageLabel->pixmap()->size());
    ui->imageLabel->show();
}

