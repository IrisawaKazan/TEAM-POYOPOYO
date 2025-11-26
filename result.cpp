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
#include "particle3d.h"
#include "effect3d.h"

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
    
    LoadFile();

    int nMotion = 0;
    std::string Path;
    if (m_IsGoal == true) nMotion = 6, Path = "data\\TEXT\\stage98.json";
    else nMotion = 7, Path = "data\\TEXT\\stage97.json";

    CMapManager::Instance()->Load(Path);

    CManager::GetCamera()->Init();
    CManager::GetCamera()->SetPosRDest({0.0f,35.0f,0.0f});
    CManager::GetCamera()->SetMode(CCamera::MODE::NORMAL);
    CManager::GetCamera()->SetRot({ 0.0f,0.0f,0.0f });
    CManager::GetCamera()->SetDistance(200.0f);

    CPlayer::Create({50.0f,20.0f,0.0f }, {0.0f,-D3DX_PI,0.0f})->GetMotionInfo()->SetMotion(nMotion, false);

    if (m_IsGoal == true)
    {
        // 火の情報を設定
        CParticle3D::DefoultEffectInfo FireInfo;
        FireInfo.Bece.Col = D3DXCOLOR(0.9f, 1.0f, 0.4f, 1.0f);
        FireInfo.Bece.fMaxSpeed = 3.0f;
        FireInfo.Bece.fMinSpeed = 3.0f;
        FireInfo.Bece.MaxDir = { -0.5f,1.0f,0.1f };
        FireInfo.Bece.MinDir = { -0.5f,0.5f,-0.1f };
        FireInfo.Bece.nLife = 1;
        FireInfo.Bece.nMaxLife = 60;
        FireInfo.Bece.nMinLife = 60;
        FireInfo.Bece.nNumEffect = 30;
        FireInfo.Bece.Pos = { 150.0f,0.0f,0.0f };
        FireInfo.MaxRadius = 5.0f;
        FireInfo.MinRadius = 5.0f;
        FireInfo.Bece.bLoop = true;
        FireInfo.Bece.nCoolDown = 60;
        FireInfo.Bece.Gravity = -0.1f;
        memcpy(FireInfo.Bece.FilePath, CEffect3D::Config::Sphere, sizeof(FireInfo.Bece.FilePath));
        FireInfo.Bece.nPriority = 5;

        // 生成
        CParticle3D::Create(FireInfo);

        FireInfo.Bece.MaxDir = { 0.5f,1.0f,0.1f };
        FireInfo.Bece.MinDir = { 0.5f,0.5f,-0.1f };
        FireInfo.Bece.Pos = { -150.0f,0.0f,0.0f };

        // 生成
        CParticle3D::Create(FireInfo);
    }
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
        getline(pFile, line);

        istringstream iss(line);

        iss >> m_IsGoal;

        // ファイルを閉じる
        pFile.close();
    }
}