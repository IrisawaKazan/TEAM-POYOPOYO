//=================================================
//
//	block.h
// 
// Author:chikada shouya
//
//=================================================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_

#include "main.h"
#include "block.h"

//マクロ
#define HALF (0.5f)					// 半分の値

class CGimmick :public CBlock
{
public:
	//***************************
	// ブロックの種類の列挙型宣言
	//***************************
	typedef enum
	{
		TYPE_NONE = 0,	//
		TYPE_SWITCH,	//
		TYPE_2,			//
		TYPE_3,			//
		TYPE_4,			//
		TYPE_MAX,
	}TYPE;

	CGimmick(int nPriority = 7);
	~CGimmick();
	HRESULT Init(void);
	void InitRB(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGimmick* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	TYPE m_type;			// ブロックのタイプ
	D3DXVECTOR3 m_size;		// サイズ
	D3DXVECTOR3 m_VtxMax;	// 頂点最大値
	D3DXVECTOR3 m_VtxMin;	// 頂点最小値
	D3DXVECTOR3 m_RBOffset;	// オフセット
	std::string sNamePath;	// ファイルパス
	int m_nIdx;				// インデックス
	std::unique_ptr<btCollisionShape> m_CollisionShape;	// 当たり判定の形状
	std::unique_ptr<btRigidBody> m_RigitBody;			// リジットボディー
};
#endif