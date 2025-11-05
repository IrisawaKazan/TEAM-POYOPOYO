//==============================================================
//
// [goal.h]
// Author: Irisawa Kazan
//
//==============================================================
#ifndef _GOAL_H_
#define _GOAL_H_

// インクルード


class CGoal : public CObject
{
public:
	CGoal();
	~CGoal();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CGoal* Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Size);

private:
	std::unique_ptr<btCollisionShape> m_CollisionShape;	// 当たり判定の形状
	std::unique_ptr<btGhostObject> m_GhostObject;		// ゴーストオブジェクト
	D3DXVECTOR3 m_pos;									// 位置
	D3DXVECTOR3 m_size;									// サイズ
};

#endif