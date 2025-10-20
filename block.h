//=================================================
//
//	block.h
// 
// Author:近田 尚也
//
//=================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectx.h"
#include "object.h"

//マクロ
#define HALF (0.5f)					// 半分の値

class CBlock :public CObjectX
{
public:
	//***************************
	// ブロックの種類の列挙型宣言
	//***************************
	typedef enum
	{
		TYPE_0 = 0,	//
		TYPE_1,		//
		TYPE_2,		//
		TYPE_3,		//
		TYPE_4,		//
		TYPE_MAX,
	}TYPE;

	CBlock(int nPriority = 7);
	~CBlock();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBlock* Create(std::string sName,D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	bool Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, D3DXVECTOR3* move, D3DXVECTOR3* size);
	//bool Collision(void);

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き
	TYPE m_type;			// ブロックのタイプ
	D3DXVECTOR3 m_size;		// サイズ
	D3DXVECTOR3 m_VtxMax;	// 頂点最大値
	D3DXVECTOR3 m_VtxMin;	// 頂点最小値
	std::string sNamePath;	// ファイルパス
	int m_nIdx;
};

#endif