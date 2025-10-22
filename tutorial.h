//****************************************************************
//
// �`���[�g���A���̏���
// Author:Takahashi Misaki
//
//****************************************************************

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// �C���N���[�h
#include "main.h"

// �`���[�g���A���N���X
class CTutorial
{
public:

	typedef enum
	{
		TUTORIAL_NOEN = 0,	// �Ȃ�
		TUTORIAL_BACK,		// ���ɐi�ގw��
		TUTORIAL_FORWARD,	// �O�i����w��
		TUTORIAL_FRONT,		// ��O�ɐi�ގw��
		TUTORIAL_JUMP,		// �W�����v����w��
		TUTORIAL_BREAK,		// �󂷎w��
		TUTORIAL_STOP,		// �~�܂�w��
		TUTORIAL_MAX,		// ���
	}TUTORIAL;

	// �R���X�g���N�^�E�f�X�g���N�^
	CTutorial();
	~CTutorial();

	// �����o�֐�
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);
	void SetTutorial(const TUTORIAL type);		// �`���[�g���A���̐ݒ菈��

	// �ÓI�����o�֐�

private:

	// �����o�ϐ�
	TUTORIAL m_type;	// �`���[�g���A���̎��
	float m_fWidth;		// ����
	float m_fHeigth;	// �c��
};

#endif