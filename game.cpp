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
	m_pMapManager->Load("data\\TEXT\\stage100.json");

	// カメラの初期化
	CManager::GetCamera()->Init();

	// ナビゲーションのセット
	CNavi::GetInstance()->set();

	m_pTutorialBoard = CTutorialBoard::Create();

	CObject3D::Create(D3DXVECTOR3(0.0f,1000.0f,-1000.0f), D3DXVECTOR3((D3DX_PI * 0.5f),0.0f,0.0f),"data\\TEXTURE\\wall.jpg",D3DXVECTOR2(2000.0f,1000.0f));
	CObject3D::Create(D3DXVECTOR3(-2000.0f,1000.0f,0.0f), D3DXVECTOR3((-D3DX_PI * 0.5f), (-D3DX_PI * 0.5f), 0.0f),"data\\TEXTURE\\wall.jpg",D3DXVECTOR2(1000.0f, 1000.0f));
	CObject3D::Create(D3DXVECTOR3(2000.0f,1000.0f,0.0f), D3DXVECTOR3((-D3DX_PI * 0.5f), (D3DX_PI * 0.5f), 0.0f), "data\\TEXTURE\\wall.jpg", D3DXVECTOR2(1000.0f, 1000.0f));

	CBlock::Create("data\\Model\\mine_shaft.x", { 2000.0f,0.0f,0.0f }, {0.0f, D3DX_PI,0.0f}, { 1.0f,1.0f,1.0f });

	CNaviUI::Create("data/TEXTURE/UI/Frame001.png", { NAVI_UI_TEXTURES.begin(), NAVI_UI_TEXTURES.end() }, D3DXVECTOR3(SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 0.82f, 0.0f), D3DXVECTOR2(100.0f, 100.0f));
	
	//CTimer::Instance()->SetPosition(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	CTimer::Create(D3DXVECTOR3(640.0f,360.0f,0.0f));

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

	return S_OK;
}

//***************************************
// 更新処理
//***************************************
void CGame::Update(void)
{
#ifdef _DEBUG
	if (CManager::GetInputKeyboard() != NULL)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_SPACE) == true)
		{
			if (CManager::GetScene()->GetMode() == MODE_GAME)
			{
				CFade::SetFade(new CResult);
			}
		}
	}
#endif // DEBUG
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
}

//***************************************
// 終了処理
//***************************************
void CGame::Uninit(void)
{
	// ナビゲーションのリムーブ
	CNavi::GetInstance()->remove();

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