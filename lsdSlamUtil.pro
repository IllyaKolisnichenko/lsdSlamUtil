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

QMAKE_CFLAGS_DEBUG    += -g -funwind-tables -fno-omit-frame-pointer -std=c++11
QMAKE_CXXFLAGS_DEBUG  += -g -funwind-tables -fno-omit-frame-pointer -std=c++11
QMAKE_LFLAGS_DEBUG    += -g -funwind-tables -fno-omit-frame-pointer -std=c++11

#QMAKE_CFLAGS   += -std=c++11 -fopenmp
#QMAKE_CXXFLAGS += -std=c++11 -fopenmp
#QMAKE_LFLAGS   += -std=c++11 -fopenmp

SOURCES +=                                  \
            src/SophusUtil.cpp              \
            src/globalFuncs.cpp             \
#            src/ImageDisplay_OpenCV.cpp     \
            src/settings.cpp                \
            src/debugimage/debugimage.cpp   \
            src/debugimage/opencvdebugimage.cpp

INCLUDEPATH += include/ include/debugimage/

HEADERS +=                                  \
            include/EigenCoreInclude.h      \
#           include/IndexThreadReduce.h     \
            include/settings.h              \
            include/SophusUtil.h            \
            include/globalFuncs.h           \
#            include/ImageDisplay.h          \
            include/debugimage/debugimage.h \
            include/debugimage/opnecvdebugimage.h

unix {
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
