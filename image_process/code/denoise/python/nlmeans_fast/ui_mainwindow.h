/********************************************************************************
** Form generated from reading UI file 'ui_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_UI_MAINWINDOW_H
#define UI_UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *loadButton;
    QPushButton *saveButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *toggleOriginalButton;
    QScrollArea *imageScrollarea;
    QWidget *scrollAreaWidgetContents;
    QTabWidget *tabWidget;
    QWidget *tab;
    QSlider *axSlider;
    QLabel *label;
    QSpinBox *axSpinbox;
    QLabel *label_2;
    QSpinBox *sxSpinbox;
    QSlider *sxSlider;
    QLabel *label_3;
    QDoubleSpinBox *aSpinbox;
    QSlider *aSlider;
    QLabel *label_4;
    QSlider *hSlider;
    QDoubleSpinBox *hSpinbox;
    QLabel *label_5;
    QScrollArea *maskScrollarea;
    QWidget *scrollAreaWidgetContents_2;
    QLabel *label_6;
    QSpinBox *timeSpinbox;
    QLabel *label_7;
    QDoubleSpinBox *fpsSpinbox;
    QLabel *label_8;
    QDoubleSpinBox *mpixSpinbox;
    QPushButton *resetButton;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(735, 610);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        horizontalLayout_2 = new QHBoxLayout(centralwidget);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        loadButton = new QPushButton(groupBox);
        loadButton->setObjectName(QStringLiteral("loadButton"));

        horizontalLayout->addWidget(loadButton);

        saveButton = new QPushButton(groupBox);
        saveButton->setObjectName(QStringLiteral("saveButton"));

        horizontalLayout->addWidget(saveButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        toggleOriginalButton = new QPushButton(groupBox);
        toggleOriginalButton->setObjectName(QStringLiteral("toggleOriginalButton"));

        horizontalLayout->addWidget(toggleOriginalButton);


        verticalLayout->addLayout(horizontalLayout);

        imageScrollarea = new QScrollArea(groupBox);
        imageScrollarea->setObjectName(QStringLiteral("imageScrollarea"));
        imageScrollarea->setWidgetResizable(true);
        imageScrollarea->setAlignment(Qt::AlignCenter);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 439, 526));
        imageScrollarea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(imageScrollarea);


        horizontalLayout_2->addWidget(groupBox);

        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy);
        tabWidget->setMinimumSize(QSize(250, 0));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        axSlider = new QSlider(tab);
        axSlider->setObjectName(QStringLiteral("axSlider"));
        axSlider->setGeometry(QRect(10, 40, 160, 19));
        axSlider->setMinimum(1);
        axSlider->setMaximum(10);
        axSlider->setValue(4);
        axSlider->setOrientation(Qt::Horizontal);
        axSlider->setTickPosition(QSlider::TicksAbove);
        axSlider->setTickInterval(1);
        label = new QLabel(tab);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 111, 16));
        axSpinbox = new QSpinBox(tab);
        axSpinbox->setObjectName(QStringLiteral("axSpinbox"));
        axSpinbox->setGeometry(QRect(190, 30, 42, 22));
        axSpinbox->setReadOnly(true);
        axSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        label_2 = new QLabel(tab);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 80, 111, 16));
        sxSpinbox = new QSpinBox(tab);
        sxSpinbox->setObjectName(QStringLiteral("sxSpinbox"));
        sxSpinbox->setGeometry(QRect(190, 90, 42, 22));
        sxSpinbox->setReadOnly(true);
        sxSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        sxSlider = new QSlider(tab);
        sxSlider->setObjectName(QStringLiteral("sxSlider"));
        sxSlider->setGeometry(QRect(10, 100, 160, 19));
        sxSlider->setMinimum(1);
        sxSlider->setMaximum(10);
        sxSlider->setValue(2);
        sxSlider->setOrientation(Qt::Horizontal);
        sxSlider->setTickPosition(QSlider::TicksAbove);
        sxSlider->setTickInterval(1);
        label_3 = new QLabel(tab);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 130, 141, 16));
        aSpinbox = new QDoubleSpinBox(tab);
        aSpinbox->setObjectName(QStringLiteral("aSpinbox"));
        aSpinbox->setGeometry(QRect(190, 140, 42, 22));
        aSpinbox->setReadOnly(true);
        aSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        aSlider = new QSlider(tab);
        aSlider->setObjectName(QStringLiteral("aSlider"));
        aSlider->setGeometry(QRect(10, 150, 160, 19));
        aSlider->setMinimum(1);
        aSlider->setMaximum(10000);
        aSlider->setSingleStep(1);
        aSlider->setValue(1000);
        aSlider->setOrientation(Qt::Horizontal);
        aSlider->setTickPosition(QSlider::NoTicks);
        aSlider->setTickInterval(1000);
        label_4 = new QLabel(tab);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(10, 190, 121, 16));
        hSlider = new QSlider(tab);
        hSlider->setObjectName(QStringLiteral("hSlider"));
        hSlider->setGeometry(QRect(10, 210, 160, 19));
        hSlider->setMinimum(1);
        hSlider->setMaximum(50000);
        hSlider->setSingleStep(1);
        hSlider->setValue(1000);
        hSlider->setOrientation(Qt::Horizontal);
        hSlider->setTickPosition(QSlider::NoTicks);
        hSlider->setTickInterval(1000);
        hSpinbox = new QDoubleSpinBox(tab);
        hSpinbox->setObjectName(QStringLiteral("hSpinbox"));
        hSpinbox->setGeometry(QRect(190, 200, 42, 22));
        hSpinbox->setReadOnly(true);
        hSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        label_5 = new QLabel(tab);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(50, 280, 71, 21));
        maskScrollarea = new QScrollArea(tab);
        maskScrollarea->setObjectName(QStringLiteral("maskScrollarea"));
        maskScrollarea->setGeometry(QRect(130, 250, 101, 91));
        maskScrollarea->setWidgetResizable(true);
        maskScrollarea->setAlignment(Qt::AlignCenter);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 99, 89));
        maskScrollarea->setWidget(scrollAreaWidgetContents_2);
        label_6 = new QLabel(tab);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(10, 390, 81, 16));
        timeSpinbox = new QSpinBox(tab);
        timeSpinbox->setObjectName(QStringLiteral("timeSpinbox"));
        timeSpinbox->setGeometry(QRect(100, 390, 91, 22));
        timeSpinbox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        timeSpinbox->setReadOnly(true);
        timeSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        timeSpinbox->setMaximum(999999999);
        label_7 = new QLabel(tab);
        label_7->setObjectName(QStringLiteral("label_7"));
        label_7->setGeometry(QRect(10, 420, 81, 16));
        fpsSpinbox = new QDoubleSpinBox(tab);
        fpsSpinbox->setObjectName(QStringLiteral("fpsSpinbox"));
        fpsSpinbox->setGeometry(QRect(100, 420, 91, 22));
        fpsSpinbox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        fpsSpinbox->setReadOnly(true);
        fpsSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        fpsSpinbox->setDecimals(1);
        fpsSpinbox->setMaximum(100000);
        label_8 = new QLabel(tab);
        label_8->setObjectName(QStringLiteral("label_8"));
        label_8->setGeometry(QRect(10, 450, 81, 16));
        mpixSpinbox = new QDoubleSpinBox(tab);
        mpixSpinbox->setObjectName(QStringLiteral("mpixSpinbox"));
        mpixSpinbox->setGeometry(QRect(100, 450, 91, 22));
        mpixSpinbox->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        mpixSpinbox->setReadOnly(true);
        mpixSpinbox->setButtonSymbols(QAbstractSpinBox::NoButtons);
        mpixSpinbox->setDecimals(2);
        mpixSpinbox->setMaximum(100000);
        resetButton = new QPushButton(tab);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        resetButton->setGeometry(QRect(10, 500, 75, 23));
        tabWidget->addTab(tab, QString());

        horizontalLayout_2->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "NLMeans", nullptr));
        groupBox->setTitle(QApplication::translate("MainWindow", "Image", nullptr));
        loadButton->setText(QApplication::translate("MainWindow", "Load", nullptr));
        saveButton->setText(QApplication::translate("MainWindow", "Save", nullptr));
        toggleOriginalButton->setText(QApplication::translate("MainWindow", "Toogle Original", nullptr));
        label->setText(QApplication::translate("MainWindow", "Window Size (AX, AY)", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "Support Size (SX, SY)", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Gaussian Mask stdev (A)", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "Filtering Strength (H)", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "Mask Preview", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "Rendering time", nullptr));
        timeSpinbox->setSuffix(QApplication::translate("MainWindow", " ms", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "Frame rate", nullptr));
        fpsSpinbox->setSuffix(QApplication::translate("MainWindow", " fps", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "Throughput", nullptr));
        mpixSpinbox->setSuffix(QApplication::translate("MainWindow", " MPix/s", nullptr));
        resetButton->setText(QApplication::translate("MainWindow", "Reset", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Settings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UI_MAINWINDOW_H
