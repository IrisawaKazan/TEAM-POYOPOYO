//*************************************
//
//�@�L�����N�^�[����[character.h]
//�@Author:chikada shouya
//
//*************************************
// ��d�C���N���[�h�h�~
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "object.h"

class CCharacter : public CObject
{
public:
	CCharacter();
	~CCharacter();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);


private:
	D3DXVECTOR3 m_pos;						//�ʒu
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_move;						//�ړ�
	D3DXVECTOR3 m_rot;						//����
	D3DXVECTOR3 m_rotDest;					//�ړI�̌���

};

#endif