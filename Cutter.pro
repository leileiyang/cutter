#-------------------------------------------------
#
# Project created by QtCreator 2018-03-20T16:06:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cutter
TEMPLATE = app


SOURCES += main.cpp\
        cuttermainpage.cpp \
    craftparam.cpp \
    layercfg.cpp \
    controller.cpp \
    xmlparser/LaserXmlParser.cpp \
    xmlparser/pugixml.cpp \
    xmlparser/XmlParser.cpp

HEADERS  += cuttermainpage.h \
    craftparam.h \
    dev_cfg/DelayCfg.h \
    dev_cfg/FollowerCfg.h \
    dev_cfg/GasCfg.h \
    dev_cfg/IOCfg.h \
    dev_cfg/LaserCfg.h \
    dev_cfg/PlcCfg.h \
    layercfg.h \
    controller.h \
    xmlparser/LaserXmlParser.h \
    xmlparser/pugiconfig.hpp \
    xmlparser/pugixml.hpp \
    xmlparser/XmlParser.h

FORMS    += cuttermainpage.ui \
    craftparam.ui \
    layercfg.ui

unix|win32: LIBS += -lboost_serialization

unix|win32: LIBS += -lzmq
