//=================================================
//
//	block.h
// 
// Author:chikada shouya
//
//=================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectx.h"

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
		TYPE_NONE = 0,	//
		TYPE_0,		//
		TYPE_1,		//
		TYPE_2,		//
		TYPE_3,		//
		TYPE_MAX,
	}TYPE;

	CBlock(int nPriority = 3);
	~CBlock();
	HRESULT Init(void);
	void InitRB(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	D3DXVECTOR3 GetClosestPointOnSurface(D3DXVECTOR3 worldPoint) const; // sato Add

	D3DXVECTOR3 GetSize(void) const { return m_size; }
	btRigidBody* GetRB(void) const { return m_RigitBody.get(); }
	std::string GetFilepath(void) const { return m_sNamePath; }

	static CBlock* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });

	// セッター
	void SetFilepath(std::string sName) { m_sNamePath = sName; };
	void SetQuat(btQuaternion Quad);
private:
	TYPE m_type;			// ブロックのタイプ
	D3DXVECTOR3 m_size;		// サイズ
	D3DXVECTOR3 m_RBOffset;	// オフセット
	std::string m_sNamePath;// ファイルパス
	std::unique_ptr<btCollisionShape> m_CollisionShape;	// 当たり判定の形状
	std::unique_ptr<btRigidBody> m_RigitBody;			// リジットボディー
};
#endif