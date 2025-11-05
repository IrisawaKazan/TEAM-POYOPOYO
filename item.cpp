//****************************************************************
//
// アイテムの処理
// Author:Takahashi Misaki
//
//****************************************************************

// インクルード
#include "item.h"
#include "modelmanager.h"

//***************************************
// コンストラクタ
//***************************************
CItem::CItem()
{
	m_type = ITEM_NOEN;		// アイテムの種類
	m_size = VEC3_NULL;		// サイズ
	m_RBOffset = VEC3_NULL;	// リジットボディのオフセット
}

//***************************************
// デストラクタ
//***************************************
CItem::~CItem()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CItem::Init(void)
{
	// オブジェクトXの初期化処理
	CObjectX::Init();

	// メモリ確保OBBの大きさを設定
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(m_size.x * GetScale().x, m_size.y * GetScale().y, m_size.z * GetScale().z));

	// 質量
	btScalar mass = 0.0f;

	// 慣性モーメントの計算用変数
	btVector3 inertia(0, 0, 0);

	// 計算
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	// 最終的な計算結果、位置、位置からのオフセット
	btTransform transform, Origin, Offset;

	// 初期化
	transform.setIdentity();
	Origin.setIdentity();
	Offset.setIdentity();

	// 向き
	btQuaternion rotation;

	// 代入
	rotation = CMath::ConvertQuat(GetQuad());

	// 位置と向きを設定
	Origin.setRotation(rotation);
	Origin.setOrigin(btVector3(GetPosition().x, GetPosition().y, GetPosition().z));

	// オフセットを設定
	Offset.setOrigin(btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z));

	// 合成
	transform.mult(Origin, Offset);

	// 位置と向きを管理するモーションを生成
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, m_CollisionShape.get());

	// 生成
	m_RigitBody = std::make_unique<btRigidBody>(info);

	// 移動制限を設定
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// 物理ボディーに自分自身を紐付け
	m_RigitBody->setUserPointer(this);

	// 動的オブジェクトか静的オブジェクトか
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界に物理ボディーを追加
	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CItem::Uninit(void)
{
	// オブジェクトXの終了処理
	CObjectX::Uninit();

}

//***************************************
// 更新処理
//***************************************
void CItem::Update(void)
{
	// クオータニオン
	D3DXQUATERNION pQuat;
	// 回転させる軸
	D3DXVECTOR3 Axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 向き
	static float fAngle = 0.0f;

	// クオータニオンの取得
	pQuat = GetQuad();
	// 回転量を加算
	fAngle += 0.05f;

	// クオータニオンの計算
	D3DXQuaternionRotationAxis(&pQuat, &Axis, fAngle);

	// 更新したクオータニオンを引数に代入
	SetQuad(pQuat);


}

//***************************************
// 描画処理
//***************************************
void CItem::Draw(void)
{
	// オブジェクトXの描画処理
	CObjectX::Draw();

}

//***************************************
// 生成処理
//***************************************
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale,  const std::string FileName)
{
	// アイテムのポインタ
	CItem* pItem = nullptr;

	// 生成
	pItem = new CItem;

	if (pItem == nullptr)
	{// ポインタがnullptrだったら
		return nullptr;
	}

	// オブジェクトXのセッターに引数の値を入れる
	pItem->SetPosition(pos);	// 位置
	pItem->SetRotasion(rot);	// 向き
	pItem->SetScale(scale);		// 拡大率
	pItem->SetIdx(FileName);	// モデルのファイル名

	// メンバ変数に代入する
	pItem->m_type = type;

	// 初期化処理
	pItem->Init();

	// ポインタを返す
	return pItem;
}
