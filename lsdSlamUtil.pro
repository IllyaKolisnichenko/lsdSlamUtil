#-------------------------------------------------
#
# Project created by QtCreator 2015-11-13T15:20:05
#
#-------------------------------------------------

QT       -= core gui

TARGET = lsdSlamUtil
TEMPLATE = lib

CONFIG += c++11

DEFINES += LSDSLAMUTIL_LIBRARY

SOURCES += \
    SophusUtil.cpp \
    globalFuncs.cpp \
    ImageDisplay_OpenCV.cpp \
    settings.cpp

HEADERS += \
    EigenCoreInclude.h \
    IndexThreadReduce.h \
    settings.h \
    SophusUtil.h \
    globalFuncs.h \
    ImageDisplay.h

unix {

    # OpenCV
    OPENCV_INCLUDE_PATH        = /home/sergey/libs/opencv-3.0.0/include
    OPENCV_INCLUDE_MODULE_PATH = /home/sergey/libs/opencv-3.0.0/release/modules

    OPENCV_LIBS_PATH           = /home/sergey/libs/opencv-3.0.0/release/lib

    message( " Unix - Version OpenCV - 3.00 - Release " )
    message( $$OPENCV_LIBS_PATH )

    LIBS    += -L$$OPENCV_LIBS_PATH
    LIBS    += -lopencv_objdetect   -lopencv_imgproc
    LIBS    += -lopencv_videoio     -lopencv_core
    LIBS    += -lopencv_imgcodecs   -lopencv_highgui
    LIBS    += -lopencv_features2d  -lopencv_calib3d

    INCLUDEPATH += /home/sergey/libs/Sophus

    INCLUDEPATH +=  /home/sergey/MyProject/MySlamProject/Qt/lsdSlamIO/
    LIBS        +=  -L/home/sergey/MyProject/MySlamProject/Qt/FullProject/build/lsdSlamIO     \
                    -llsdSlamIO

    INCLUDEPATH += /home/sergey/MyProject/MySlamProject/Qt/lsdSlamFrame/
    LIBS        +=  -L/home/sergey/MyProject/MySlamProject/Qt/FullProject/build/lsdSlamFrame  \
                    -llsdSlamFrame

    INCLUDEPATH += /home/sergey/MyProject/MySlamProject/Qt/lsdSlamGlobalMapping/
    LIBS        +=  -L/home/sergey/MyProject/MySlamProject/Qt/FullProject/build/lsdSlamGlobalMapping  \
                    -llsdSlamGlobalMapping


    #target.path = /usr/lib
    target.path = /home/sergey/MyProject/MySlamProject/lsdSlamSharedLibs
    INSTALLS += target
}
