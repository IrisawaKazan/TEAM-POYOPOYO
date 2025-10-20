//************************************************************
//
//　プレイヤーの処理[player.h]
//　Author:chikada shouya
//
//************************************************************
// 二重インクルード防止
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "character.h"

class CPlayer : public CModelCharacter
{
public:
	static constexpr float MOVE_SPEED = 10.0f;           // 移動スピード sato Add

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void SetPos(D3DXVECTOR3 Pos);

	// 静的メンバ関数
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー
};
#endif