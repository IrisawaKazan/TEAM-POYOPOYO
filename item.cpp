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
	m_type = ITEM_NOEN;						// アイテムの種類
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 向き
	m_fWidth = 0.0f;						// 横幅
	m_fDepth = 0.0f;						// 奥行
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
	// オブジェクト3Dの初期化処理
	CObject3D::Init();

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CItem::Uninit(void)
{
	// オブジェクト3Dの終了処理
	CObject3D::Uninit();

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

		// 前進する指示の場合
	case ITEM_FORWARD:


		break;

		// 手前に進む指示の場合
	case ITEM_FRONT:


		break;
	}

	// オブジェクト3Dの更新処理
	CObject3D::Update();

}

//***************************************
// 描画処理
//***************************************
void CItem::Draw(void)
{
	// オブジェクト3Dの描画処理
	CObject3D::Draw();

}

//***************************************
// 当たり判定処理
//***************************************
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const float fWidth, const float fDepth)
{
	// アイテムのポインタ
	CItem* pItem;

	// 引数の値をそれぞれの変数に代入 
	pItem->m_type = type;		// アイテムの種類
	pItem->m_pos = pos;			// 位置
	pItem->m_fWidth = fWidth;	// 横幅
	pItem->m_fDepth = fDepth;	// 奥行

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

	// 当たったかどうかの判定
	bool bCollX = false;	// X軸での判定
	bool bCollZ = false;	// Z軸での判定

	// X軸での当たり判定
	if (SubjectXR >= ItemXL || SubjectXL <= ItemXR)
	{// 当たった場合
		// X軸で当たっているとする
		bCollX = true;
	}
	else
	{
		return false;
	}

	// Z軸での当たり判定
	if (SubjectZB >= ItemZF || SubjectZF <= ItemZB)
	{// 当たった場合
		// Z軸で当たっているとする
		bCollZ = true;
	}
	else
	{
		return false;
	}

	//  X軸とZ軸で当たっている場合
	if (bCollX == true && bCollZ == true)
	{
		// 当たっていると判定する
		return true;
	}
}
