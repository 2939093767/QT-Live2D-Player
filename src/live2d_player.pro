QT       += core gui openglwidgets widgets multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
DEFINES += CSM_TARGET_WIN_GL
QMAKE_PROJECT_DEPTH = 0
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(ConfigManager/ConfigManager.pri)
include(FaceHandle/FaceHandle.pri)

SOURCES += \
    CustomWidget.cpp \
    # cameracontrol.cpp \
    #config_setting.cpp \
    config_widget.cpp \
    # facedetector.cpp \
    # inc/Framework/Effect/CubismEyeBlink.cpp \
    # inc/Framework/Motion/CubismLipSyncUpdater.cpp \
    # inc/Framework/Motion/CubismUpdateScheduler.cpp \
    live2d_src/CubismSampleViewMatrix_Common.cpp \
    live2d_src/CubismUserModelExtend.cpp \
    live2d_src/LAppAllocator_Common.cpp \
    live2d_src/LAppDefine.cpp \
    live2d_src/LAppDelegate.cpp \
    live2d_src/LAppLive2DManager.cpp \
    live2d_src/LAppModel.cpp \
    live2d_src/LAppModel_Common.cpp \
    live2d_src/LAppPal.cpp \
    live2d_src/LAppSprite.cpp \
    live2d_src/LAppSpriteShader.cpp \
    live2d_src/LAppSprite_Common.cpp \
    live2d_src/LAppTextureManager.cpp \
    live2d_src/LAppTextureManager_Common.cpp \
    live2d_src/LAppView.cpp \
    live2d_src/LAppView_Common.cpp \
    live2d_src/LAppWavFileHandler_Common.cpp \
    live2d_src/MouseActionManager.cpp \
    live2d_src/MouseActionManager_Common.cpp \
    live2d_src/TouchManager_Common.cpp \
    llamacontrol.cpp \
    main.cpp \
    mainwindow.cpp \
    myopenglw.cpp

HEADERS += \
    CustomWidget.h \
    # cameracontrol.h \
    #config_setting.h \
    config_widget.h \
    # facedetector.h \
    # inc/Framework/Effect/CubismEyeBlink.hpp \
    # inc/Framework/Motion/CubismLipSyncUpdater.hpp \
    # inc/Framework/Motion/CubismUpdateScheduler.hpp \
    live2d_src/CubismSampleViewMatrix_Common.hpp \
    live2d_src/CubismUserModelExtend.hpp \
    live2d_src/LAppAllocator_Common.hpp \
    live2d_src/LAppDefine.hpp \
    live2d_src/LAppDelegate.hpp \
    live2d_src/LAppLive2DManager.hpp \
    live2d_src/LAppModel.hpp \
    live2d_src/LAppModel_Common.hpp \
    live2d_src/LAppPal.hpp \
    live2d_src/LAppSprite.hpp \
    live2d_src/LAppSpriteShader.hpp \
    live2d_src/LAppSprite_Common.hpp \
    live2d_src/LAppTextureManager.hpp \
    live2d_src/LAppTextureManager_Common.hpp \
    live2d_src/LAppView.hpp \
    live2d_src/LAppView_Common.hpp \
    live2d_src/LAppWavFileHandler_Common.hpp \
    live2d_src/MouseActionManager.hpp \
    live2d_src/MouseActionManager_Common.hpp \
    live2d_src/TouchManager_Common.hpp \
    llamacontrol.h \
    mainwindow.h \
    myopenglw.h

FORMS += \
    AppWidget.ui \
    ModelWidget.ui \
    # cameracontrol.ui \
    config_widget.ui \
    mainwindow.ui

# Default rules for deployment.
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
                                -lllama-common\
                                -llLama-common-base\
                                -lggml-cpu\
                                -lggml-base\
                                -lggml\
                                -lcpp-httplib


INCLUDEPATH += $$PWD/inc/llama
DEPENDPATH += $$PWD/libfiles/llama


DEFINES += _USE_MATH_DEFINES
DEFINES += NOMINMAX
DEFINES += DLIB_USE_SYSTEM_MATH_FUNCTIONS

#dlib
LIBS += -L$$PWD/libfiles/dlib/ -ldlib20.0.0_release_64bit_msvc1929
INCLUDEPATH += $$PWD/inc/dlib
DEPENDPATH += $$PWD/libfiles/dlib



