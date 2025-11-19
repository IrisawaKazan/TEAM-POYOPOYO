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
	static constexpr float Fall = -100.0f;		// 落下として扱う高さ
	static constexpr int SPAWN_TIME = 30;		// NPCの出現時間
#ifdef _DEBUG
	static constexpr int CAPACITY = 1;		    // 最大数
#else
	static constexpr int CAPACITY = 500;		// 最大数
#endif // _DEBUG

	CPlayerManager();
	~CPlayerManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	std::vector<CPlayer*>& GetVPlayer(void) { return m_pPlayer; }
	void DethMessage(CPlayer* pPlayer);

private:
	int m_Timer;						// タイマー
	CPlayer* m_pOrigin;					// オリジナル
	std::vector<bool> m_bUses;			// プールの使用状態
	std::vector<CPlayer*> m_pPlayer;	// 可変長配列
};
#endif