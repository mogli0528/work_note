TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    defaultparamter.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    defaultparamter.h

