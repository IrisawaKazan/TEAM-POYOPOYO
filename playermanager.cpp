//************************************************************
//
//�@�v���C���[�̏���[playermanager.cpp]
//�@Author:chikada shouya
//
//************************************************************
#include "playermanager.h"

// �ÓI�����o�ϐ��̒�`
const D3DXVECTOR3 CPlayerManager::SPAWN_POS = D3DXVECTOR3(-1800.0f, 0.0f, 0.0f); // NPC�̏o���ʒu sato Add

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
	// ���[�J���ϐ�
	CPlayer* Info = {};

	for (int nCnt = 0; nCnt < NUM_PLAYER; nCnt++)
	{
		Info = CPlayer::Create(SPAWN_POS + D3DXVECTOR3(50.0f * nCnt, 0.0f, 0.0f), VEC3_NULL);

		// ����O����ǉ����Ă���
		m_pPlayer.push_back(Info);
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

}

// �`��
void CPlayerManager::Draw(void)
{

}