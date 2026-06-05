#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include <QMap>
#include <QObject>




class BaseManager : public QObject
{
    Q_OBJECT
public:
    // 构造函数：强制指定父对象（Qt 内存管理规范）
    explicit BaseManager(QObject *parent = nullptr);
    virtual ~BaseManager() override;

    // ================= 统一生命周期接口 =================
    virtual bool init() = 0;          // 初始化（纯虚函数）
    virtual void release() = 0;       // 释放资源
    virtual void reset() = 0;         // 重置

    // ================= 状态管理 ================
    QString managerName() const{return m_managerName;};
    bool isInited() const{return m_inited;;};


protected:
    bool m_inited  = false;
    QString m_managerName = "BaseManager";
};



/***************************************************************
*  @FileName:   basemanager.h
*  @Brief:      动作使用管理（键鼠操控）
*  @Author:     LH
*  @Date:       2026-05-30
*  @note:
****************************************************************/
enum MOTION_TYPE{
    EXPRESSION = 0,
    MOTION = 1
};

struct motion_unit{
    QString name;
    MOTION_TYPE type;
    QString hitarea = "";
    QString quickkey = "";
    bool quickkey_isuse = false;
    bool isuse = true;
};


class MotionManager:BaseManager
{
    Q_OBJECT
public:
    // 强制规范：explicit + parent
    explicit MotionManager(QObject *parent = nullptr);
    ~MotionManager() override;

    // ================= 重写生命周期 =================
    bool init() override;
    void release() override;
    void reset() override;

    // ================= 业务接口（根据需要加） =================
    //修改
    void AddMotion(QString name, motion_unit unit);
    void Motionisuse(QString name,bool use);
    //查询
    QMap<QString,motion_unit> GetOnceMotion(QString name);
    QMap<QString,motion_unit> GetActiveExpression();
    QMap<QString,motion_unit> GetActiveMotion();
    QMap<QString,motion_unit> GetHitareaMotion(QString area);
    QMap<QString,motion_unit> GetQuickKeyMotion(QString quickkey_name);

private:
    // 内部资源变量
    QMap<QString,motion_unit> m_manager;

};






/***************************************************************
*  @FileName:   basemanager.h
*  @Brief:      人脸资源管理，仅单人脸数据（多人脸等人脸跟踪识别功能实现后再说）
*  @Author:     LH
*  @Date:       2026-05-30
*  @note:
****************************************************************/
struct FaceInfo{
    //人脸姿态欧拉角
    double yaw = 0.1f;
    double pitch = 0.1f;
    double roll = 0.1f;
    double mouth_height= 0.1f;    // 上下唇垂直距离
    double mouth_angle= 0.1f;     // 嘴唇弯曲角度

    double left_eye_height= 0.1f;    // 左眼上下垂直距离
    double left_eye_angle= 0.1f;     //左眼弯曲角度（表情）

    double right_eye_height= 0.1f;    // 右眼上下垂直距离
    double right_eye_angle= 0.1f;     //右眼弯曲角度（表情）

    int64_t time = 0;
    //清空
    void clear() {
        yaw = pitch = roll = 0.0;
        time = 0;
    }
    FaceInfo() = default;
    FaceInfo(double yaw,double pitch,double roll)
        :yaw(yaw),pitch(pitch),roll(roll){}

};



class FaceManager:BaseManager
{
    Q_OBJECT
public:
    // 强制规范：explicit + parent
    explicit FaceManager(QObject *parent = nullptr);
    ~FaceManager() override;

    // ================= 重写生命周期 =================
    bool init() override;
    void release() override;
    void reset() override;

    // ================= 业务接口（根据需要加） =================
    //添加数据
    void ChangeFaceInfo(FaceInfo info);
    //获取数据
    FaceInfo GetFaceEular();
    //值域转换
    void rangeConversion(FaceInfo &info);
    //最大值标准对比
    void CompareFaceInfo(FaceInfo& info);
    //二阶平滑滤波
    void SmoothFace(FaceInfo &info);
    float mapToRange(float value, float inMin, float inMax, float outMin, float outMax);

private:
    // 内部资源变量
    FaceInfo FaceCurrent;
    FaceInfo FaceTarget;
    QTimer* m_counttimer;
    int nowtime = 0; //时间戳，查看是否超时

    FaceInfo maxFace;  //检测最大值Face，用于值域映射
};







#endif // BASEMANAGER_H
