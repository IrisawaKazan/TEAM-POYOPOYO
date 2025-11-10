//************************************************************
//
//　リザルトの処理[rezult.cpp]
//　Author:Hatori Kensaku
//
//************************************************************
//  インクルード
#include "result.h"
#include "object2D.h"
#include "ranking.h"

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
    m_pRanking = new CRanking;

    //  CObject2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f), VEC3_NULL);
    m_pRanking->Init();
    
    return E_NOTIMPL;
}

//  更新
void CResult::Update(void)
{
    m_pRanking->Update();
}

//  終了
void CResult::Uninit(void)
{
    m_pRanking->Uninit();
    delete this;
}

//  描画
void CResult::Draw(void)
{
    m_pRanking->Draw();
}
