//************************************************************
//
//　リザルトの処理[rezult.cpp]
//　Author:Hatori Kensaku
//
//************************************************************
//  インクルード
#include "result.h"
#include "object2D.h"

//  コンストラクタ
CResult::CResult()  :CScene(MODE_RESULT)
{

}

//  デストラクタ
CResult::~CResult()
{

}

//  初期化
HRESULT CResult::Init(void)
{
    CObject2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f), VEC3_NULL);

    return E_NOTIMPL;
}

//  更新
void CResult::Update(void)
{

}

//  終了
void CResult::Uninit(void)
{
    delete this;
}

//  描画
void CResult::Draw(void)
{

}
