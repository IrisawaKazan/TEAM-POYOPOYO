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
	static constexpr int NUM_PLAYER = 5;        // NPC�̐�       sato Add
	static const D3DXVECTOR3 SPAWN_POS;         // NPC�̏o���ʒu sato Add
	static const D3DXVECTOR3 SPAWN_ROT;         // NPC�̏o����] sato Add
	static constexpr float SPAWN_RANGE = 50.0f; // NPC�̏o���Ԋu sato Add
	static constexpr int SPAWN_TIME = 120;		// NPC�̏o������
	static constexpr int CAPACITY = 10;		// �ő吔

	CPlayerManager();
	~CPlayerManager();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_Timer;						// �^�C�}�[
	std::vector<bool> m_bUses;			// �v�[���̎g�p���
	std::vector<CPlayer*> m_pPlayer;	// �ϒ��z��
};
#endif