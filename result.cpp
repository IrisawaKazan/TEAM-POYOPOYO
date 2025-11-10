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
#include "fade.h"
#include "title.h"

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
    m_pRanking = CRanking::Instance();

    //  CObject2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f), VEC3_NULL);
    m_pRanking->Init();
    
    return E_NOTIMPL;
}

//  更新
void CResult::Update(void)
{
    m_pRanking->Update();

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
    {
        //CManager::GetSound()->Play(CSound::LABEL_ENTER);
        CFade::SetFade(new CTitle);
    }
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
