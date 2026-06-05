#include "qrc_manager.h"

#include <QVariant>



QRC_Manager::QRC_Manager() {}

QRC_Manager::~QRC_Manager()
{

}


void QRC_Manager::MotionQuery(QString action, QString Condition, QMap<QString, motion_unit> &value)
{
    if(action == MOTION_ONCEMOTION){
        value = m_motion.GetOnceMotion(Condition);
    }else if(action == MOTION_ACTIVEEXPRESSION){
        value = m_motion.GetActiveExpression();
    }else if(action == MOTION_ACTIVEMOTION){
        value = m_motion.GetActiveMotion();
    }else if(action == MOTION_HITAREAMOTION){
        value = m_motion.GetHitareaMotion(Condition);
    }else if(action == MOTION_KEYMOTION){
        value = m_motion.GetQuickKeyMotion(Condition);
    }
}

void QRC_Manager::ChangeMotionValue(QString name, motion_unit value)
{
    m_motion.AddMotion(name,value);
}

void QRC_Manager::ClearMotion()
{
    m_motion.reset();
}


void QRC_Manager::GetFaceInfo(FaceInfo &info)
{
    info = m_face.GetFaceEular();
}

void QRC_Manager::ChangeFaceInfo(FaceInfo &info)
{
    m_face.ChangeFaceInfo(info);
}

void QRC_Manager::ClearFace()
{
    m_face.reset();
}


