#ifndef QRC_MANAGER_H
#define QRC_MANAGER_H

#include <QObject>
#include <ConfigManager/basemanager.h>

#define MOTION_ONCEMOTION "GETONCEMOTION"
#define MOTION_ACTIVEEXPRESSION "GETEXPRESSION"
#define MOTION_ACTIVEMOTION "GETMOTION"
#define MOTION_HITAREAMOTION "GETHITAAREA"
#define MOTION_KEYMOTION "QUICKKEYMTOION"



class QRC_Manager :QObject
{
    Q_OBJECT
public:
    static QRC_Manager& instance(){
        static QRC_Manager instance;
        return instance;
    }
    // 禁止拷贝构造
    QRC_Manager(const QRC_Manager&) = delete;
    QRC_Manager& operator=(const QRC_Manager&) = delete;
    //动作管理
    void MotionQuery(QString action, QString Condition, QMap<QString,motion_unit>& value);
    void ChangeMotionValue(QString name , motion_unit value);
    void ClearMotion();
    //人脸管理
    void GetFaceInfo(FaceInfo& info);
    void ChangeFaceInfo(FaceInfo& info);
    void ClearFace();
private:
    QRC_Manager();
    ~QRC_Manager();

    MotionManager m_motion;
    FaceManager m_face;


};

#endif // QRC_MANAGER_H
