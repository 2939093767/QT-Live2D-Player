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






// class GLRefreshThread;

class MyOpenGL : public QOpenGLWidget
{
    Q_OBJECT
public:
    MyOpenGL(QWidget *parent = nullptr);
    ~MyOpenGL();
    void ToolBtnInit();

    // 添加工具按钮
    // iconPath: 图标路径（可为空），onClicked: 点击回调lambda
    void addToolButton(const QString &iconPath, std::function<void()> onClicked);

    const void SetMode(APP_MODE mode){m_mode = mode;}

    void KeyRenderUpdate();
    void FaceRenderUpdate();

    void setBackgroundImage(const QString &path);

    // 模型显示控制
    void setModelScale(float scale);    // 设置模型缩放 (0.1 - 3.0)
    void setModelOpacity(float opacity); // 设置模型透明度 (0.0 - 1.0)
    float getModelScale() const;
    float getModelOpacity() const;


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
    bool   m_isrunning;

    // 模型显示参数
    float  m_modelScale = 1.0f;    // 模型缩放
    float  m_modelOpacity = 1.0f;  // 模型透明度

    // 工具按钮容器
    QWidget *m_toolWidget;
    QWidget *m_panelWidget;        // 按钮面板
    QVBoxLayout *m_toolLayout;
    QList<QPushButton*> m_toolButtons;

    // 事件处理
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
};







#endif // MYOPENGLW_H
