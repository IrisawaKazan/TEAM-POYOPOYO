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
	static constexpr float CAPSULE_RADIUS = 7.0f;  // カプセルの半径 sato Add
	static constexpr float CAPSULE_HEIGHT = 20.0f; // カプセルの高さ sato Add
	static constexpr float MOVE_SPEED = 10.0f;     // 移動スピード sato Add
	static constexpr float JUMP_POWER = 30.0f;     // ジャンプ力 sato Add
	static constexpr float JUMP_SPEED_INA = 5.0f;  // ジャンプ中の移動係数 sato Add
	static constexpr float GROUND_SPACE = 1.0f;    // 着地時の判定値 sato Add

	// 状態
	enum class STATE : unsigned char
	{
		Normal,
		Turn,
		Climb,
		Jump,
		Jumping,
		Max
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
	void UpdateGroundedState();
	void Turn();
	void Climb();
	void Jump(btVector3& moveDir);

	static constexpr float TURN_RADIUS = 8.0f; // ターンの中心軸からのずれの許容範囲

	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// リジットボディー

	std::unordered_map<size_t, unsigned short> m_naviObjectCountMap; // ナビゲーションオブジェクトの接触回数管理用マップ
	std::vector<size_t> m_naviObjectIdxListOld;                      // 前のフレームで触れていたナビゲーションオブジェクトのインデックスリスト

	STATE m_state;            // 状態 sato Add
	D3DXVECTOR3 m_activePos;  // 行動位置 sato Add
	float m_turnAngle;        // 曲がる方向 sato Add

	bool m_isGrounded;		// 着地しているか?
	bool m_IsSlopeTrigger;	// スロープに乗った瞬間のフラグ
};
#endif