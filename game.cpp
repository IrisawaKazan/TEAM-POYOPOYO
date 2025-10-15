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

// 規定値を設定
// 山
const D3DXVECTOR3 CGame::Config::MeshCylinder::Pos = {0.0f,700.0f,0.0f};
const D3DXVECTOR3 CGame::Config::MeshCylinder::Rot = { 0.0f,0.0f,0.0f };
// 川
const D3DXVECTOR3 CGame::Config::River::Pos = { 0.0f, -20.0f, -2250.0f };
const D3DXVECTOR2 CGame::Config::River::Size = { 2000.0f, 5000.0f };
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
D3DXVECTOR3 CGame::m_WildFirePos = { 143.0f,350.0f ,2500 };
bool CGame::m_isPause = false;

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
	m_pPlayerManager = new CPlayerManager;

	m_pPlayerManager->Init();

	// シングルトンを生成
	m_pPauseManager = CPauseManager::CreateSingleton();

	CObject3D::Create(VEC3_NULL, VEC3_NULL,"data\\TEXTURE\\floor.jpg", D3DXVECTOR2(2000.0f, 1000.0f));
	CObject3D::Create(D3DXVECTOR3(0.0f,1000.0f,1000.0f), D3DXVECTOR3((-D3DX_PI * 0.5f),0.0f,0.0f),"data\\TEXTURE\\wall.jpg",D3DXVECTOR2(2000.0f,1000.0f));
	CObject3D::Create(D3DXVECTOR3(-2000.0f,1000.0f,0.0f), D3DXVECTOR3((-D3DX_PI * 0.5f), (-D3DX_PI * 0.5f), 0.0f),"data\\TEXTURE\\wall.jpg",D3DXVECTOR2(1000.0f, 1000.0f));
	CObject3D::Create(D3DXVECTOR3(2000.0f,1000.0f,0.0f), D3DXVECTOR3((-D3DX_PI * 0.5f), (D3DX_PI * 0.5f), 0.0f), "data\\TEXTURE\\wall.jpg", D3DXVECTOR2(1000.0f, 1000.0f));
#ifdef _DEBUG
#else
#endif // _DEBUG

	{
		m_GroundShape = make_unique<btBoxShape>(btVector3(btScalar(5000), btScalar(10), btScalar(5000)));

		btTransform groundTransform;
		groundTransform.setIdentity();
		groundTransform.setOrigin(btVector3(0, -10, 0));

		btScalar mass(0);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			m_GroundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* motionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, m_GroundShape.get(), localInertia);
		m_RigitBody = make_unique<btRigidBody>(rbInfo);

		//add the body to the dynamics world
		CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
	}

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
				//CFade::SetFade(new CResult);
			}
		}
	}
#endif // DEBUG
	if (m_pPauseManager != NULL)
	{
		m_pPauseManager->Update();
	}
}

//***************************************
// 終了処理
//***************************************
void CGame::Uninit(void)
{
	// プレイヤーマネージャーの破棄
	if (m_pPlayerManager != NULL)
	{
		delete m_pPlayerManager;
		m_pPlayerManager = NULL;
	}

	if (m_pPauseManager != NULL)
	{
		m_pPauseManager->Uninit();
		m_pPauseManager = NULL;
	}

	// 剛体の削除
	if (m_RigitBody)
	{
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());
		if (m_RigitBody->getMotionState())
		{
			delete m_RigitBody->getMotionState();
		}
		m_RigitBody.reset();
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