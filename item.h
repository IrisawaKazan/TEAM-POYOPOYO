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
		ITEM_LEFT,		// 左に進む指示
		ITEM_JUMP,		// ジャンプする指示
		ITEM_CLIMB,		// 壁を登る指示
		ITEM_MAX,		// 上限
	}ITEM;

	// コンストラクタ・デストラクタ
	CItem(int nPriority = 3);
	~CItem();

	// メンバ関数
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);

	// アイテムを入手状態の設定処理
	void SetItemTake(const bool bTake);

	// リジットボディ
	void UpdateRB(void);
	btRigidBody* GetRB(void) { return m_RigitBody.get(); }

	// 静的メンバ関数
	static CItem* Create(const ITEM type,	// 種類
		const D3DXVECTOR3 pos,				// 位置
		const D3DXVECTOR3 rot,				// 向き
		const D3DXVECTOR3 scale,			// 向き
		const std::string FileName);		// モデルのファイル名

private:

	// メンバ変数
	ITEM m_type;											// アイテムの種類
	D3DXVECTOR3 m_size;										// サイズ
	bool m_bTake;											// 入手したかどうか
	D3DXVECTOR3 m_RBOffset;									// オフセット
	std::unique_ptr<btCollisionShape> m_CollisionShape;		// 当たり判定の形状
	std::unique_ptr<btRigidBody> m_RigitBody;				// リジットボディー

};

#endif