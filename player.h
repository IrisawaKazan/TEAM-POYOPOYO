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
	static constexpr float TEST_MOVE_SPEED = 10.0f;           // �v���g�p�ړ��X�s�[�h sato Add
	static constexpr float TEST_MOVE_STOP = 10.0f;            // �v���g�p�ړ���~���� sato Add
	static constexpr float TEST_MOVE_ANGLE = D3DX_PI * -0.5f; // �v���g�p�ړ��p�x sato Add
	static constexpr float TEST_MOVE_LIMIT = 1900.0f;         // �v���g�p�ړ����� sato Add

	CPlayer();
	~CPlayer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
private:
	void TestMove(void); // �v���g�p�ړ����� sato Add
	std::unique_ptr<btCollisionShape> m_CollisionShape;
	std::unique_ptr<btRigidBody> m_RigitBody;		// ���W�b�g�{�f�B�[
};
#endif