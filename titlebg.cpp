//****************************************************************
//
// タイトル背景の処理
// Author:Takahashi Misaki
//
//****************************************************************

// インクルード
#include "titlebg.h"

//***************************************
// コンストラクタ
//***************************************
CTitleBG::CTitleBG() : CObject2D(2)
{
}

//***************************************
// デストラクタ
//***************************************
CTitleBG::~CTitleBG()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitleBG::Init(void)
{
    // オブジェクト2Dの初期化処理
    CObject2D::Init();

    return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CTitleBG::Uninit(void)
{
    // オブジェクト2Dの終了処理
    CObject2D::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CTitleBG::Update(void)
{
    // オブジェクト2Dの更新処理
    CObject2D::Update();
}

//***************************************
// 描画処理
//***************************************
void CTitleBG::Draw(void)
{
    // オブジェクト2Dの描画処理
    CObject2D::Draw();
}

//***************************************
// 生成処理
//***************************************
CTitleBG* CTitleBG::Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, const D3DXVECTOR2 Size, const char* FilePath)
{
    // ポインタを宣言
    CTitleBG* pTitleBG = NULL;

    // メモリ確保
    pTitleBG = new CTitleBG;

    // 初期化処理
    pTitleBG->Init();

    // 各種処理
    pTitleBG->SetPosition(Pos);                                             // 位置
    pTitleBG->SetRotasion(Rot);                                             // 向き
    pTitleBG->SetSize(Size);                                                // 大きさ
    pTitleBG->SetCol(WHITE);                                                // 色
    pTitleBG->SetTexIndx(CTextureManager::Instance()->Register(FilePath));  // テクスチャの名前
    
    return pTitleBG;
}