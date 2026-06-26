/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include "LAppLive2DManager.hpp"
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Rendering/CubismRenderer.hpp>
#include <Rendering/OpenGL/CubismOffscreenManager_OpenGLES2.hpp>
#include <ConfigManager/configini.h>
#include "LAppPal.hpp"
#include "LAppDefine.hpp"
#include "LAppDelegate.hpp"
#include "LAppModel.hpp"
#include "LAppView.hpp"

using namespace Csm;
using namespace LAppDefine;
using namespace std;

namespace {
    LAppLive2DManager* s_instance = NULL;

    void BeganMotion(ACubismMotion* self)
    {
        LAppPal::PrintLogLn("Motion Began: %x", self);
    }

    void FinishedMotion(ACubismMotion* self)
    {
        LAppPal::PrintLogLn("Motion Finished: %x", self);
    }

    int CompareCsmString(const void* a, const void* b)
    {
        return strcmp(reinterpret_cast<const Csm::csmString*>(a)->GetRawString(),
            reinterpret_cast<const Csm::csmString*>(b)->GetRawString());
    }
}

LAppLive2DManager* LAppLive2DManager::GetInstance()
{
    if (s_instance == NULL)
    {
        s_instance = new LAppLive2DManager();
    }

    return s_instance;
}

void LAppLive2DManager::ReleaseInstance()
{
    if (s_instance != NULL)
    {
        delete s_instance;
    }

    s_instance = NULL;
}

LAppLive2DManager::LAppLive2DManager()
    : _viewMatrix(NULL)
    , _sceneIndex(0)
{
    _viewMatrix = new CubismMatrix44();
    SetUpModelNew();
    ChangeScene(_sceneIndex);

}

LAppLive2DManager::~LAppLive2DManager()
{
    ReleaseAllModel();
    delete _viewMatrix;
    Csm::Rendering::CubismOffscreenManager_OpenGLES2::ReleaseInstance();
}

void LAppLive2DManager::ReleaseAllModel()
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        delete _models[i];
    }

    _models.Clear();
}

void LAppLive2DManager::SetUpModel()
{
    // ResourcesPathの中にあるフォルダ名を全てクロールし、モデルが存在するフォルダを定義する。
    // フォルダはあるが同名の.model3.jsonが見つからなかった場合はリストに含めない。
    // 一部文字が受け取れないためワイド文字で受け取ってUTF8に変換し格納する。
    //qDebug()<<ResourcesPath;

    csmString crawlPath(ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString().toUtf8());
    crawlPath += "/";
    crawlPath += "*.*";
    qDebug()<<crawlPath.GetRawString();
    wchar_t wideStr[MAX_PATH];
    csmChar name[MAX_PATH];
    LAppPal::ConvertMultiByteToWide(crawlPath.GetRawString(), wideStr, MAX_PATH);

    struct _wfinddata_t fdata;
    intptr_t fh = _wfindfirst(wideStr, &fdata);
    if (fh == -1)
    {
        return;
    }

    _modelDir.Clear();
    try{
        while (_wfindnext(fh, &fdata) == 0)
        {
            if ((fdata.attrib & _A_SUBDIR) && wcscmp(fdata.name, L"..") != 0)
            {
                LAppPal::ConvertWideToMultiByte(fdata.name, name, MAX_PATH);

                // フォルダと同名の.model3.jsonがあるか探索する
                csmString model3jsonPath(ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString().toUtf8());
                model3jsonPath.Append(1, '/');
                model3jsonPath += name;
                // model3jsonPath.Append(1, '/');
                // model3jsonPath += name;
                model3jsonPath += ".model3.json";
                qDebug()<< model3jsonPath.GetRawString();
                LAppPal::ConvertMultiByteToWide(model3jsonPath.GetRawString(), wideStr, MAX_PATH);

                struct _wfinddata_t fdata2;
                if (_wfindfirst(wideStr, &fdata2) != -1)
                {
                    _modelDir.PushBack(csmString(name));
                }
            }
        }
    }catch(const _exception& error){
        qDebug()<<"setup error";
    }

    qsort(_modelDir.GetPtr(), _modelDir.GetSize(), sizeof(csmString), CompareCsmString);
}




bool LAppLive2DManager::Add_SetUpModel(QString path)
{
    //QString path = ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString();
    // if(!ProgramUtils::Model3jsonfile_check(path))return false;
    string modelFolder = path.toUtf8().constData();

    // 2. 拼接搜索路径：目录/*.model3.json
    string searchPath = modelFolder + "/*.model3.json";

    wchar_t widePath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, searchPath.c_str(), -1, widePath, MAX_PATH);

    // 3. 开始查找文件
    _wfinddata_t fdata;
    intptr_t fh = _wfindfirst(widePath, &fdata);

    if (fh == -1) {
        return false;
    }

    //_modelDir.Clear();
    qDebug()<<"加载文件";
    try {
        do {
            // 跳过目录，只处理文件
            if (!(fdata.attrib & _A_SUBDIR))
            {
                // 文件名转窄字符串
                char fileName[MAX_PATH];
                WideCharToMultiByte(CP_UTF8, 0, fdata.name, -1, fileName, MAX_PATH, NULL, NULL);

                string nameStr = fileName;

                // 4. 提取文件名（去掉后缀 .model3.json）
                size_t dotPos = nameStr.find(".model3.json");
                if (dotPos != string::npos) {
                    string modelName = nameStr.substr(0, dotPos);
                    _modelfolderDir.PushBack(csmString(modelFolder.c_str()));
                    _modelDir.PushBack(csmString(modelName.c_str()));
                }
            }
        } while (_wfindnext(fh, &fdata) == 0);
        _findclose(fh);
        qsort(_modelDir.GetPtr(), _modelDir.GetSize(), sizeof(csmString), CompareCsmString);
        qDebug()<<_modelDir.GetSize();
        return true;
    }
    catch (...) {
        qDebug() << "setup error";
        return false;
    }


}



csmVector<csmString> LAppLive2DManager::GetModelDir() const
{
    return _modelDir;
}

csmInt32 LAppLive2DManager::GetModelDirSize() const
{
    return _modelDir.GetSize();
}

LAppModel* LAppLive2DManager::GetModel(csmUint32 no) const
{
    if (no < _models.GetSize())
    {
        return _models[no];
    }

    return NULL;
}

void LAppLive2DManager::SetRenderTargetSize(csmUint32 width, csmUint32 height)
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        LAppModel* model = GetModel(i);

        model->SetRenderTargetSize(width, height);
    }
}

void LAppLive2DManager::OnDrag(csmFloat32 x, csmFloat32 y) const
{
    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        LAppModel* model = GetModel(i);

        model->SetDragging(x, y);
    }
}

void LAppLive2DManager::OnTap(csmFloat32 x, csmFloat32 y)
{
    if (DebugLogEnable)
    {
        LAppPal::PrintLogLn("[APP]tap point: {x:%.2f y:%.2f}", x, y);
    }

    for (csmUint32 i = 0; i < _models.GetSize(); i++)
    {
        if (_models[i]->HitTest(HitAreaNameHead, x, y))
        {
            if (DebugLogEnable)
            {
                LAppPal::PrintLogLn("[APP]hit area: [%s]", HitAreaNameHead);
            }
            _models[i]->SetRandomExpression();
        }
        else if (_models[i]->HitTest(HitAreaNameBody, x, y))
        {
            if (DebugLogEnable)
            {
                LAppPal::PrintLogLn("[APP]hit area: [%s]", HitAreaNameBody);
            }
            _models[i]->StartRandomMotion(MotionGroupTapBody, PriorityNormal, FinishedMotion, BeganMotion);
        }
    }
}




bool LAppLive2DManager::isHit(const csmChar* area,csmFloat32 x, csmFloat32 y)
{
    if (DebugLogEnable)
    {
        LAppPal::PrintLogLn("[APP]tap point: {x:%.2f y:%.2f}", x, y);
    }

    return _models[0]->HitTest(area, x, y);
}


void LAppLive2DManager::OnUpdate() const
{
    int width, height;
    //glfwGetWindowSize(LAppDelegate::GetInstance()->GetWindow(), &width, &height);
    width = LAppDelegate::GetInstance()->GetWindow()->width();
    height = LAppDelegate::GetInstance()->GetWindow()->height();
    // モデルで使用するオフスクリーン管理の開始処理
    Csm::Rendering::CubismOffscreenManager_OpenGLES2::GetInstance()->BeginFrameProcess();

    csmUint32 modelCount = _models.GetSize();
    for (csmUint32 i = 0; i < modelCount; ++i)
    {
        CubismMatrix44 projection;
        LAppModel* model = GetModel(i);

        if (model->GetModel() == NULL)
        {
            LAppPal::PrintLogLn("Failed to model->GetModel().");
            continue;
        }

        if (model->GetModel()->GetCanvasWidth() > 1.0f && width < height)
        {
            // 横に長いモデルを縦長ウィンドウに表示する際モデルの横サイズでscaleを算出する
            model->GetModelMatrix()->SetWidth(2.0f);
            projection.Scale(1.0f, static_cast<float>(width) / static_cast<float>(height));
        }
        else
        {
            projection.Scale(static_cast<float>(height) / static_cast<float>(width), 1.0f);
        }

        // 必要があればここで乗算
        if (_viewMatrix != NULL)
        {
            projection.MultiplyByMatrix(_viewMatrix);
        }

        // モデル1体描画前コール
        LAppDelegate::GetInstance()->GetView()->PreModelDraw(*model);

        model->Update();
        model->Draw(projection);///< 参照渡しなのでprojectionは変質する

        // モデル1体描画後コール
        LAppDelegate::GetInstance()->GetView()->PostModelDraw(*model);
    }

    // モデルで使用するオフスクリーン管理の終了処理
    Csm::Rendering::CubismOffscreenManager_OpenGLES2::GetInstance()->EndFrameProcess();
    // もし余っているオフスクリーンのリソースを解放したい場合行う処理
    Csm::Rendering::CubismOffscreenManager_OpenGLES2::GetInstance()->ReleaseStaleRenderTextures();
}

void LAppLive2DManager::NextScene()
{
    csmInt32 no = (_sceneIndex + 1) % GetModelDirSize();
    ChangeScene(no);
}

void LAppLive2DManager::ChangeScene(Csm::csmInt32 index)
{
    _sceneIndex = index;
    if (DebugLogEnable)
    {
        LAppPal::PrintLogLn("[APP]model index: %d", _sceneIndex);
    }

    // model3.jsonのパスを決定する.
    // ディレクトリ名とmodel3.jsonの名前を一致していることが条件
    const csmString& modelFolder = _modelfolderDir[index];
    const csmString& model = _modelDir[index];

    csmString modelPath(modelFolder);
    modelPath.Append(1, '/');

    csmString modelJsonName(model);
    modelJsonName += ".model3.json";
    qDebug()<<modelJsonName.GetRawString();
    ReleaseAllModel();
    _models.PushBack(new LAppModel());
    _models[0]->LoadAssets(modelPath.GetRawString(), modelJsonName.GetRawString());

    /*
     * モデル半透明表示を行うサンプルを提示する。
     * ここでUSE_RENDER_TARGET、USE_MODEL_RENDER_TARGETが定義されている場合
     * 別のレンダリングターゲットにモデルを描画し、描画結果をテクスチャとして別のスプライトに張り付ける。
     */
    {
#if defined(USE_RENDER_TARGET)
        // LAppViewの持つターゲットに描画を行う場合、こちらを選択
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ViewFrameBuffer;
#elif defined(USE_MODEL_RENDER_TARGET)
        // 各LAppModelの持つターゲットに描画を行う場合、こちらを選択
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ModelFrameBuffer;
#else
        // デフォルトのメインフレームバッファへレンダリングする(通常)
        LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_None;
#endif

#if defined(USE_RENDER_TARGET) || defined(USE_MODEL_RENDER_TARGET)
        // モデル個別にαを付けるサンプルとして、もう1体モデルを作成し、少し位置をずらす
        _models.PushBack(new LAppModel());
        _models[1]->LoadAssets(modelPath.GetRawString(), modelJsonName.GetRawString());
        _models[1]->GetModelMatrix()->TranslateX(0.2f);
#endif

        LAppDelegate::GetInstance()->GetView()->SwitchRenderingTarget(useRenderTarget);

        // 別レンダリング先を選択した際の背景クリア色
        float clearColor[3] = { 0.0f, 0.0f, 0.0f };
        LAppDelegate::GetInstance()->GetView()->SetRenderTargetClearColor(clearColor[0], clearColor[1], clearColor[2]);
    }
}




// void LAppLive2DManager::ChangeSceneNew()
// {

//     string modelFolder = ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString().toUtf8().constData();
//     const csmString& model = _modelDir[0];

//     csmString modelPath(modelFolder.c_str());
//     modelPath.Append(1, '/');

//     csmString modelJsonName(model);
//     modelJsonName += ".model3.json";

//     //ReleaseAllModel();
//     _models.PushBack(new LAppModel());
//     _models[0]->LoadAssets(modelPath.GetRawString(), modelJsonName.GetRawString());

//     /*
//      * モデル半透明表示を行うサンプルを提示する。
//      * ここでUSE_RENDER_TARGET、USE_MODEL_RENDER_TARGETが定義されている場合
//      * 別のレンダリングターゲットにモデルを描画し、描画結果をテクスチャとして別のスプライトに張り付ける。
//      */
//     {
// #if defined(USE_RENDER_TARGET)
//         // LAppViewの持つターゲットに描画を行う場合、こちらを選択
//         LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ViewFrameBuffer;
// #elif defined(USE_MODEL_RENDER_TARGET)
//         // 各LAppModelの持つターゲットに描画を行う場合、こちらを選択
//         LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_ModelFrameBuffer;
// #else
//         // デフォルトのメインフレームバッファへレンダリングする(通常)
//         LAppView::SelectTarget useRenderTarget = LAppView::SelectTarget_None;
// #endif

// #if defined(USE_RENDER_TARGET) || defined(USE_MODEL_RENDER_TARGET)
//         // モデル個別にαを付けるサンプルとして、もう1体モデルを作成し、少し位置をずらす
//         _models.PushBack(new LAppModel());
//         _models[1]->LoadAssets(modelPath.GetRawString(), modelJsonName.GetRawString());
//         _models[1]->GetModelMatrix()->TranslateX(0.2f);
// #endif

//         LAppDelegate::GetInstance()->GetView()->SwitchRenderingTarget(useRenderTarget);

//         // 別レンダリング先を選択した際の背景クリア色
//         float clearColor[3] = { 0.0f, 0.0f, 0.0f };
//         LAppDelegate::GetInstance()->GetView()->SetRenderTargetClearColor(clearColor[0], clearColor[1], clearColor[2]);
//     }
// }




csmUint32 LAppLive2DManager::GetModelNum() const
{
    return _models.GetSize();
}

void LAppLive2DManager::SetViewMatrix(CubismMatrix44* m)
{
    for (int i = 0; i < 16; i++) {
        _viewMatrix->GetArray()[i] = m->GetArray()[i];
    }
}

bool LAppLive2DManager::SetUpModelNew()
{
    QString path = ConfigManager::instance().getValue(CONFIG_MODEL_FOLDER).toString();
    qDebug()<<path;
    // if(!ProgramUtils::Model3jsonfile_check(path))return false;
    string modelFolder = path.toUtf8().constData();

    // 2. 拼接搜索路径：目录/*.model3.json
    string searchPath = modelFolder + "/*.model3.json";

    wchar_t widePath[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, searchPath.c_str(), -1, widePath, MAX_PATH);

    // 3. 开始查找文件
    _wfinddata_t fdata;
    intptr_t fh = _wfindfirst(widePath, &fdata);

    if (fh == -1) {
        return false;
    }

    _modelDir.Clear();
    qDebug()<<"加载文件";
    try {
        do {
            // 跳过目录，只处理文件
            if (!(fdata.attrib & _A_SUBDIR))
            {
                // 文件名转窄字符串
                char fileName[MAX_PATH];
                WideCharToMultiByte(CP_UTF8, 0, fdata.name, -1, fileName, MAX_PATH, NULL, NULL);

                string nameStr = fileName;

                // 4. 提取文件名（去掉后缀 .model3.json）
                size_t dotPos = nameStr.find(".model3.json");
                if (dotPos != string::npos) {
                    string modelName = nameStr.substr(0, dotPos);
                    _modelfolderDir.PushBack(csmString(modelFolder.c_str()));
                    _modelDir.PushBack(csmString(modelName.c_str()));
                }
            }
        } while (_wfindnext(fh, &fdata) == 0);
        _findclose(fh);
        qsort(_modelDir.GetPtr(), _modelDir.GetSize(), sizeof(csmString), CompareCsmString);
        return true;
    }
    catch (...) {
        qDebug() << "setup error";
        return false;
    }
}
