//************************************************************
//
//　プレイヤーの処理[playermanager.h]
//　Author:chikada shouya
//
//************************************************************
// 二重インクルード防止
#ifndef _PLAYERMANAGER_H_
#define _PLAYERMANAGER_H_

#include "main.h"
#include "player.h"

class CPlayerManager
{
public:
	static constexpr int NUM_PLAYER = 5;        // NPCの数       sato Add
	static const D3DXVECTOR3 SPAWN_POS;         // NPCの出現位置 sato Add
	static const D3DXVECTOR3 SPAWN_ROT;         // NPCの出現回転 sato Add
	static constexpr float SPAWN_RANGE = 50.0f; // NPCの出現間隔 sato Add
	static constexpr int SPAWN_TIME = 120;		// NPCの出現時間
	static constexpr int CAPACITY = 10;		// 最大数

	CPlayerManager();
	~CPlayerManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_Timer;						// タイマー
	std::vector<bool> m_bUses;			// プールの使用状態
	std::vector<CPlayer*> m_pPlayer;	// 可変長配列
};
#endif