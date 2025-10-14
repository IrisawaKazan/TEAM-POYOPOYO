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
	CPlayerManager();
	~CPlayerManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	std::vector<CPlayer*> m_pPlayer;
};
#endif