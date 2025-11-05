//************************************************************
//
//　プレイヤーの処理[player.h]
//　Author:chikada shouya
//
//************************************************************
// 二重インクルード防止
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <unordered_map>
#include "character.h"

class CPlayer : public CModelCharacter
{
public:
	static constexpr float MOVE_SPEED = 10.0f;           // 移動スピード sato Add

	// 状態
	enum class STATE : unsigned char
	{
		NORMAL,
		TURN,
		MAX
	};

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	btRigidBody* GetRB(void) { return m_RigitBody.get(); }

	// セッター
	void SetPos(D3DXVECTOR3 Pos);

	// 静的メンバ関数
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	static constexpr float TURN_RADIUS = 5.0f; // ターンの中心軸からのずれの許容範囲

	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー

	std::unordered_map<size_t, unsigned short> m_naviObjectCountMap; // ナビゲーションオブジェクトの接触回数管理用マップ
	std::vector<size_t> m_naviObjectIdxListOld;                      // 前のフレームで触れていたナビゲーションオブジェクトのインデックスリスト

	STATE m_state;         // 状態 sato Add
	D3DXVECTOR3 m_turnPos; // 曲がる位置 sato Add
	float m_turnAngle;     // 曲がる方向 sato Add
};
#endif