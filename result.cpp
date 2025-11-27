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
        // 祝砲の情報を設定
        CParticle3D::DefoultEffectInfo HappyMisile;
        HappyMisile.Bece.Col = D3DXCOLOR(0.9f, 1.0f, 0.4f, 1.0f);
        HappyMisile.Bece.fMaxSpeed = 3.0f;
        HappyMisile.Bece.fMinSpeed = 3.0f;
        HappyMisile.Bece.MaxDir = { -0.5f,1.0f,0.1f };
        HappyMisile.Bece.MinDir = { -0.5f,0.5f,-0.1f };
        HappyMisile.Bece.nLife = 1;
        HappyMisile.Bece.nMaxLife = 60;
        HappyMisile.Bece.nMinLife = 60;
        HappyMisile.Bece.nNumEffect = 30;
        HappyMisile.Bece.Pos = { 150.0f,0.0f,0.0f };
        HappyMisile.MaxRadius = 5.0f;
        HappyMisile.MinRadius = 5.0f;
        HappyMisile.Bece.bLoop = true;
        HappyMisile.Bece.nCoolDown = 60;
        HappyMisile.Bece.Gravity = -0.1f;
        memcpy(HappyMisile.Bece.FilePath, CEffect3D::Config::Sphere, sizeof(HappyMisile.Bece.FilePath));
        HappyMisile.Bece.nPriority = 5;

        // 生成
        CParticle3D::Create(HappyMisile);

        HappyMisile.Bece.MaxDir = { 0.5f,1.0f,0.1f };
        HappyMisile.Bece.MinDir = { 0.5f,0.5f,-0.1f };
        HappyMisile.Bece.Pos = { -150.0f,0.0f,0.0f };

        // 生成
        CParticle3D::Create(HappyMisile);
    }
    else
    {
        CObject2D* Info;
        Info = CObject2D::Create({ CManager::GetRenderer()->GetSenterPos().x,CManager::GetRenderer()->GetSenterPos().y - 400.0f ,0.0f }, VEC3_NULL);
        Info->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\game_over.png"));
        Info->SetSize({ Info->GetSize().x + 300.0f,Info->GetSize().y });
    }
    return E_NOTIMPL;
}

//  更新
void CResult::Update(void)
{
    CRanking::Instance()->Update();

    if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) || CManager::GetInputMouse()->OnDown(0) == true)
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