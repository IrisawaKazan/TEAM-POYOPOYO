//************************************************************
//
//�@�v���C���[�̏���[playermanager.cpp]
//�@Author:chikada shouya
//
//************************************************************

// �C���N���[�h
#include "playermanager.h"

// �ÓI�����o�ϐ��̒�`
const D3DXVECTOR3 CPlayerManager::SPAWN_POS = D3DXVECTOR3(-100.0f, 0.0f, 0.0f); // NPC�̏o���ʒu sato Add

// �R���X�g���N�^
CPlayerManager::CPlayerManager()
{
	
}

// �f�X�g���N�^
CPlayerManager::~CPlayerManager()
{

}

// ������
HRESULT CPlayerManager::Init(void)
{
	return S_OK;
}

// �j��
void CPlayerManager::Uninit(void)
{

}

// �X�V
void CPlayerManager::Update(void)
{
	// �^�C�}�[��i�߂�
	m_Timer++;

	// �X�|�[�������鎞�ԂȂ�
	if (m_Timer > SPAWN_TIME)
	{
		// �^�C�}�[���Z�b�g
		m_Timer = 0;

		// �ő吔�ȉ��Ȃ�
		if (m_pPlayer.size() < CAPACITY)
		{
			// ���[�J���ϐ�
			CPlayer* Info = {};

			// ����
			Info = CPlayer::Create(SPAWN_POS, VEC3_NULL);

			// ����O����ǉ����Ă���
			m_pPlayer.push_back(Info);
		}
	}
}

// �`��
void CPlayerManager::Draw(void)
{

}