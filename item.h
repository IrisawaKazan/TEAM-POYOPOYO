//****************************************************************
//
// アイテムの処理
// Author:Takahashi Misaki
//
//****************************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

// インクルード
#include "objectX.h"

// アイテムクラス
class CItem : public CObjectX
{
public:

	typedef enum
	{
		ITEM_NOEN = 0,	// なし
		ITEM_BACK,		// 奥に進む指示
		ITEM_RIGHT,		// 右に進む指示
		ITEM_FRONT,		// 手前に進む指示
		ITEM_LEFT,		// 左に進む指示
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

	// アイテムの当たり判定
	bool CollisionItem(const D3DXVECTOR3 pos, const float fWidth, const float fDepth);

	// 静的メンバ関数
	static CItem* Create(const ITEM type,	// 種類
		const D3DXVECTOR3 pos,				// 位置
		const D3DXVECTOR3 rot,				// 向き
		const float fWidth,					// 横幅
		const float fDepth,					// 奥行
		const D3DXVECTOR3 scale,			// 向き
		const std::string FileName);		// モデルのファイル名

private:

	// メンバ変数
	ITEM m_type;			// アイテムの種類
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	float m_fWidth;			// 横幅
	float m_fDepth;			// 奥行
	D3DXVECTOR3 m_scale;	// 拡大率
	int m_nModelIdx;		// インデックス
};

#endif