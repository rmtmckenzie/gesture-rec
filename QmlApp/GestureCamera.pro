# Add more folders to ship with the application, here
folder_01.source = qml/GestureCamera
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += multimedia

QMAKE_CXXFLAGS += -Os

win32 | win64 {

    #For this to work on windows, must define OPENCV_DIR.
    #This can be done for each project in Projects tab, by
    #simply adding the variable, or system wide by searching
    #for system environment variables.

    #In addition to this, the bin folder should also be added
    #to the system's path, e.g. %PATH%;%OPENCV_DIR%\bin
    #For my system, OPENCV_DIR=C:\OpenCV\opencv\build\x86\vc11
    #(vc11 = Visual Studio 2012, vc12 = Visual Studio 2013...)
    INCLUDEPATH += $$(OPENCV_DIR)/../../include/
    LIBS += -L$$(OPENCV_DIR)/lib \

    LIBS += \
        -lopencv_core249d \
        -lopencv_imgproc249d \
        -lopencv_highgui249d \

} else {

    CONFIG += link_pkgconfig
    PKGCONFIG += opencv
#        opencv_imgproc opencv_video opencv_highgui

#    LIBS += \
#        -lopencv_core \
#        -lopencv_imgproc \
#        -lopencv_video \
#        -lopencv_highgui \

}

#echo.target = ec
#echo.commands = echo $$(OPENCV_DIR)
#echo.CONFIG = target_predeps
#QMAKE_EXTRA_TARGETS += echo
#PRE_TARGETDEPS += ec


# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()

SOURCES += main.cpp \
    cameraview.cpp \
    handrecapi.cpp \
    Processing/opencvcamerasource.cpp \
    Processing/filter.cpp \
    Processing/parser.cpp \
    Processing/recognizer.cpp \
    Processing/opencvvideobuffer.cpp \
    Processing/camerasettings.cpp \
    Processing/handrecthread.cpp \
    Processing/handrecprivate.cpp \
    Utils/jsonreader.cpp

HEADERS += \
    cameraview.hpp \
    handrecapi.hpp \
    Processing/opencvcamerasource.hpp \
    Processing/filter.hpp \
    Processing/parser.hpp \
    Processing/recognizer.hpp \
    Processing/opencvvideobuffer.hpp \
    Processing/camerasettings.hpp \
    Processing/handrecthread.hpp \
    Processing/handrecprivate.hpp \
    Utils/jsonreader.hpp \
    Processing/defines.hpp
