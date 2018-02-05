#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setWindowIcon(QIcon(":/new/icon/images/notepad.ico"));
    ui->setupUi(this);
    this->setWindowTitle("Untitled    mynotepad.");
    QObject::connect(ui->actionNew, SIGNAL(triggered(bool)), this, SLOT(newFileSlot()));
    QObject::connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFileSlot()));
    QObject::connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(saveFileSlot()));
    QObject::connect(ui->actionSave_As, SIGNAL(triggered(bool)), this, SLOT(saveAsFileSlot()));
    QObject::connect(ui->actionPrint, SIGNAL(triggered(bool)), this, SLOT(printFileSlot()));

    QObject::connect(ui->actionFont, SIGNAL(triggered(bool)), this, SLOT(setFontSlot()));
    QObject::connect(ui->actionColor, SIGNAL(triggered(bool)), this, SLOT(setColorSlot()));

    QObject::connect(ui->actionTime, SIGNAL(triggered(bool)), this, SLOT(currentDateTimeSlot()));

    // Edit
    QObject::connect(ui->actionCopy, SIGNAL(triggered(bool)), ui->textEdit, SLOT(copy()));
    QObject::connect(ui->actionCut, SIGNAL(triggered(bool)), ui->textEdit, SLOT(cut()));
    QObject::connect(ui->actionPaste, SIGNAL(triggered(bool)), ui->textEdit, SLOT(paste()));
    QObject::connect(ui->actionSelectAll, SIGNAL(triggered(bool)), ui->textEdit, SLOT(selectAll()));
    QObject::connect(ui->actionRedo, SIGNAL(triggered(bool)), ui->textEdit, SLOT(redo()));
    QObject::connect(ui->actionUndo, SIGNAL(triggered(bool)), ui->textEdit, SLOT(undo()));

    QObject::connect(ui->actionAbout_QT, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));

    QObject::connect(ui->actionUpdate, SIGNAL(triggered(bool)), this, SLOT(accessNotepadWebSlot()));
    QObject::connect(ui->actionAbout_NotePad, SIGNAL(triggered(bool)), this, SLOT(aboutNotepadSlot()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFileSlot()
{
    if (ui->textEdit->document()->isModified()) {
        qDebug() << "current file modified..." << endl;
    } else {
        ui->textEdit->clear();
        this->setWindowTitle("Untitled   mynotepad");
        qDebug() << "current file has no change..." << endl;
    }

}

void MainWindow::openFileSlot()
{
    saveFileName = QFileDialog::getOpenFileName(this,"OpenFile", QDir::currentPath());
    if(saveFileName.isEmpty()) {
        QMessageBox::information(this, "error", "Please Select a File..");
        return;
    }

    QFile *file  = new QFile();
    file->setFileName(saveFileName);
    bool retVal = file->open(QIODevice::ReadOnly);
    if(retVal) {
        // 文件和文本流关联
        QTextStream in(file);
        ui->textEdit->setText(in.readAll());
        this->setWindowTitle(saveFileName + "   mynotepad");

        file->close();
        delete file;
    } else {
        QMessageBox::information(this, "error", "open File error..");

    }
}

void MainWindow::saveFileSlot()
{
    if(saveFileName.isEmpty()){
        saveAsFileSlot();

    } else {
        QFile *file = new QFile();
        file->setFileName(saveFileName);
        bool retVal = file->open(QIODevice::WriteOnly);

        if(retVal) {
            QTextStream out(file);
            out << ui->textEdit->toPlainText();  // 取出 textEdit 中的纯文本内容
            this->setWindowTitle(saveFileName + "    mynotepad");

            file->close();
            delete file;
        }
    }
}


void MainWindow::saveAsFileSlot()
{
    // 不同于 getOpenFileName()
    saveFileName = QFileDialog::getSaveFileName(this,"Save File", QDir::currentPath());
    if(saveFileName.isEmpty()){
        QMessageBox::information(this, "error", "need to save as a file");
        return;
    }


    QFile *file = new QFile();
    file->setFileName(saveFileName);
    bool retVal = file->open(QIODevice::WriteOnly);

    if(retVal) {
        QTextStream out(file);
        out << ui->textEdit->toPlainText();  // 取出 textEdit 中的纯文本内容
        this->setWindowTitle(saveFileName + "    mynotepad");

        file->close();
        delete file;
    }
    else
        QMessageBox::information(this, "error", "Save File failed");

}

void MainWindow::printFileSlot()
{
    qDebug() << "Not Supported" << endl;
}

void MainWindow::setFontSlot()
{
    bool ok;
    QFont font = QFontDialog::getFont(
                    &ok, QFont("Helvetica [Cronyx]", 10), this);
    if (ok) {
        // the user clicked OK and font is set to the font the user selected
        ui->textEdit->setFont(font);
    } else {
        // the user canceled the dialog; font is set to the initial
        // value, in this case Helvetica [Cronyx], 10
    }
}

void MainWindow::setColorSlot()
{
    const QColor color = QColorDialog::getColor(Qt::black, this, "Select Color");

    if (color.isValid()) {
        ui->textEdit->setTextColor(color);
    }
}

void MainWindow::currentDateTimeSlot()
{
    QDateTime currentDT = QDateTime::currentDateTime();
    QString dateTime = currentDT.toString("yyyy-mm-dd hh:mm:ss");
    ui->textEdit->append(dateTime);
}

void MainWindow::accessNotepadWebSlot()
{
    QDesktopServices::openUrl(QUrl("https://www.baidu.com"));
}


void MainWindow::aboutNotepadSlot()
{
    about *dialog = new about();
    dialog->show(); // 显示一个非模态对话框
    // 显示模态对话框
    // about->exec();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    switch (ret) {
    case QMessageBox::Save:
        // Save was clicked
        if (ui->textEdit->document()->isModified()) {
            saveFileSlot();
        }
        break;
    case QMessageBox::Discard:
        // Don't Save was clicked
        event->accept();
        break;
    case QMessageBox::Cancel:
        // Cancel was clicked
        event->ignore();
        break;
    default:
        // should never be reached
        break;
    }

}




