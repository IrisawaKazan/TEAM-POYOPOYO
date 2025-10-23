//************************************************************
//
//�@�v���C���[�̏���[playermanager.cpp]
//�@Author:chikada shouya
//
//************************************************************

// �C���N���[�h
#include "playermanager.h"
#include "manager.h"

// �ÓI�����o�ϐ��̒�`
const D3DXVECTOR3 CPlayerManager::SPAWN_POS = D3DXVECTOR3(-1900.0f, 0.0f, 0.0f);              // NPC�̏o���ʒu sato Add
const D3DXVECTOR3 CPlayerManager::SPAWN_ROT = D3DXVECTOR3(0.0f, D3DXToRadian(-90.0f), 0.0f); // NPC�̏o����] sato Add

//************************************************************
// �R���X�g���N�^
//************************************************************
CPlayerManager::CPlayerManager()
{

}

//************************************************************
// �f�X�g���N�^
//************************************************************
CPlayerManager::~CPlayerManager()
{

}

//************************************************************
// ������
//************************************************************
HRESULT CPlayerManager::Init(void)
{
	return S_OK;
}

//************************************************************
// �j��
//************************************************************
void CPlayerManager::Uninit(void)
{

}

//************************************************************
// �X�V
//************************************************************
void CPlayerManager::Update(void)
{
	bool bPause = CManager::isPause();

	// �|�[�Y����������
	if (bPause != true)
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
				Info = CPlayer::Create(SPAWN_POS, SPAWN_ROT);

				// ����O����ǉ����Ă���
				m_pPlayer.push_back(Info);
			}
		}
	}

	// �v���C���[�̔z��ɃA�N�Z�X
	for (auto Players = m_pPlayer.begin(); Players != m_pPlayer.end(); Players++)
	{
		// �����Ă�����
		if ((*Players)->GetPos().y < Fall)
		{
			// �j��
			(*Players)->Uninit();
			Players = m_pPlayer.erase(Players);
			if (Players == m_pPlayer.end()) break;
		}
	}
}

//************************************************************
// �`��
//************************************************************
void CPlayerManager::Draw(void)
{

}