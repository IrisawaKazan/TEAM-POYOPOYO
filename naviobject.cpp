//--------------------------------
//
// ナビゲーションオブジェクト [naviobject.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviobject.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

size_t CNaviObject::m_nextIdx = 0; // 次のインデックス (Trigger渡し用)

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviObject::Init(void)
{
	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// 頂点バッファをロック
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.y);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.y);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.y);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.y);

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVertex->Unlock();

	// 当たり判定
	SetTriggerObject();
	SetReleaseObject();

	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviObject::Uninit(void)
{
	// ワールドへのポインタを取得
	btDynamicsWorld* world = CManager::GetDynamicsWorld();

	// オブジェクトがワールドに登録されている場合、削除
	if (m_triggerObject != nullptr)
	{
		world->removeCollisionObject(m_triggerObject.get());
		m_triggerObject.reset();
	}
	if (m_releaseObject != nullptr)
	{
		world->removeCollisionObject(m_releaseObject.get());
		m_releaseObject.reset();
	}

	if (m_pVertex != nullptr)
	{
		m_pVertex->Release();
		m_pVertex = nullptr;
	}
	// メモリの開放
	Release();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNaviObject::Update(void)
{
	// 座標と回転
	btVector3 bulletPos(m_pos.x, m_pos.y, m_pos.z);

	// Bullet用の3x3行列（Basis）を作成
	btMatrix3x3 bulletBasis;
	bulletBasis.setValue(
		m_mtxRot._11, m_mtxRot._12, m_mtxRot._13,
		m_mtxRot._21, m_mtxRot._22, m_mtxRot._23,
		m_mtxRot._31, m_mtxRot._32, m_mtxRot._33
	);

	// トランスフォームを作成
	btTransform triggerTransform;
	triggerTransform.setIdentity();
	triggerTransform.setOrigin(bulletPos);
	triggerTransform.setBasis(bulletBasis);

	// 円柱にトランスフォームを設定
	if (m_triggerObject != nullptr)
	{
		m_triggerObject->setWorldTransform(triggerTransform);
	}
	if (m_releaseObject != nullptr)
	{
		m_releaseObject->setWorldTransform(triggerTransform);
	}
}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 計算用マトリックス
	D3DXMATRIX mtxWorld, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// アングルの適応
	const D3DXVECTOR3 up = VEC_UP;
	D3DXMatrixRotationAxis(&mtxWorld, &up, m_angle);

	// 向きを反転
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデバイスからデータストリームに設定
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));

	// テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::Instance()->GetAddress(m_texIdx));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// 触れたら起動する
//--------------------------------
CNavi::TYPE CNaviObject::ActivateTrigger(const btCollisionObject* const& collisionObject, D3DXVECTOR3* outPos, float* outAngle, size_t* outIdx) const
{
	// ゴーストオブジェクトと重なっているオブジェクトの数を取得
	int numOverlapping = m_triggerObject->getNumOverlappingObjects();

	// オブジェクトをループ処理
	for (int cntObject = 0; cntObject < numOverlapping; cntObject++)
	{
		// 重なっている相手のオブジェクトを取得
		btCollisionObject* otherObject = m_triggerObject->getOverlappingObject(cntObject);

		if (collisionObject == otherObject)
		{// 呼び出し元の特定
			Activate(outPos, outAngle); // 固有処理
			if (outIdx != nullptr)
			{
				*outIdx = m_idx; //自身の番号を渡す
			}
			return m_type;
		}
	}

	return CNavi::TYPE::None;	// 衝突していいない
}

//--------------------------------
// 触れたら消す
//--------------------------------
bool CNaviObject::ReleaseTrigger(const btCollisionObject* const& collisionObject) const
{
	// ゴーストオブジェクトと重なっているオブジェクトの数を取得
	int numOverlapping = m_releaseObject->getNumOverlappingObjects();

	// 重なっているオブジェクトをループ処理
	for (int i = 0; i < numOverlapping; i++)
	{
		// 重なっている相手のオブジェクトを取得
		btCollisionObject* otherObject = m_releaseObject->getOverlappingObject(i);
		
		if (collisionObject == otherObject)
		{
			return true;
		}
	}

	return false;
}

//--------------------------------
// ActivateTrigger用の当たり判定
//--------------------------------
void CNaviObject::SetTriggerObject()
{
	// 円柱の設定
	float radius = m_chengeLength;
	float halfHeight = CHENGE_HEIGHT;
	btVector3 cylinderHalfExtents(radius, halfHeight, radius);
	m_triggerShape = std::make_unique<btCylinderShape>(cylinderHalfExtents);

	m_triggerObject = std::make_unique<btGhostObject>();
	m_triggerObject->setCollisionShape(m_triggerShape.get());

	// 座標と回転
	btVector3 bulletPos(m_pos.x, m_pos.y, m_pos.z);

	// Bullet用の3x3行列（Basis）を作成
	btMatrix3x3 bulletBasis;
	bulletBasis.setValue(
		m_mtxRot._11, m_mtxRot._12, m_mtxRot._13,
		m_mtxRot._21, m_mtxRot._22, m_mtxRot._23,
		m_mtxRot._31, m_mtxRot._32, m_mtxRot._33
	);

	// トランスフォームを作成
	btTransform triggerTransform;
	triggerTransform.setIdentity();
	triggerTransform.setOrigin(bulletPos);
	triggerTransform.setBasis(bulletBasis);

	// 円柱にトランスフォームを設定
	m_triggerObject->setWorldTransform(triggerTransform);

	// isTrigger(押し返し無し)
	m_triggerObject->setCollisionFlags(m_triggerObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// 追加
	CManager::GetDynamicsWorld()->addCollisionObject(m_triggerObject.get(), short(CollisionFilterGroups::COL_TRIGGER), btBroadphaseProxy::DefaultFilter);
}

//--------------------------------
// ReleaseTrigger用の当たり判定
//--------------------------------
void CNaviObject::SetReleaseObject()
{
	// 円柱の設定
	float radius = m_length;
	float halfHeight = CHENGE_HEIGHT;
	btVector3 cylinderHalfExtents(radius, halfHeight, radius);
	m_releaseShape = std::make_unique<btCylinderShape>(cylinderHalfExtents);

	m_releaseObject = std::make_unique<btGhostObject>();
	m_releaseObject->setCollisionShape(m_releaseShape.get());

	// 座標と回転
	btVector3 bulletPos(m_pos.x, m_pos.y, m_pos.z);

	// Bullet用の3x3行列（Basis）を作成
	btMatrix3x3 bulletBasis;
	bulletBasis.setValue(
		m_mtxRot._11, m_mtxRot._12, m_mtxRot._13,
		m_mtxRot._21, m_mtxRot._22, m_mtxRot._23,
		m_mtxRot._31, m_mtxRot._32, m_mtxRot._33
	);

	// トランスフォームを作成
	btTransform triggerTransform;
	triggerTransform.setIdentity();
	triggerTransform.setOrigin(bulletPos);
	triggerTransform.setBasis(bulletBasis);

	// 円柱にトランスフォームを設定
	m_releaseObject->setWorldTransform(triggerTransform);

	// isTrigger(押し返し無し)
	m_releaseObject->setCollisionFlags(m_releaseObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	// 追加
	CManager::GetDynamicsWorld()->addCollisionObject(m_releaseObject.get(), short(CollisionFilterGroups::COL_TRIGGER), short(CollisionFilterGroups::COL_TRIGGER));
}