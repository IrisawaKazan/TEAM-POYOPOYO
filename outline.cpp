//****************************************************************
//
// アウトラインシェーダー[COutLine.cpp]
// Author:Takahashi Misaki
//
//****************************************************************

// インクルード
#include "outline.h"

//***************************************
// デストラクタ
//***************************************
COutLine::~COutLine()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT COutLine::Init(std::string Path)
{
    // 初期化処理
    HRESULT hr = CShader::Init(Path);

    // 初期化失敗
    if (FAILED(hr)) return E_FAIL;

    // エフェクト取得
    LPD3DXEFFECT pEffect = GetEffect();

    // グローバル変数ハンドル取得
    GetHandle("g_mtxWorld") = pEffect->GetParameterByName(NULL, "g_mtxWorld");
    GetHandle("g_mtxView") = pEffect->GetParameterByName(NULL, "g_mtxView");
    GetHandle("g_mtxProj") = pEffect->GetParameterByName(NULL, "g_mtxProj");
    GetHandle("g_Color") = pEffect->GetParameterByName(NULL, "g_Color");
    GetHandle("g_Thickness") = pEffect->GetParameterByName(NULL, "g_Thickness");

    return S_OK;
}

//***************************************
// 開始
//***************************************
void COutLine::Begin(void)
{
    CShader::Begin();
}

//***************************************
// パスの開始
//***************************************
void COutLine::BeginPass(const UINT Pass)
{
    CShader::BeginPass(Pass);
}

//***************************************
// 終了
//***************************************
void COutLine::End(void)
{
    CShader::End();
}

//***************************************
// パスの終了
//***************************************
void COutLine::EndPass(void)
{
    CShader::EndPass();
}

//***************************************
// 定数バッファを設定
//***************************************
void COutLine::SetParameters(D3DXMATRIX World, D3DXMATRIX View, D3DXMATRIX Proj, const D3DXVECTOR4 Col, const float Thickness)
{
    // エフェクトを取得
    LPD3DXEFFECT pEffect = GetEffect();

    // パラメータ(グローバル変数の設定)
    if (GetHandle("g_mtxWorld") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxWorld"), &World);
    }
    if (GetHandle("g_mtxView") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxView"), &View);
    }
    if (GetHandle("g_mtxProj") != nullptr)
    {
        pEffect->SetMatrix(GetHandle("g_mtxProj"), &Proj);
    }
    if (GetHandle("g_Color") != nullptr)
    {
        pEffect->SetVector(GetHandle("g_Color"), &Col);
    }
    if (GetHandle("g_Thickness") != nullptr)
    {
        pEffect->SetFloat(GetHandle("g_Thickness"), Thickness);
    }

    // GPUに変更を適応
    pEffect->CommitChanges();
}