//************************************************************
//
//�@�v���C���[�̏���[player.cpp]
//�@Author:chikada shouya
//
//************************************************************
#include "player.h"

// �R���X�g���N�^
CPlayer::CPlayer()
{

}

// �f�X�g���N�^
CPlayer::~CPlayer()
{

}

// ������
HRESULT CPlayer::Init(void)
{
	CModelCharacter::Init("data\\Model\\MOTION\\player.txt",5.0f);
	return S_OK;
}

// �j��
void CPlayer::Uninit(void)
{
	CModelCharacter::Uninit();
}

// �X�V
void CPlayer::Update(void)
{
	CModelCharacter::Update();
}

// �`��
void CPlayer::Draw(void)
{
	CModelCharacter::Draw();
}

// ����
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->SetPos(pos);
		pPlayer->SetRot(rot);
		pPlayer->Init();
		return pPlayer;
	}
	else
	{
		return nullptr;
	}

}
