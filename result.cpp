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
#include "file.h"
#include "timer.h"

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
    D3DXVECTOR2 screenSize{};
    CManager::GetRenderer()->GetBackBufferSize(&screenSize);
    CRanking::Create(D3DXVECTOR3(screenSize.x * 0.75f, screenSize.y * 0.5f, 0.0f), D3DXVECTOR2(screenSize.x * 0.5f, screenSize.y), 2u);

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
        // 生成
        m_GameOver = CObject2D::Create({ CManager::GetRenderer()->GetSenterPos().x,CManager::GetRenderer()->GetSenterPos().y - 400.0f ,0.0f }, {0.0f,0.0f,D3DX_PI * -0.05f});
        // テクスチャ設定
        m_GameOver->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\game_over.png"));
        // 大きさ設定
        m_GameOver->SetSize({ m_GameOver->GetSize().x + 300.0f,m_GameOver->GetSize().y });
    }
    return S_OK;
}

//  更新
void CResult::Update(void)
{
    if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) || CManager::GetInputMouse()->OnDown(0) == true)
    {
        //CManager::GetSound()->Play(CSound::LABEL_ENTER);
        CFade::SetFade(new CTitle);
    }
    if (m_GameOver == nullptr) return;

    // 位置を取得
    D3DXVECTOR3 Pos = m_GameOver->GetPosition();
    // ずらす
    Pos.y+=10;
    // 設定
    m_GameOver->SetPosition(Pos);
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

//****************************************************************
// ファイル読み込み
//****************************************************************
void CResult::LoadFile(void)
{
    CFile* pFile = new CFile(CTimer::FILE_PATH);
    const auto timeOver = pFile->ReadBinary<bool>();
    if (timeOver.has_value())
    {// 読み取り成功している
        m_IsGoal = !timeOver.value(); // タイムオーバーじゃない = ゴール
    }
    else
    {
        m_IsGoal = false;
    }
    delete pFile;
}