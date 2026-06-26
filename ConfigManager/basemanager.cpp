#include "basemanager.h"

#include <QTimer>
#include <qrandom.h>
#include <ConfigManager/configini.h>

BaseManager::BaseManager(QObject *parent)
    : QObject(parent)
{

}

BaseManager::~BaseManager()
{

}



//================================动作表情部分===========================//
/***************************************************************
*  @FileName:   basemanager.cpp
*  @Brief:      动作数据管理
*  @Author:     LH
*  @Date:       2026-05-30
*  @note:
****************************************************************/
MotionManager::MotionManager(QObject *parent)
{
    m_managerName = "MotionManager";
}

MotionManager::~MotionManager()
{

}

bool MotionManager::init()
{

    return true;
}

void MotionManager::release()
{

}

void MotionManager::reset()
{
    m_manager.clear();
}

void MotionManager::AddMotion(QString name, motion_unit unit)
{m_manager[name] = unit;}

void MotionManager::Motionisuse(QString name, bool use)
{m_manager[name].isuse = use;}

QMap<QString, motion_unit> MotionManager::GetOnceMotion(QString name)
{
    QMap<QString, motion_unit> resultMap;
    resultMap[name] = m_manager[name];
    return resultMap;
}

QMap<QString, motion_unit> MotionManager::GetHitareaMotion(QString area)
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value();

        // ====================== 筛选条件 ======================

        if (unit.hitarea == area && unit.isuse && unit.quickkey_isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}




QMap<QString, motion_unit> MotionManager::GetQuickKeyMotion(QString quickkey_name)
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value();

        // ====================== 筛选条件 ======================
        // 提取 type == "expression" 的项
        if (unit.quickkey == quickkey_name && unit.isuse && unit.quickkey_isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}



QMap<QString, motion_unit> MotionManager::GetActiveExpression()
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value();

        // ====================== 筛选条件 ======================
        if (unit.type == EXPRESSION && unit.isuse)
        {
            resultMap.insert(key, unit);
        }
    }


    return resultMap;
}



QMap<QString, motion_unit> MotionManager::GetActiveMotion()
{
    QMap<QString, motion_unit> resultMap;
    // 遍历内部的总 map
    for (auto it = m_manager.begin(); it != m_manager.end(); ++it)
    {
        const QString& key = it.key();
        const motion_unit& unit = it.value() ;

        // ====================== 筛选条件 ======================
        // 提取 type == "expression" 的项
        if (unit.type == MOTION && unit.isuse)
        {
            resultMap.insert(key, unit);
        }
    }
    return resultMap;
}









//================================人脸部分===========================//
/***************************************************************
*  @FileName:   basemanager.cpp
*  @Brief:      人脸数据管理
*  @Author:     LH
*  @Date:       2026-05-30
*  @note:
****************************************************************/

FaceManager::FaceManager(QObject *parent)
{

    init();
}

FaceManager::~FaceManager()
{

}

bool FaceManager::init()
{
    m_counttimer = new QTimer(this);
    m_counttimer->setInterval(1000);
    connect(m_counttimer, &QTimer::timeout, this, [=]{
        nowtime += 1;
    });
    m_counttimer->start();
    m_managerName = "FaceManager";
    return true;
}

void FaceManager::release()
{
    delete m_counttimer;
}



void FaceManager::reset()
{
    FaceCurrent.clear();
    FaceTarget.clear();
    nowtime = 0;
}


void FaceManager::rangeConversion(FaceInfo &info){

    double sensitivity = ConfigManager::instance().getValue(CONFIG_MOTION_SENSITIVITY).toDouble();
    info.yaw = mapToRange((info.yaw==0?0.1:info.yaw)*sensitivity,-10,50,-30,30);
    info.pitch = mapToRange((info.pitch==0?0.1:info.pitch)*sensitivity,-20,10,-30,30);
    info.roll = mapToRange((info.roll==0?0.1:info.roll)*sensitivity,-10,20,-30,30);
    info.left_eye_height = mapToRange((info.left_eye_height==0?0.1:info.left_eye_height)*sensitivity,5,8,-1,1);
    info.right_eye_height = mapToRange((info.right_eye_height ==0?0.1:info.right_eye_height)*sensitivity,5,8,-1,1);
    info.mouth_height = mapToRange((info.mouth_height ==0?0.1:info.mouth_height)*sensitivity,0,10,0,2);

}


void FaceManager::CompareFaceInfo(FaceInfo &info)
{
    // maxFace.yaw = std::max(maxFace.yaw,info.yaw);
    // maxFace.pitch = std::max(maxFace.pitch,info.pitch);
    // maxFace.roll = std::max(maxFace.roll,info.roll);
    // maxFace.left_eye_height = std::max(maxFace.left_eye_height,info.left_eye_height);
    // maxFace.right_eye_height = std::max(maxFace.right_eye_height,info.right_eye_height);
    // maxFace.left_eye_angle = std::max(maxFace.left_eye_angle,info.left_eye_angle);
    // maxFace.right_eye_angle = std::max(maxFace.right_eye_angle,info.right_eye_angle);
    // maxFace.mouth_height = std::max(maxFace.mouth_height,info.mouth_height);
    // maxFace.mouth_angle = std::max(maxFace.mouth_angle,info.mouth_angle);

}



void FaceManager::ChangeFaceInfo(FaceInfo info)
{
    //qDebug()<<info.yaw<<info.pitch<<info.roll;
    CompareFaceInfo(info);
    rangeConversion(info);
    FaceTarget = info;
}


void FaceManager::SmoothFace(FaceInfo& info){
    double alpha = ConfigManager::instance().getValue(CONFIG_MOTION_SMOOTH_PARAMTER).toDouble();;
    info.yaw   = FaceCurrent.yaw + (FaceTarget.yaw - FaceCurrent.yaw) * alpha;
    info.pitch = FaceCurrent.pitch + (FaceTarget.pitch - FaceCurrent.pitch) * alpha;
    info.roll  = FaceCurrent.roll + (FaceTarget.roll - FaceCurrent.roll) * alpha;
    info.left_eye_height = FaceCurrent.left_eye_height + (FaceTarget.left_eye_height - FaceCurrent.left_eye_height) * alpha;
    info.right_eye_height   = FaceCurrent.right_eye_height + (FaceTarget.right_eye_height - FaceCurrent.right_eye_height) * alpha;
    info.mouth_height  = FaceCurrent.mouth_height + (FaceTarget.mouth_height - FaceCurrent.mouth_height) * alpha;
}

float FaceManager::mapToRange(float value, float inMin, float inMax, float outMin, float outMax)
{
    // 1. 先把输入限制在有效范围内（防止异常）
    if (value < inMin) value = inMin;
    if (value > inMax) value = inMax;

    // 2. 线性映射到目标范围
    float normalized = (value - inMin) / (inMax - inMin);
    float result = outMin + normalized * (outMax - outMin);

    return result;
}


FaceInfo FaceManager::GetFaceEular()
{
    FaceInfo smooth;
    SmoothFace(smooth);
    FaceCurrent = smooth;
    return smooth;
}





