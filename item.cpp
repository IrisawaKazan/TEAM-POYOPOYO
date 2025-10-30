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
	m_type = ITEM_NOEN;							// アイテムの種類
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_fWidth = 0.0f;							// 横幅
	m_fDepth = 0.0f;							// 奥行
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// 拡大率
	m_nModelIdx = 0;							// インデックス
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

	// モデルマネージャーのポインタ
	CModelManager* pModelManager;

	// インスタンス生成
	pModelManager = CModelManager::Instance();

	// モデルの詳細情報
	CModelManager::ModelInfo ModelInfo = pModelManager->GetAddress(m_nModelIdx);

	// メッシュ情報
	LPD3DXMESH pMesh = ModelInfo.pMesh;

	// ローカル変数
	int nNumVtx;				// 頂点の総数
	DWORD sizeFVF;				// 頂点のサイズ
	BYTE* pVtxBuff;				// 頂点のポインタ
	D3DXVECTOR3 max, min;		// 頂点の最大値と最小値
	D3DXVECTOR3 Vtx;			// 頂点座標

	// 最大値と最小値の初期化
	max = VEC3_NULL;
	min = VEC3_NULL;

	// 頂点の最大数を取得
	nNumVtx = pMesh->GetNumVertices();

	// 頂点のサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	// 頂点バッファをロック
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// 頂点の数だけ回す
	for (int nCount = 0; nCount < nNumVtx; nCount++)
	{
		// 頂点座標を代入する
		Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ***  頂点座標（X軸）の比較  ***

		if (Vtx.x > max.x)
		{// 最大値より大きい場合
			// 最大値に現在の値を代入する
			max.x = Vtx.x;
		}

		if (Vtx.x < min.x)
		{// 最大値より小さい場合
			// 最小値に現在の値を代入する
			min.x = Vtx.x;
		}


		// ***  頂点座標（Z軸）の比較  ***

		// 最大値より大きい場合
		if (Vtx.z > max.z)
		{
			// 最大値に現在の値を代入する
			max.z = Vtx.z;
		}

		if (Vtx.z < min.z)
		{// 最大値より小さい場合
			// 最小値に現在の値を代入する
			min.z = Vtx.z;
		}

		// 頂点のサイズ分進める
		pVtxBuff += sizeFVF;
	}

	// XとZの最大値を代入
	m_fWidth = max.x;	// 横幅に代入
	m_fDepth = max.z;	// 奥行に代入

	// 頂点バッファをアンロック
	pMesh->UnlockVertexBuffer();

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
	// 横に少しづつ回転
	m_rot.z += 0.05f;

	// 更新した向きを引数に代入
	SetRotasion(m_rot);

	switch (m_type)
	{
		// 左に進む指示の場合
	case ITEM_LEFT:


		break;

		// ジャンプする指示の場合
	case ITEM_JUMP:


		break;

		// 壁を登る指示の場合
	case ITEM_CLIMB:


		break;

	}


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

	// オブジェクトXのセッターに引数の値を代入
	pItem->SetPosition(pos);	// 位置
	pItem->SetRotasion(rot);	// 向き
	pItem->SetScale(scale);		// 拡大率
	pItem->SetIdx(FileName);	// モデルのファイル名

	// 引数の値をそれぞれのメンバ変数に代入 
	pItem->m_type = type;					// アイテムの種類
	pItem->m_pos = pos;						// 位置
	pItem->m_rot = rot;						// 向き
	pItem->m_scale = scale;					// 拡大率
	pItem->m_nModelIdx = pItem->GetIndx();	// インデックス

	// 初期化処理
	pItem->Init();

	// ポインタを返す
	return pItem;
}

//***************************************
// 当たり判定処理
//***************************************
bool CItem::CollisionItem(const D3DXVECTOR3 pos, const float fWidth, const float fDepth)
{
	// 対象の当たり判定
	float SubjectXL = pos.x - fWidth;	// X軸での左側の当たり判定
	float SubjectXR = pos.x + fWidth;	// X軸での右側の当たり判定
	float SubjectZF = pos.z - fDepth;	// Z軸での手前側の当たり判定
	float SubjectZB = pos.z + fDepth;	// Z軸での奥側の当たり判定

	// アイテムの当たり判定
	float ItemXL = m_pos.x - m_fWidth;	// X軸での左側の当たり判定
	float ItemXR = m_pos.x + m_fWidth;	// X軸での右側の当たり判定
	float ItemZF = m_pos.z - m_fDepth;	// Z軸での手前側の当たり判定
	float ItemZB = m_pos.z + m_fDepth;	// Z軸での奥側の当たり判定

	// X軸での当たり判定
	if (SubjectXR <= ItemXL || SubjectXL >= ItemXR)
	{// 当たってない場合
		return false;
	}

	// Z軸での当たり判定
	if (SubjectZB <= ItemZF || SubjectZF >= ItemZB)
	{// 当たってない場合
		return false;
	}

	// 当たっていると判定する
	return true;
}
