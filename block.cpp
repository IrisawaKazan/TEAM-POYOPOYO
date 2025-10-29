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

	// サイズを代入する
	m_size.x = Max.x;			// sizeのx
	m_size.y = Max.y * 0.5f;	// sizeのy
	m_size.z = Max.z;			// sizeのz

	// リジットボディーのオフセットを設定
	m_RBOffset.y = m_size.y * GetScale().y;

	// アンロック
	pMesh->UnlockVertexBuffer();

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

	// リジットボディーの更新
	UpdateRB();

	// トランス*フォーム
	btTransform trans;

	// ゲット
	m_RigitBody->getMotionState()->getWorldTransform(trans);

	// 回転行列を取得オフセット分掛ける
	// 物理世界での位置からオフセット分ずらした現実世界での位置を計算する用の変数
	btVector3 worldoffet = trans.getBasis() * btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z);

	// 物理世界の位置から回転行列をかけ合わせたオフセットを引く
	btVector3 pos = trans.getOrigin() - worldoffet;

	// 位置を設定
	SetPosition(D3DXVECTOR3(pos.x(), pos.y(), pos.z()));

	// ナビにレイキャストオブジェクトを登録 sato 仮
	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(GetIndx());
	CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, GetWorldMtx());
}

//***************************************
// リジットボディーの更新処理
//***************************************
void CBlock::UpdateRB(void)
{
	// 剛体の削除
	if (m_RigitBody)
	{
		// 物理世界から削除
		CManager::GetDynamicsWorld()->removeRigidBody(m_RigitBody.get());

		// モーションステートを取得nullチェック
		if (m_RigitBody->getMotionState())
		{
			// モーションステートを削除
			delete m_RigitBody->getMotionState();
		}
		// リジットボディをクリア
		m_RigitBody.reset();
	}

	D3DXVECTOR3 Scale = GetScale();

	// 当たり判定を再生成
	m_CollisionShape.reset(new btBoxShape(btVector3(m_size.x * Scale.x, m_size.y * Scale.y, m_size.z * Scale.z)));
	m_RBOffset.y = m_size.y * Scale.y;

	// 質量を宣言
	btScalar Mass = 0;

	// 抗力を代入
	btVector3 Inertia = { 0.0f,0.0f,0.0f };

	// 抗力を設定
	m_CollisionShape->calculateLocalInertia(Mass, Inertia);

	// 物理世界の位置などを取得
	btTransform transform, origin, offset;

	// 初期化
	transform.setIdentity();
	origin.setIdentity();
	offset.setIdentity();

	// OBBの回転（例：Y軸まわりに45度回転）
	btQuaternion rotation;
	rotation = CMath::ConvertQuat(GetQuad());
	origin.setRotation(rotation);
	origin.setOrigin(btVector3(GetPosition().x, GetPosition().y, GetPosition().z));
	offset.setOrigin(btVector3(m_RBOffset.x, m_RBOffset.y, m_RBOffset.z));
	transform.mult(origin, offset);

	// インターフェイスを設定
	btDefaultMotionState* motionState = new btDefaultMotionState(transform);
	btRigidBody::btRigidBodyConstructionInfo info(Mass, motionState, m_CollisionShape.get());

	// リジットボディーを再生成
	m_RigitBody.reset(new btRigidBody(info));

	// 移動方向を制限
	m_RigitBody->setLinearFactor(btVector3(1, 1, 1));

	// ユーザーポインタを設定
	m_RigitBody->setUserPointer(this);

	// スリープ状態を設定
	m_RigitBody->setActivationState(DISABLE_DEACTIVATION);

	// 物理世界にリジットボディーを追加
	CManager::GetDynamicsWorld()->addRigidBody(m_RigitBody.get());
}

//***************************************
//　ブロック描画処理
//***************************************
void CBlock::Draw(void)
{
	// 描画
	CObjectX::Draw();
}