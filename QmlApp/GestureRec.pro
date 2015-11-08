TEMPLATE = app

QT += qml quick widgets multimedia

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

    CONFIG(debug, debug|release) {
        LIBS += -lopencv_world300d
    } else {
        LIBS += -lopencv_world300
    }

} else:android {

    #not currently tested

    INCLUDEPATH += $$(OPENCV_DIR)/jni/include

    INCLUDEPATH

    LIBS += \
        -L$$(OPENCV_DIR)/libs/armeabi-v7a \
        -lopencv_java \
        -lnative_camera_r4.4.0 \
        -lopencv_info \
        -lopencv_androidcamera  \
#        -lopencv_legacy \
#        -lopencv_calib3d \
#        -lopencv_ml  \
#        -lopencv_contrib \
        -lopencv_core      \
#        -lopencv_objdetect \
#        -lopencv_features2d  \
#        -lopencv_ocl \
#        -lopencv_flann \
        -lopencv_photo \
#        -lopencv_stitching \
        -lopencv_highgui     \
#        -lopencv_superres \
        -lopencv_imgproc    \
#        -lopencv_ts \
#        -lopencv_video \
#        -lopencv_videostab

} else {

#    CONFIG += link_pkgconfig
#    PKGCONFIG += opencv
#    PKGCONFIG += opencv_imgproc opencv_video opencv_highgui

    #not currently tested.

    LIBS += \
        -lopencv_core \
        -lopencv_imgproc \
        -lopencv_video \
        -lopencv_highgui

}

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
    Utils/jsonreader.cpp \
    QtCamera/camerasource.cpp \
    QtCamera/videosurface.cpp

RESOURCES += qml.qrc

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
    Processing/defines.hpp \
    QtCamera/camerasource.hpp \
    QtCamera/videosurface.hpp

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_EXTRA_LIBS = /home/morgan/GestureRec/../Programming/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_info.so /home/morgan/GestureRec/../Programming/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libopencv_java.so /home/morgan/GestureRec/../Programming/OpenCV-2.4.9-android-sdk/sdk/native/libs/armeabi-v7a/libnative_camera_r4.4.0.so
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

OTHER_FILES += \
    android/AndroidManifest.xml
