//****************************************************************
//
// アイテムの処理
// Author:Takahashi Misaki
//
//****************************************************************

// インクルード
#include "item.h"

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
	switch (m_type)
	{
		// 奥に進む指示の場合
	case ITEM_BACK:


		break;

		// 右に進む指示の場合
	case ITEM_RIGHT:


		break;

		// 手前に進む指示の場合
	case ITEM_FRONT:


		break;

		// 左に進む指示の場合
	case ITEM_LEFT:


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
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, const D3DXVECTOR3 scale,  const std::string FileName)
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
	pItem->m_fWidth = fWidth;				// 横幅
	pItem->m_fDepth = fDepth;				// 奥行
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
