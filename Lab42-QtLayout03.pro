QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

INCLUDEPATH += C:\opencv\build\include

LIBS += C:\opencv\build\bin\libopencv_core470.dll
LIBS += C:\opencv\build\bin\libopencv_highgui470.dll
LIBS += C:\opencv\build\bin\libopencv_imgcodecs470.dll
LIBS += C:\opencv\build\bin\libopencv_features2d470.dll
LIBS += C:\opencv\build\bin\libopencv_calib3d470.dll

# more correct variant, how set includepath and libs for mingw
# add system variable: OPENCV_SDK_DIR=D:/opencv/opencv-build/install
# read http://doc.qt.io/qt-5/qmake-variable-reference.html#libs

OPENCV_SDK_DIR = C:\opencv\build\install

INCLUDEPATH += $$(OPENCV_SDK_DIR)\include

LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_core470.dll.a
LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_highgui470.dll.a
LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_imgcodecs470.dll.a
LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_imgproc470.dll.a
LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_features2d470.dll.a
LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_calib3d470.dll.a
LIBS += C:\opencv\build\install\x64\mingw\lib\libopencv_videoio470.dll.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
