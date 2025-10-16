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
	static constexpr float TEST_MOVE_SPEED = 10.0f;           // プロト用移動スピード sato Add
	static constexpr float TEST_MOVE_STOP = 10.0f;            // プロト用移動停止距離 sato Add
	static constexpr float TEST_MOVE_ANGLE = D3DX_PI * -0.5f; // プロト用移動角度 sato Add
	static constexpr float TEST_MOVE_LIMIT = 1900.0f;         // プロト用移動制限 sato Add

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
	void TestMove(void); // プロト用移動処理 sato Add
	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー
};
#endif