# Add more folders to ship with the application, here
folder_01.source = qml/GestureCamera
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

QT += multimedia

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    cameraview.cpp \
    camerasource.cpp \
    videosurface.cpp \
    handrecapi.cpp \
    Processing/opencvcamerasource.cpp \
    Processing/handrecprivate.cpp \
    Processing/filter.cpp \
    Processing/parser.cpp \
    Processing/recognizer.cpp \
    Processing/opencvvideobuffer.cpp

LIBS += \
    -lopencv_core \
    -lopencv_imgproc \
    -lopencv_video \
    -lopencv_highgui \

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2controlsapplicationviewer/qtquick2controlsapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    cameraview.hpp \
    camerasource.hpp \
    videosurface.hpp \
    handrecapi.hpp \
    Processing/opencvcamerasource.hpp \
    Processing/handrecprivate.hpp \
    Processing/filter.hpp \
    Processing/parser.hpp \
    Processing/recognizer.hpp \
    Processing/opencvvideobuffer.hpp
