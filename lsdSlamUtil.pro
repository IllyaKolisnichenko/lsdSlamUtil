#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T15:20:05
#
#-------------------------------------------------

QT       -= core gui

TARGET      = lsdSlamUtil
TEMPLATE    = lib

CONFIG += c++11

DEFINES += LSDSLAMUTIL_LIBRARY

#QMAKE_CFLAGS_DEBUG    += -pg
#QMAKE_CXXFLAGS_DEBUG  += -pg
#QMAKE_LFLAGS_DEBUG    += -pg

SOURCES += \
    SophusUtil.cpp \
    globalFuncs.cpp \
    ImageDisplay_OpenCV.cpp \
    settings.cpp

HEADERS += \
    EigenCoreInclude.h \
#    IndexThreadReduce.h \
    settings.h \
    SophusUtil.h \
    globalFuncs.h \
    ImageDisplay.h

unix {

    # OpenCV
#    OPENCV_INCLUDE_PATH        = /home/sergey/libs/opencv-3.0.0/include
#    OPENCV_INCLUDE_MODULE_PATH = /home/sergey/libs/opencv-3.0.0/release/modules

#    OPENCV_LIBS_PATH           = /home/sergey/libs/opencv-3.0.0/release/lib

#    message( " Unix - Version OpenCV - 3.00 - Release " )
#    message( $$OPENCV_LIBS_PATH )

##    LIBS    += -L$$OPENCV_LIBS_PATH
    LIBS    += -L/usr/local/lib

    LIBS    += -lopencv_objdetect   -lopencv_imgproc
    LIBS    += -lopencv_videoio     -lopencv_core
    LIBS    += -lopencv_highgui
#    LIBS    += -lopencv_features2d  -lopencv_calib3d
#    LIBS    += -lopencv_imgcodecs

#    INCLUDEPATH += /home/sergey/libs/Sophus

    PKGCONFIG = gtk+-2.0
    CONFIG += link_pkgconfig

    BASE_LIBS_PATH = $$PWD/../build

    INCLUDEPATH +=  ../lsdSlamIO/
    LIBS        +=  -L$$BASE_LIBS_PATH/lsdSlamIO     \
                    -llsdSlamIO

    INCLUDEPATH +=  ../lsdSlamFrame/
    LIBS        +=  -L$$BASE_LIBS_PATH/lsdSlamFrame  \
                    -llsdSlamFrame

    INCLUDEPATH +=  ../lsdSlamGlobalMapping/
    LIBS        +=  -L$$BASE_LIBS_PATH/lsdSlamGlobalMapping  \
#                    -llsdSlamGlobalMapping


    #target.path = /usr/lib
    target.path = $$BASE_LIBS_PATH/lsdSlamApp
    INSTALLS += target
}
