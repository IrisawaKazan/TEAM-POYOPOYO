//************************************************************
//
//�@�v���C���[�̏���[player.h]
//�@Author:chikada shouya
//
//************************************************************
// ��d�C���N���[�h�h�~
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "character.h"

class CPlayer : public CModelCharacter
{
public:
	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
};
#endif