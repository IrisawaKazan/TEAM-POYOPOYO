//****************************************************************
//
// リフトの処理[uplift.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "manager.h"
#include "uplift.h"

//***************************************
// コンストラクタ
//***************************************
CUpLift::CUpLift()
{
}

//***************************************
// デストラクタ
//***************************************
CUpLift::~CUpLift()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CUpLift::Init(void)
{
	CObjectX::Init();

	// 当たり判定を生成
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(5.0f,5.0f,5.0f));

	// 質量を設定
	btScalar mass = 100.0f;

	// 移動方向を設定
	btVector3 inertia(0, 0, 0);

	// 計算
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	// トランスフォームを宣言、初期化
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(GetPosition().x, GetPosition().y + 20.0f, GetPosition().z));

	// モーションステートを宣言
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);

	// リジットボディーの情報を宣言
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	// リジットボディを生成
	m_RigitBody = std::make_unique<btRigidBody>(info);

	// 移動制限
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ユーザーポインタを設定
	m_RigitBody->setUserPointer(this);

	// 静的が動的か設定
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CUpLift::Uninit(void)
{
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

	// 衝突形状の削除
	m_CollisionShape.reset();

	CObjectX::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CUpLift::Update(void)
{
	// 制限距離
	D3DXVECTOR3 Dest = m_ApperPos + (m_Velocity * m_Limit);
	float LmitDis = D3DXVec3Length(&Dest);

	// 位置
	btVector3 Pos;
	btTransform trans;
	trans.setIdentity();
	m_RigitBody->getMotionState()->getWorldTransform(trans);
	Pos = trans.getOrigin();

	// 今の位置から初期位置のベクトルの長さを計算
	Dest = D3DXVECTOR3(Pos.x(), Pos.y(), Pos.z()) - m_ApperPos;
	float NowDis = D3DXVec3Length(&Dest);

	// スイッチが押されていなくて制限まで移動したら
	if (!m_Switch && NowDis > LmitDis)
	{
		// 移動量を反転
		m_Velocity = -m_Velocity;

		// フラグを有効化
		m_Switch = true;
	}
	// スイッチが押されていて初期位置に近かったら
	else if (m_Switch && NowDis < 10.0f)
	{
		// 移動量を反転
		m_Velocity = -m_Velocity;

		// フラグを無効か
		m_Switch = false;
	}

	// 移動量を設定
	m_RigitBody->setLinearVelocity(btVector3(m_Velocity.x, m_Velocity.y, m_Velocity.z));

	// 見た目の位置を設定
	SetPosition({ Pos.x() ,Pos.y() ,Pos.z() });

	// 更新
	CObjectX::Update();
}

//***************************************
// 描画
//***************************************
void CUpLift::Draw(void)
{
	CObjectX::Draw();
}

//***************************************
// 生成
//***************************************
CUpLift* CUpLift::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Velocity, float Limit)
{
	// インスタンスを宣言
	CUpLift* pInstance = nullptr;

	// メモリ確保
	pInstance = new CUpLift;

	// 確保できたら
	if (pInstance != nullptr)
	{
		// 情報を設定
		pInstance->SetPosition(Pos);
		pInstance->m_Velocity = Velocity;
		pInstance->m_Limit = Limit;
		pInstance->m_ApperPos = Pos;
		pInstance->m_Switch = false;
		pInstance->SetIdx("data\\MODEL\\test.x");

		// 初期化処理
		pInstance->Init();

		// インスタンスを返す
		return pInstance;
	}
	else
	{
		return nullptr;
	}
}