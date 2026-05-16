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


#pragma once
#include "LAppDelegate.hpp"
#include "LAppTextureManager.hpp"
#include "LAppModel.hpp"
#include "LAppLive2DManager.hpp"
#include "LAppView.hpp"

#include <QtOpenGLWidgets/QOpenGLWidget>
#include <QObject>
#include <QMouseEvent>

#include "config_setting.h"








class GLRefreshThread;

class MyOpenGL : public QOpenGLWidget
{
    Q_OBJECT
public:
    MyOpenGL(QWidget *parent = nullptr);
    ~MyOpenGL();

    const void SetMode(APP_MODE mode){m_mode = mode;}





public slots:
    void modelupdate();



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

    const int invertFPS(int mode);

private:
    QTimer* t_modelupdate;
    APP_MODE m_mode = PET_MODE;



    QPoint m_lastPos;       // 上次鼠标位置
    QPoint m_winDragPos;
    bool   m_isDragging;    // 是否正在拖拽
    float  m_offsetX;       // 画布X偏移
    float  m_offsetY;       // 画布Y偏移

};







#endif // MYOPENGLW_H
