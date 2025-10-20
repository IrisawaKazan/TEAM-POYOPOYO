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
	m_pPlayer.resize(CAPACITY);
	m_bUses.resize(CAPACITY);

	for (int nCnt = 0; nCnt < CAPACITY; nCnt++)
	{
		// ����O����ǉ����Ă���
		m_pPlayer[nCnt] = CPlayer::Create({10000.0f,0.0f,0.0f}, VEC3_NULL);
		m_bUses[nCnt] = false;
	}

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

		bool IsCapacity = false;

		for (int nCnt = 0; nCnt < m_pPlayer.size(); nCnt++)
		{
			if (m_bUses[nCnt] == false)
			{
				IsCapacity = true;
				m_bUses[nCnt] = true;
				m_pPlayer[nCnt]->SetPos(SPAWN_POS);
				break;
			}
		}
		if (IsCapacity == false)
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