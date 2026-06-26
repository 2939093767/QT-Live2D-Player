QT       += core gui openglwidgets widgets multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = Live2DpetSettings
TEMPLATE = app

DEFINES += CSM_TARGET_WIN_GL
QMAKE_PROJECT_DEPTH = 0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    myopenglw.cpp

HEADERS += \
    mainwindow.h \
    myopenglw.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    resources.qrc

include(settings/appsettingwindow.pri)
include(monitor/monitor.pri)
include(CustomWidget/CustomWidget.pri)
include(live2d_src/live2d_src.pri)
include(ConfigManager/ConfigManager.pri)
include(LLamaAIControl/LLamaAIControl.pri)
include(FaceHandle/FaceHandle.pri)
include(utils/utils.pri)

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



INCLUDEPATH += $$PWD/inc/Core/include
INCLUDEPATH += $$PWD/inc/GLFW
INCLUDEPATH += $$PWD/inc
INCLUDEPATH += $$PWD/inc/Framework
INCLUDEPATH += $$PWD/inc/stb
INCLUDEPATH += $$PWD/inc/Common
INCLUDEPATH += $$PWD/live2d_src

LIBS += -lopengl32 -luser32 -lgdi32 -lshell32
LIBS += -L$$PWD/libfiles/framework/ -lFramework
LIBS += -L$$PWD/libfiles/glew/ -llibglew32
LIBS += -L$$PWD/libfiles/glfw/ -lglfw3
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libfiles/live2dCore/ -lLive2DCubismCore_MD
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libfiles/live2dCore/ -lLive2DCubismCore_MDd

RESOURCES += \
    image.qrc \
    qttheme/darkstyle.qrc




#opencv
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libfiles/opencv/ -lopencv_world4120
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libfiles/opencv/ -lopencv_world4120d
INCLUDEPATH += $$PWD/inc/opencv/include
DEPENDPATH += $$PWD/libfiles/opencv





#llama
LIBS += -L$$PWD/libfiles/llama/ -lllama\
                                -lggml-cuda\
                                -lllama-common\
                                -llLama-common-base\
                                -lggml-cpu\
                                -lggml-base\
                                -lggml\
                                -lcpp-httplib


INCLUDEPATH += $$PWD/inc/llama_new
INCLUDEPATH += $$PWD/inc/llama_new/common
INCLUDEPATH += $$PWD/inc/llama_new/vendor


DEPENDPATH += $$PWD/libfiles/llama


DEFINES += _USE_MATH_DEFINES
DEFINES += NOMINMAX
DEFINES += DLIB_USE_SYSTEM_MATH_FUNCTIONS

#dlib
LIBS += -L$$PWD/libfiles/dlib/ -ldlib20.0.0_release_64bit_msvc1929
INCLUDEPATH += $$PWD/inc/dlib
DEPENDPATH += $$PWD/libfiles/dlib
