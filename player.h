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
	static constexpr float MOVE_SPEED = 10.0f;           // �ړ��X�s�[�h sato Add

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �Z�b�^�[
	void SetPos(D3DXVECTOR3 Pos);

	// �ÓI�����o�֐�
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

private:
	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// ���W�b�g�{�f�B�[
};
#endif