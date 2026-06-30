#ifndef MYOPENGLW_H
#define MYOPENGLW_H

#include <QTimer>
#include <QWidget>
#include <QFile>
#include <QPushButton>
#include <QLineEdit>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QMoveEvent>
#include <QShowEvent>
#include <QHideEvent>
#include <QEnterEvent>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QList>
#include <functional>


#pragma once
#include "LAppDelegate.hpp"
#include "LAppTextureManager.hpp"
#include "LAppModel.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppView.hpp"
#include "Framework/CubismDefaultParameterId.hpp"

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QObject>
#include <QMouseEvent>

#include "ConfigManager/qrc_manager.h"
#include "ConfigManager/configini.h"
#include "FaceHandle/cameracontrol.h"






// class GLRefreshThread;

class MyOpenGL : public QOpenGLWidget
{
    Q_OBJECT
public:
    enum TrackMode {
        TrackMode_None = 0,
        TrackMode_Mouse = 1,
        TrackMode_Face = 2,
        TrackMode_AI = 3
    };
    Q_ENUM(TrackMode)

    MyOpenGL(QWidget *parent = nullptr);
    ~MyOpenGL();
    void ToolBtnInit();

    // 添加工具按钮
    // iconPath: 图标路径（可为空），onClicked: 点击回调lambda
    void addToolButton(const QString &iconPath, std::function<void()> onClicked);
    // 添加文字按钮
    void addTextButton(const QString &text, std::function<void()> onClicked);
    // 添加文字模式按钮
    void addModeButton(const QString &text, TrackMode mode);

    const void SetMode(APP_MODE mode){m_mode = mode;}

    void KeyRenderUpdate();
    void FaceRenderUpdate();

    void setBackgroundImage(const QString &path);

    // 模型显示控制
    void setModelScale(float scale);    // 设置模型缩放 (0.1 - 3.0)
    void setModelOpacity(float opacity); // 设置模型透明度 (0.0 - 1.0)
    float getModelScale() const;
    float getModelOpacity() const;

    // 跟踪模式
    void setTrackMode(TrackMode mode);
    TrackMode getTrackMode() const { return m_trackMode; }

    void addSeparator();
signals:
    void openSettingsRequested();       // 请求打开设置界面
    void trackModeChanged(TrackMode mode);  // 跟踪模式改变


public slots:
    void modelupdate();
    void StartRender();
    void StopRender();
    void ChangeModel();
protected:
/**********************************************
* @brief         初始化gl
* @date          2026-05-03
**********************************************/
    void initializeGL() override;



/**********************************************
* @brief         gl窗口更改
* @date          2026-05-03
**********************************************/
    void resizeGL(int w, int h) override;




/**********************************************
* @brief         绘制函数
* @date          2026-05-03
**********************************************/
    void paintGL() override;

    // ======================
    // 鼠标事件
    // ======================
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

    void moveEvent(QMoveEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;

    const int invertFPS(int mode);

private:
    QTimer* t_modelupdate;
    APP_MODE m_mode = PET_MODE;



    QPoint m_lastPos;       // 上次鼠标位置
    QPoint m_winDragPos;
    bool   m_isDragging;    // 是否正在拖拽
    float  m_offsetX;       // 画布X偏移
    float  m_offsetY;       // 画布Y偏移

    //运行标志位
    bool   m_isrunning = false;

    // 模型显示参数
    float  m_modelScale = 1.0f;    // 模型缩放
    float  m_modelOpacity = 1.0f;  // 模型透明度

    // 跟踪模式
    TrackMode m_trackMode = TrackMode_None;

    // 工具按钮容器
    QWidget *m_stubWidget;           // 顶部触发条（小边栏）
    QWidget *m_panelWidget;            // 弹出按钮面板（横向）
    QHBoxLayout *m_stubLayout;        // 触发条内部布局
    QHBoxLayout *m_panelLayout;       // 按钮面板水平布局
    QList<QPushButton*> m_toolButtons;
    QList<QPushButton*> m_modeButtons;
    bool m_panelVisible = false;      // 面板显示状态

    // 事件处理
    bool eventFilter(QObject *watched, QEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

    // 工具栏位置更新
    void updateToolWidgetPosition();
    void togglePanel();
    void showPanel();
    void hidePanel();

    // 人脸跟踪控制
    void startFaceTracking();
    void stopFaceTracking();
};







#endif // MYOPENGLW_H
