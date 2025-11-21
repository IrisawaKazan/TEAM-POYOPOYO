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

using namespace std;

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
    //  CObject2D::Create(D3DXVECTOR3(640.0f, 370.0f, 0.0f), VEC3_NULL);
    CRanking::Instance()->Init();
    
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
    CRanking::Instance()->Update();

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
    {
        //CManager::GetSound()->Play(CSound::LABEL_ENTER);
        CFade::SetFade(new CTitle);
    }
}

//  終了
void CResult::Uninit(void)
{
    CRanking::Instance()->Uninit();
    delete this;
}

//  描画
void CResult::Draw(void)
{
    CRanking::Instance()->Draw();
}

//****************************************************************
// ファイル読み込み
//****************************************************************
void CResult::LoadFile(void)
{
    ifstream pFile("data\\Goal.txt");
    string line = {};

    // ファイルが正常に開けたら
    if (pFile.is_open())
    {
            istringstream iss(line);

            iss >> m_IsGoal;

        // ファイルを閉じる
        pFile.close();
    }
}