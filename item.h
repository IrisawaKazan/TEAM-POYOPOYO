//****************************************************************
//
// アイテムの処理
// Author:Takahashi Misaki
//
//****************************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

// インクルード
#include "object3D.h"

// アイテムクラス
class CItem : public CObject3D
{
public:

	typedef enum
	{
		ITEM_NOEN = 0,	// なし
		ITEM_BACK,		// 奥に進む指示
		ITEM_FORWARD,	// 前進する指示
		ITEM_FRONT,		// 手前に進む指示
		ITEM_MAX,		// 上限
	}ITEM;

	// コンストラクタ・デストラクタ
	CItem();
	~CItem();

	// メンバ関数
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);
	bool CollisionItem(const D3DXVECTOR3 pos, const float fWidth, const float fDepth);		// アイテムの当たり判定

	// 静的メンバ関数
	static CItem* Create(const ITEM type, const D3DXVECTOR3 pos, const float fWidth, const float fDepth);

private:

	// メンバ変数
	ITEM m_type;		// アイテムの種類
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
	float m_fWidth;		// 横幅
	float m_fDepth;		// 奥行
};

#endif