//=================================================
//
//	block.cpp
// 
// Author:近田 尚也
//
//=================================================

//***************************************
// インクルードファイル宣言
//***************************************
#include "block.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"
#include "switch.h"

//***************************************
// コンストラクタ
//***************************************
CBlock::CBlock(int nPriority):CObjectX(nPriority)
{
	m_type = TYPE_NONE;
	m_size = VEC3_NULL;
	m_RBOffset = VEC3_NULL;
	m_sNamePath = {};
}

//***************************************
// デストラクタ
//***************************************
CBlock::~CBlock()
{

}

//***************************************
// ブロックの生成
//***************************************
CBlock* CBlock::Create(std::string sName,D3DXVECTOR3 pos, D3DXVECTOR3 rot,D3DXVECTOR3 Scale)
{
	CBlock* pBlock = nullptr;
	pBlock = new CBlock;

	if (pBlock != nullptr)
	{
		// オブジェクト設定
		pBlock->m_sNamePath = sName;
		pBlock->SetPosition(pos);
		pBlock->SetRotasion(rot);
		pBlock->SetScale(Scale);
		pBlock->Init();
		return pBlock;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
// クォータニオンを設定
//***************************************
void CBlock::SetQuat(btQuaternion Quad)
{
	if (m_RigitBody == nullptr) return;
	D3DXQUATERNION myQuat = CMath::ConvertQuat(Quad);
	SetQuad(myQuat);
}

//***************************************
//　ブロック初期化処理
//***************************************
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(m_sNamePath));

	LPD3DXMESH pMesh = modelinfo.pMesh;

	// ローカル変数宣言-----------------------------
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;		// 頂点バッファのポインタ
	//----------------------------------------------

	// 頂点数の取得
	nNumVtx = pMesh->GetNumVertices();

	// 頂点のサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	// 頂点バッファをロック
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	D3DXVECTOR3 Max, Min;
	Max = VEC3_NULL;
	Min = VEC3_NULL;

	// 頂点数分回す
	for (int nCntBlock = 0; nCntBlock < nNumVtx; nCntBlock++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点座標の比較
		if (Vtx.x > Max.x)
		{// xが最大値より大きかったら
			// 代入
			Max.x = Vtx.x;
		}
		if (Vtx.y > Max.y)
		{// yが最大値より大きかったら
			// 代入
			Max.y = Vtx.y;
		}
		if (Vtx.z > Max.z)
		{// zが最大値より大きかったら
			// 代入
			Max.z = Vtx.z;
		}

		if (Vtx.x < Min.x)
		{// xが最小値より小さかったら
			// 代入
			Min.x = Vtx.x;
		}
		if (Vtx.y < Min.y)
		{// yが最小値より小さかったら
			// 代入
			Min.y = Vtx.y;
		}
		if (Vtx.z < Min.z)
		{// zが最小値より小さかったら
			// 代入
			Min.z = Vtx.z;
		}

		// 頂点フォーマットのサイズ分進める
		pVtxBuff += sizeFVF;
	}

	// アンロック
	pMesh->UnlockVertexBuffer();

	// 1. モデルの実際の半分のサイズ sato 変更
	m_size.x = (Max.x - Min.x) * 0.5f;
	m_size.y = (Max.y - Min.y) * 0.5f;
	m_size.z = (Max.z - Min.z) * 0.5f;

	// 2. モデル原点(0,0,0)から実際の中心までのオフセット
	//    Y軸は、CBlockのGetPosition()が底面
	m_RBOffset.x = (Max.x + Min.x) * 0.5f;
	m_RBOffset.y = (Max.y + Min.y) * 0.5f - Min.y;
	m_RBOffset.z = (Max.z + Min.z) * 0.5f;

	InitRB();

	SetIdx(m_sNamePath);

	return S_OK;
}

//***************************************
//　リジットボディーの初期化
//***************************************
void CBlock::InitRB(void)
{
	// メモリ確保OBBの大きさを設定
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(m_size.x, m_size.y, m_size.z));

	// 質量
	btScalar mass = 0.0f;

	// 慣性モーメントの計算用変数
	btVector3 inertia(0, 0, 0);

	// 計算
	m_CollisionShape->calculateLocalInertia(mass, inertia);

	// 最終的な計算結果、位置、位置からのオフセット
	btTransform transform,Origin,Offset;

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

	// オフセットを設定 sato 変更
	D3DXVECTOR3 Scale = GetScale();
	Offset.setOrigin(btVector3(m_RBOffset.x * Scale.x, m_RBOffset.y * Scale.y, m_RBOffset.z * Scale.z));

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
}

//***************************************
//　ブロック終了処理
//***************************************
void CBlock::Uninit(void)
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

	// 破棄
	CObjectX::Uninit();
}

//***************************************
//　ブロック更新処理
//***************************************
void CBlock::Update(void)
{
	if (m_RigitBody == nullptr) return;

	// スケールの更新
	D3DXVECTOR3 Scale = GetScale();

	// CollisionShapeにスケールを適用
	m_RigitBody->getCollisionShape()->setLocalScaling(btVector3(Scale.x, Scale.y, Scale.z));

	// トランスフォームの更新
	btTransform transform, Origin, Offset;
	transform.setIdentity();
	Origin.setIdentity();
	Offset.setIdentity();

	// 向きと位置でOriginを設定
	btQuaternion rotation = CMath::ConvertQuat(GetQuad());
	Origin.setRotation(rotation);
	Origin.setOrigin(btVector3(GetPosition().x, GetPosition().y, GetPosition().z));

	// Offsetを設定
	btVector3 scaledOffset = btVector3(m_RBOffset.x * Scale.x, m_RBOffset.y * Scale.y, m_RBOffset.z * Scale.z);
	Offset.setOrigin(scaledOffset);

	// トランスフォームを合成
	transform.mult(Origin, Offset);

	// トランスフォームを更新
	m_RigitBody->setWorldTransform(transform);
	m_RigitBody->getMotionState()->setWorldTransform(transform);

	// 物理ワールドにAABB更新通知
	CManager::GetDynamicsWorld()->updateSingleAabb(m_RigitBody.get());
}

//***************************************
//　ブロック描画処理
//***************************************
void CBlock::Draw(void)
{
	// 描画
	CObjectX::Draw();
}

//***************************************
//　渡された座標から一番近い表面の位置を返す
//***************************************
D3DXVECTOR3 CBlock::GetClosestPointOnSurface(D3DXVECTOR3 worldPoint) const
{
	// 箱状の形状がない
	if (m_RigitBody == nullptr || m_CollisionShape->getShapeType() != BOX_SHAPE_PROXYTYPE)
	{
		// 中心座標
		return GetPosition();
	}

	// 半分のサイズを取得
	btBoxShape* boxShape = static_cast<btBoxShape*>(m_CollisionShape.get());
	btVector3 halfExtents = boxShape->getHalfExtentsWithMargin();

	// トランスフォームを取得
	btTransform blockTransform = m_RigitBody->getWorldTransform();

	// 渡された座標をbtVector3に変換
	btVector3 queryPoint(worldPoint.x, worldPoint.y, worldPoint.z);

	// ワールド座標をブロックのローカル座標に変換
	btVector3 localQueryPoint = blockTransform.inverse() * queryPoint;

	// ローカル座標で、位置を箱の範囲内にクランプする
	btVector3 localClosestPoint;
	localClosestPoint.setX(btClamped(localQueryPoint.x(), -halfExtents.x(), halfExtents.x()));
	localClosestPoint.setY(btClamped(localQueryPoint.y(), -halfExtents.y(), halfExtents.y()));
	localClosestPoint.setZ(btClamped(localQueryPoint.z(), -halfExtents.z(), halfExtents.z()));

	// ワールド座標に直す
	btVector3 worldClosestPoint = blockTransform * localClosestPoint;

	// D3DXVECTOR3変換して返す
	return D3DXVECTOR3(worldClosestPoint.x(), worldClosestPoint.y(), worldClosestPoint.z());
}