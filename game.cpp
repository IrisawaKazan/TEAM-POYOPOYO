//************************************************************
//
//　ゲームの処理[game.cpp]
//　Author:Hatori Kensaku
//
//************************************************************

// インクルード
#include "game.h"
#include "playermanager.h"
#include "manager.h"
#include "object2D.h"
#include "pausemanager.h"
#include "math.h"
#include "object3D.h"
#include "uplift.h"
#include "navi.h"
#include "naviUi.h"
#include "block.h"
#include "timer.h"
#include "result.h"
#include "fade.h"
#include "gimmick.h"
#include "switch.h"
#include "mapmanager.h"
#include "item.h"
#include "tutorialBoard.h"
#include "particle3d.h"
#include "effect3d.h"
#include "gemespeedUI.h"

// 規定値を設定
// プレイヤー
const D3DXVECTOR3 CGame::Config::Player::Pos = { 120.0f,0.0f,-1700.0f };
const D3DXVECTOR3 CGame::Config::Player::Rot = { 0.0f,D3DX_PI,0.0f };
// カメラ
const D3DXVECTOR3 CGame::Config::Camera::Rot = { 0.0f,D3DX_PI,0.0f };
const D3DXVECTOR3 CGame::Config::Camera::PosR = { 120.0f,50.0f,-1700.0f };
const D3DXVECTOR3 CGame::Config::Camera::PosV = { 119.9f,60.0f,-1900.0f };
// 空
const D3DXVECTOR3 CGame::Config::Sky::Pos = VEC3_NULL;

// 静的メンバ変数
CPauseManager* CGame::m_pPauseManager = NULL;
CPlayerManager* CGame::m_pPlayerManager = NULL;
CMapManager* CGame::m_pMapManager = NULL;
CTutorialBoard* CGame::m_pTutorialBoard = NULL;
CGameSpeedUI* CGame::m_pGameSpeedUI = NULL;
CTimer* CGame::pTimer = NULL; // タイマー sato
bool CGame::m_bGoal = false;

using namespace std;

//***************************************
// コンストラクタ
//***************************************
CGame::CGame() : CScene(MODE_GAME)
{
}

//***************************************
// デストラクタ
//***************************************
CGame::~CGame()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CGame::Init(void)
{
	m_bGoal = false;

	// プレイヤーマネジャー
	m_pPlayerManager = new CPlayerManager;

	m_pPlayerManager->Init();

	// シングルトンを生成
	m_pPauseManager = CPauseManager::CreateSingleton();

	m_pMapManager = CMapManager::Instance();
	m_pMapManager->Load("data\\TEXT\\stage99.json");

	// カメラの初期化
	CManager::GetCamera()->Init();
	CManager::GetCamera()->SetMovie(CCamera::MOTIONTYPE_STARTMOVIE);

	// チュートリアルの生成
	m_pTutorialBoard = CTutorialBoard::Create();

	// 倍速UIの生成 Misaki
	m_pGameSpeedUI = CGameSpeedUI::Create();

	// スタートオブジェクト
	m_pBlock = CBlock::Create("data\\Model\\mine_shaft.x", { 1800.0f,0.0f,-900.0f }, { 0.0f, -D3DX_PI * 0.5f,0.0f }, { 1.0f,1.0f,1.0f });

	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);

	//CTimer::Instance()->SetPosition(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	pTimer = CTimer::Create(D3DXVECTOR3(screenSize.x * 0.1f, screenSize.y * 0.05f, 0.0f), 2, 0, TIME_LIMIT);

	//// 火の情報を設定
	//CParticle3D::DefoultEffectInfo FireInfo;
	//FireInfo.Bece.Col = FIRE;
	//FireInfo.Bece.fMaxSpeed = 1.0f;
	//FireInfo.Bece.fMinSpeed = 1.0f;
	//FireInfo.Bece.MaxDir = { 0.1f,0.5f,0.1f };
	//FireInfo.Bece.MinDir = { -0.1f,0.5f,-0.1f };
	//FireInfo.Bece.nLife = 1;
	//FireInfo.Bece.nMaxLife = 30;
	//FireInfo.Bece.nMinLife = 30;
	//FireInfo.Bece.nNumEffect = 5;
	//FireInfo.Bece.Pos = VEC3_NULL;
	//FireInfo.MaxRadius = 15.0f;
	//FireInfo.MinRadius = 15.0f;
	//FireInfo.Bece.bLoop = true;
	//FireInfo.Bece.nCoolDown = 2;
	//FireInfo.Bece.Gravity = 0.0f;
	//memcpy(FireInfo.Bece.FilePath, CEffect3D::Config::Smoke, sizeof(FireInfo.Bece.FilePath));
	//FireInfo.Bece.nPriority = 1;

	//// 生成
	//CParticle3D::Create(FireInfo);

	//// アイテムの生成処理 Misaki
	//CItem::Create(CItem::ITEM_LEFT, D3DXVECTOR3(0.0f, 50.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(1.2f, 1.2f, 1.2f), "data\\Model\\item\\item000.x");

#ifdef _DEBUG
#else
#endif // _DEBUG

	//{
	//	m_GroundShape = make_unique<btBoxShape>(btVector3(btScalar(2000.0f), btScalar(10), btScalar(1000.0f)));

	//	btTransform groundTransform;
	//	groundTransform.setIdentity();
	//	groundTransform.setOrigin(btVector3(0, -10, 0));

	//	btScalar mass(0);

	//	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	//	bool isDynamic = (mass != 0.f);

	//	btVector3 localInertia(0, 0, 0);
	//	if (isDynamic)
	//		m_GroundShape->calculateLocalInertia(mass, localInertia);

	//	//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
	//	btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
	//	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_GroundShape.get(), localInertia);
	//	m_RigitBody = make_unique<btRigidBody>(rbInfo);

	//	//add the body to the dynamics world
	//	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
	//}

	// サウンドの取得
	CSound* pSound = CManager::GetSound();

	// BGM
	pSound->Play(CSound::LABEL_GAME_BGM);

	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CGame::Update(void)
{
//#ifdef _DEBUG
	int GameSpeed = CManager::GetGameSpeed();
	bool bSpeedUP = m_pGameSpeedUI->GetSpeedUP();	// Misaki

	if (CManager::GetInputKeyboard() != NULL)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_X) ||
			CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_Y) == true && m_pTutorialBoard->GetProgress() == false)
		{
			CManager::SetGameSpeed(Wrap(GameSpeed + 1, 1, 2));

			// 倍速を切り替え Misaki
			m_pGameSpeedUI->SetSpeed(bSpeedUP ? false : true);
		}
	}
//#endif // DEBUG

	if (m_pMapManager != nullptr)
	{
		m_pMapManager->Update();
	}
	if (m_pPauseManager != NULL)
	{
		m_pPauseManager->Update();
	}
	if (m_pPlayerManager != NULL)
	{
		m_pPlayerManager->Update();
	}
	if (m_pTutorialBoard != NULL)
	{
		m_pTutorialBoard->Update();
	}

	//if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
	//{
	//	//CManager::GetSound()->Play(CSound::LABEL_ENTER);
	//	m_bGoal = true;
	//	CFade::SetFade(new CResult);
	//}

	if (pTimer->IsTimeOver())
	{// タイムオーバー
		CFade::SetFade(new CResult);
	}

	// レイキャストオブジェクトに登録 sato
	CModelManager* pModelManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelManager->GetAddress(m_pBlock->GetIndx());
	CNavi::GetInstance()->RegisterLatentObject(modelinfo.pMesh, m_pBlock->GetWorldMtx());
}

//***************************************
// 終了処理
//***************************************
void CGame::Uninit(void)
{
	// ナビゲーションのリムーブ
	CNavi::GetInstance()->remove();
	CManager::SetGameSpeed(1);

	// プレイヤーマネージャーの破棄
	if (m_pPlayerManager != NULL)
	{
		m_pPlayerManager->Uninit();
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}

	if (m_pPauseManager != NULL)
	{
		m_pPauseManager->Uninit();
		m_pPauseManager = NULL;
	}

	if (m_pMapManager != nullptr)
	{
		m_pMapManager->Uninit();
	}

	if (m_pTutorialBoard != nullptr)
	{
		m_pTutorialBoard->Uninit();
		m_pTutorialBoard = nullptr;
	}

	delete this;
}

//***************************************
// 描画処理
//***************************************
void CGame::Draw(void)
{
}

//***************************************
// プレイヤーをリスポーンさせる
//***************************************
void CGame::ResetPlayer(void)
{
}

//***************************************
// Naviたちを設定
//***************************************
void CGame::SetNavis(void)
{
	// Navi生成
	CNavi::GetInstance()->set();

	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);
	CNaviUI::Create("data/TEXTURE/UI/Frame001.png", NAVI_UI_TEXTURES, NAVI_UI_ARROW_TEXTURES, NAVI_UI_KEY_TEXTURES, D3DXVECTOR3(screenSize.x * 0.175f, screenSize.y * 0.82f, 0.0f), D3DXVECTOR2(screenSize.x * 0.1f, screenSize.x * 0.1f));
}