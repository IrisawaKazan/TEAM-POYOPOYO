//****************************************************************
//
// �`���[�g���A���̏���
// Author:Takahashi Misaki
//
//****************************************************************

// �C���N���[�h
#include "tutorial.h"

//***************************************
// �R���X�g���N�^
//***************************************
CTutorial::CTutorial()
{
	m_type = TUTORIAL_NOEN;	// �`���[�g���A���̎��
	m_fWidth = 0.0f;		// ����
	m_fHeigth = 0.0f;		// �c��
}

//***************************************
// �f�X�g���N�^
//***************************************
CTutorial::~CTutorial()
{
}

//***************************************
// ����������
//***************************************
HRESULT CTutorial::Init(void)
{
	m_type = TUTORIAL_NOEN;	// �^�C�v���Ȃ��ɐݒ�
	m_fWidth = 0.0f;		// ����
	m_fHeigth = 0.0f;		// �c��

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CTutorial::Uninit(void)
{
}

//***************************************
// �X�V����
//***************************************
void CTutorial::Update(void)
{
	// �`���[�g���A�����Ȃ�����Ȃ�������
	if (m_type != TUTORIAL_NOEN)
	{
		// �ݒ肵���`���[�g���A���̍X�V������
		switch (m_type)
		{
			// ���ɐi�ގw���̏ꍇ
		case TUTORIAL_BACK:


			break;

			// �E�ɐi�ގw���̏ꍇ
		case TUTORIAL_RIGHT:


			break;

			// ��O�ɐi�ގw���̏ꍇ
		case TUTORIAL_FRONT:


			break;

			// ���ɐi�ގw���̏ꍇ
		case TUTORIAL_LEFT:


			break;

			// �W�����v����w���̏ꍇ
		case TUTORIAL_JUMP:


			break;

			// �ǂ��悶�o��w���̏ꍇ
		case TUTORIAL_CLIMB:


			break;

		}
	}

}

//***************************************
// �`�揈��
//***************************************
void CTutorial::Draw(void)
{


}

//***************************************
// �`���[�g���A���̐ݒ菈��
//***************************************
void CTutorial::SetTutorial(const TUTORIAL type)
{
	// �ݒ肵���`���[�g���A���ɕύX
	m_type = type;
}
