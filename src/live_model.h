#ifndef LIVE_MODEL_H
#define LIVE_MODEL_H

#include <QObject>
#include <Live2DCubismCore.h>


struct DrawABLE
{
    //顶点渲染
    int VertexCount;             // 顶点数量
    QVector<csmVector4> Positions;      // SOA 顶点位置
    QVector<csmVector2> Uvs;            // SOA 纹理坐标
    //片段索引
    int IndexCount;              // 索引数量
    const unsigned short* Indices;// 索引
    //单元设置属性
    int BlendMode;
    int IsDoubleSided;
    int IsInvertedMask;
    //纹理相关索引
    int TextureIndex;
    //其他属性
    const char* ID;
    int DrawOrder;
    float Opacity;
    int RenderOrder;
    csmFlags DynamicFlag;
    //遮罩相关
    int MaskCount;
    QVector<int> Masks;
    DrawABLE* MaskLinks;

};








class Live_Model
{
public:
    Live_Model();

    static void GetModelInfor(QVector<DrawABLE>& unit);
    static void *AllocateAligned(unsigned int size, unsigned int alignment);
    static void *ReadBlobAligned(const char *filePath, int alignment, unsigned int *outSize);
    static void UpdateModel();
    static void Maskcompute(QVector<DrawABLE>& m_units);



    void GetModelInfor(QVector<DrawABLE> &unit);
private:
    static csmModel* m_model;
    QVector<DrawABLE> m_units;
};

#endif // LIVE_MODEL_H
