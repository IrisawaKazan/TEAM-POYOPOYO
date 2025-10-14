//************************************************************
//
//�@�v���C���[�̏���[playermanager.h]
//�@Author:chikada shouya
//
//************************************************************
// ��d�C���N���[�h�h�~
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