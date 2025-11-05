//==============================================================
//
// [goal.cpp]
// Author: Irisawa Kazan
//
//==============================================================
// インクルード
#include "goal.h"

//----------------------------------------
// コンストラクタ
//----------------------------------------
CGoal::CGoal()
{

}

//----------------------------------------
// デストラクタ
//----------------------------------------
CGoal::~CGoal()
{

}

//----------------------------------------
// 初期化処理
//----------------------------------------
HRESULT CGoal::Init(void)
{
	// メモリ確保OBBの大きさを設定
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(m_size.x, m_size.y, m_size.z));
	
	// GhostObjectを作成
	m_GhostObject = std::make_unique<btGhostObject>();
	
	// 衝突形状を設定
	m_GhostObject->setCollisionShape(m_CollisionShape.get());
	
	// ワールド変換（位置と回転）を設定
	D3DXVECTOR3 Offset;
	Offset.y += m_size.y * 0.5f;
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(m_pos.x, m_pos.y + Offset.y, m_pos.z)); // 位置を設定
	m_GhostObject->setWorldTransform(transform);
	
	// 衝突グループとマスクを設定してワールドに追加
	CManager::GetDynamicsWorld()->addCollisionObject(
		m_GhostObject.get(),
		btBroadphaseProxy::SensorTrigger,
		btBroadphaseProxy::AllFilter
	);

	return S_OK;
}

//----------------------------------------
// 終了処理
//----------------------------------------
void CGoal::Uninit(void)
{
	// 剛体の削除
	if (m_GhostObject)
	{
		CManager::GetDynamicsWorld()->removeCollisionObject(m_GhostObject.get());
		
		if (m_GhostObject->getCollisionShape())
		{
			delete m_GhostObject->getCollisionShape();
		}
		m_GhostObject.reset();
	}
	
	// 衝突形状の削除
	m_CollisionShape.reset();
	
	// 破棄
	Release();
}

//----------------------------------------
// 更新処理
//----------------------------------------
void CGoal::Update(void)
{

}

//----------------------------------------
// 描画処理
//----------------------------------------
void CGoal::Draw(void)
{

}

//----------------------------------------
// 生成処理
//----------------------------------------
CGoal* CGoal::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Size)
{
	CGoal* pGoal = nullptr;

	pGoal = new CGoal;

	if (pGoal != nullptr)
	{
		pGoal->m_pos = Pos;
		pGoal->m_size = Size;

		// オブジェクト設定
		pGoal->Init();

		return pGoal;
	}
	else
	{
		return nullptr;
	}
}