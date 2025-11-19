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
#include "player.h"
#include "mapmanager.h"

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
    
    CMapManager::Instance()->Load("data\\TEXT\\stage98.json");

    CManager::GetCamera()->Init();
    CManager::GetCamera()->SetPosRDest({0.0f,35.0f,0.0f});
    CManager::GetCamera()->SetMode(CCamera::MODE::NORMAL);
    CManager::GetCamera()->SetRot({ 0.0f,0.0f,0.0f });
    CManager::GetCamera()->SetDistance(200.0f);
    CPlayer::Create({50.0f,20.0f,0.0f }, {0.0f,-D3DX_PI,0.0f})->GetMotionInfo()->SetMotion(6, false);

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