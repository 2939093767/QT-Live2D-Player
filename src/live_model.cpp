#include "live_model.h"


csmModel* Live_Model::m_model = nullptr;



Live_Model::Live_Model() {
    void* mocMemory;
    unsigned int mocSize;

    // Load file to memory address aligned as 64byte.
    // The file size of .moc3 is stored in mocSize.
    mocMemory = ReadBlobAligned(R"(D:\QT_PROGRAME\live2d_player\chitose\runtime\chitose.moc3)", csmAlignofMoc, &mocSize);
    csmMoc* moc = csmReviveMocInPlace(mocMemory, mocSize);
    unsigned int modelSize = csmGetSizeofModel(moc);
    // The model needs to be aligned as 16 bytes
    void* modelMemory = AllocateAligned(modelSize, csmAlignofModel);
    // Create an instance of the model
    int consistency = csmHasMocConsistency(mocMemory, mocSize);
    m_model = csmInitializeModelInPlace(moc, modelMemory, modelSize);
    csmVector2 size;
    csmVector2 origin;
    float pixelsPerUnit;
    csmReadCanvasInfo(m_model, &size, &origin, &pixelsPerUnit);
    GetModelInfor(m_model,m_units);


}




void Live_Model::GetModelInfor(QVector<DrawABLE> &unit)
{
    int drawableCount = csmGetDrawableCount(m_model);
    unit.resize(drawableCount);
    const int* textureIndices = csmGetDrawableTextureIndices(m_model);
    const csmFlags* constantFlags = csmGetDrawableConstantFlags(m_model);
    const int* vertexCounts = csmGetDrawableVertexCounts(m_model);
    const csmVector2** vertexPositons = csmGetDrawableVertexPositions(m_model);
    const csmVector2** vertexUvs = csmGetDrawableVertexUvs(m_model);
    const int* indexCounts = csmGetDrawableIndexCounts(m_model);
    const unsigned short** vertexIndices = csmGetDrawableIndices(m_model);


    //其他信息
    const char** ids = csmGetDrawableIds(m_model);
    const float* opacities = csmGetDrawableOpacities(m_model);
    const int *drawOrders = csmGetDrawableDrawOrders(m_model);
    const int* renderOrders = csmGetRenderOrders(m_model);
    const csmFlags* dynamicFlags = csmGetDrawableDynamicFlags(m_model);
    const int* maskCounts = csmGetDrawableMaskCounts(m_model);
    const int** masks = csmGetDrawableMasks(m_model);
    const csmVector4* multiplyColors = csmGetDrawableMultiplyColors(m_model);
    const csmVector4* screenColors = csmGetDrawableScreenColors(m_model);
    const int* blendModes = csmGetDrawableBlendModes(m_model);


    // Initialize static drawable fields.
    for (int d = 0; d < drawableCount; ++d)
    {
        unit[d].TextureIndex = textureIndices[d];

        if ((constantFlags[d] & csmBlendAdditive) == csmBlendAdditive)
        {
            unit[d].BlendMode = csmBlendAdditive;
        }
        else if ((constantFlags[d] & csmBlendMultiplicative) == csmBlendMultiplicative)
        {
            unit[d].BlendMode = csmBlendMultiplicative;
        }


        unit[d].IsDoubleSided = (constantFlags[d] & csmIsDoubleSided) == csmIsDoubleSided;
        unit[d].IsInvertedMask = (constantFlags[d] & csmIsInvertedMask) == csmIsInvertedMask;
        unit[d].VertexCount = vertexCounts[d];
        unit[d].Positions.resize(vertexCounts[d]);
        unit[d].Uvs.resize(vertexCounts[d]);

        // Both VertexPositions and VertexUvs show informatin two-dimension.
        // vertexCounts shows the number of vertices, different from indices.
        for (int i = 0; i < vertexCounts[d]; ++i)
        {
            unit[d].Positions[i].X = vertexPositons[d][i].X;
            unit[d].Positions[i].Y = vertexPositons[d][i].Y;
            // Note that there is no Vertex Position but x and y
            unit[d].Positions[i].Z = 0;

            unit[d].Uvs[i].X = vertexUvs[d][i].X;
            unit[d].Uvs[i].Y = vertexUvs[d][i].Y;
        }

        // vertexIndices [d] are all triangular notation indexCounts [d] always gets a multiple number of 3.
        unit[d].IndexCount = indexCounts[d];
        unit[d].Indices = vertexIndices[d];    // Got as a single array

        // Register values such as VertexPositions, VertexUvs, vertexIndices, etc. in the graphics API
        // unit[d].Mesh = MakeMesh(drawables[d].VertexCount,
        //                              drawables[d].VertexPositions,
        //                              drawables[d].VertexUvs,
        //                              drawables[d].IndexCount,
        //                              drawables[d].Indices);


        // Access to other Drawable elements
        unit[d].ID  = ids[d];
        unit[d].DrawOrder = drawOrders[d];

        // The following three items are important on rendering.
        unit[d].Opacity  = opacities[d];
        unit[d].RenderOrder = renderOrders[d];
        unit[d].DynamicFlag = dynamicFlags[d];

        unit[d].MaskCount = maskCounts[d];
        unit[d].Masks.resize(maskCounts[d]);
        for (int m = 0; m < maskCounts[d]; ++m)
        {
            unit[d].Masks[m]  = masks[d][m];

            // Numbers in masks are index of Drawable
            unit[d].MaskLinks = &unit[(masks[d][m])];
        }
    }

}




// 完全匹配你代码调用的 ReadBlobAligned
void* Live_Model::ReadBlobAligned(const char* filePath, int alignment, unsigned int* outSize)
{
    // 1. 打开二进制文件
    FILE* file = fopen(filePath, "rb");
    if (!file)
    {
        *outSize = 0;
        return nullptr;
    }

    // 2. 获取文件大小
    fseek(file, 0, SEEK_END);
    unsigned int fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (fileSize == 0)
    {
        fclose(file);
        *outSize = 0;
        return nullptr;
    }

    // 3. 按指定字节数 **对齐分配内存**（Windows 专用）
    void* alignedBuffer = _aligned_malloc(fileSize, alignment);
    if (!alignedBuffer)
    {
        fclose(file);
        *outSize = 0;
        return nullptr;
    }

    // 4. 读取全部文件数据到对齐内存
    fread(alignedBuffer, 1, fileSize, file);

    // 5. 关闭文件
    fclose(file);

    // 输出文件大小
    *outSize = fileSize;

    // 返回对齐后的内存指针
    return alignedBuffer;
}



void Live_Model::UpdateModel()
{
    bool sort = false;
    for ( int d = 0; d < csmGetDrawableCount(m_model); d++)
    {
        auto dynamicFlags = csmGetDrawableDynamicFlags(m_model);
        auto isVisible = (dynamicFlags[d] & csmIsVisible) == csmIsVisible;

        //位置发生改变进行的vertexes修改
        if ((dynamicFlags[d] & csmVertexPositionsDidChange) ==   csmVertexPositionsDidChange)
        {
            /*update vertexes*/


        }
        // Check whether drawables need to be sorted.
        sort = sort || ((dynamicFlags[d] & csmRenderOrderDidChange) == csmRenderOrderDidChange);
    }
    if (sort)
    {
        /*   render order need sort    */
    }
}





void Live_Model::Maskcompute( QVector<DrawABLE> &m_units)
{
    int drawableCount = csmGetDrawableCount(m_model);
    const int *maskCount = csmGetDrawableMaskCounts(m_model);
    const int **masks = csmGetDrawableMasks(m_model);
    const csmFlags *dynamicFlags = csmGetDrawableDynamicFlags(m_model);
    const csmFlags *constantFlags = csmGetDrawableConstantFlags(m_model);

    for (int d = 0; d < drawableCount; ++d)
    {

        auto target = m_units[d].RenderOrder;
        if (maskCount[d] > 0){
            /* Rendering when a mask exists. */
            /* Reset mask buffe */
  //           ResetMaskBuffer();

  //           /* Change rendering target to mask buffa. */
  //           RenderTarget(MASK);

  //           SetRenderingOpacity(1.0f);              //Opacity needs to be fixed as 1.
  //           SetRenderingMode(RENDER_MODE_NORMAL);   //the method of synthesis needs to be
  //           fixed as Normal.

            for (int i = 0; i < maskCount[target]; ++i)
            {
                int maskDrawableIndex = masks[target][i];
                /* If maskDrawableIndex gets -1, the Drawable is not exported since it is hidden for example.
             * In this case, rendering mask needs to be skipped.  */
                if( maskDrawableIndex == -1 )
                {
                    continue;
                }

                /* If csmVertexPositionsDidChange of DynamicFlag of mask is not put up vertex information is not available.
               *  In this case rendering mask needs to be skipped by continue. */
                if ((dynamicFlags[maskDrawableIndex] & csmVertexPositionsDidChange) !=
                    csmVertexPositionsDidChange)
                {
                    continue;
                }

                DrawABLE maskingDrawable = m_units[maskDrawableIndex];
                /* Setting for mask needs to be used for setting of Culling and texture. */
                // SetCulling(maskingDrawable.culling);
                // SetMainTexture(maskingDrawable.texture);
                // /* Rendering */
                // DrawElements();
            }
            /* Get rendering target to the normal buffer. */
            RenderTarget(MAIN);

            /* Specify each item of rendering of Drawable */
            DrawABLE targetDrawable = m_units[target];
            SetRenderingOpacity(targetDrawable.Opacity);
            SetRenderingMode(targetDrawable.RenderOrder);
            SetCulling(targetDrawable.culling);
            SetMainTexture(targetDrawable.texture);

            bool isInvertedMask = (constantFlags[target] & csmIsInvertedMask) != csmIsInvertedMask;
            /* Specify items which will use (if shader is different it needs to be specified on this step) */
            /* Change the shader depending on inverting the mask or not */
            SetMaskTexture(MASK, isInvertedMask);
            /* Rendering */
            DrawElements();
        }
        else
        {
            /*Rendering without mask*/
            /* Specify each item for rendering of Drawable. */
            DrawABLE targetDrawable = m_units[target];
            SetRenderingOpacity(targetDrawable.Opacity);
            SetRenderingMode(targetDrawable.RenderOrder);
            SetCulling(targetDrawable.culling);
            SetMainTexture(targetDrawable.texture);

            /* Specify "not" use of mask. */
            SetMaskTexture(NULL);

            /* rendering */
            DrawElements();
        }
    }
}



void* Live_Model::AllocateAligned(unsigned int size, unsigned int alignment)
{
    // Windows 下使用 _aligned_malloc
    return _aligned_malloc(size, alignment);
}
