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

//***************************************
// コンストラクタ
//***************************************
CBlock::CBlock(int nPriority):CObjectX(nPriority)
{
	m_type = TYPE_NONE;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_VtxMax = { -10000.0f,-10000.0f,-10000.0f };
	m_VtxMin = { 10000.0f,10000.0f,10000.0f };
	m_size = VEC3_NULL;
	m_RBOffset = VEC3_NULL;
	sNamePath = {};
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
		pBlock->SetFilePath(sName);
		pBlock->sNamePath = sName;
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
//　ブロック初期化処理
//***************************************
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));

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

	// 頂点数分回す
	for (int nCntBlock = 0; nCntBlock < nNumVtx; nCntBlock++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 頂点座標の比較
		if (Vtx.x > m_VtxMax.x)
		{// xが最大値より大きかったら
			// 代入
			m_VtxMax.x = Vtx.x;
		}
		if (Vtx.y > m_VtxMax.y)
		{// yが最大値より大きかったら
			// 代入
			m_VtxMax.y = Vtx.y;
		}
		if (Vtx.z > m_VtxMax.z)
		{// zが最大値より大きかったら
			// 代入
			m_VtxMax.z = Vtx.z;
		}

		if (Vtx.x < m_VtxMin.x)
		{// xが最小値より小さかったら
			// 代入
			m_VtxMin.x = Vtx.x;
		}
		if (Vtx.y < m_VtxMin.y)
		{// yが最小値より小さかったら
			// 代入
			m_VtxMin.y = Vtx.y;
		}
		if (Vtx.z < m_VtxMin.z)
		{// zが最小値より小さかったら
			// 代入
			m_VtxMin.z = Vtx.z;
		}

		// 頂点フォーマットのサイズ分進める
		pVtxBuff += sizeFVF;
	}

	// サイズを代入する
	m_size.x = m_VtxMax.x;			// sizeのx
	m_size.y = m_VtxMax.y * 0.5f;	// sizeのy
	m_size.z = m_VtxMax.z;			// sizeのz

	// リジットボディーのオフセットを設定
	m_RBOffset.y = m_size.y * GetScale().y;

	// アンロック
	pMesh->UnlockVertexBuffer();

	InitRB();

	return S_OK;
}

//***************************************
//　リジットボディーの初期化
//***************************************
void CBlock::InitRB(void)
{
	// メモリ確保OBBの大きさを設定
	m_CollisionShape = std::make_unique<btBoxShape>(btVector3(m_size.x * GetScale().x, m_size.y * GetScale().y, m_size.z * GetScale().z));

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
	rotation = CObjectX::ConvertQuad(GetQuad());

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
	// 更新W
	CObjectX::Update();

	// ナビにレイキャストオブジェクトを登録 sato 仮
	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));
	CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, GetWorldMtx());
}

//***************************************
//　ブロック描画処理
//***************************************
void CBlock::Draw(void)
{
	// 描画
	CObjectX::Draw();
}