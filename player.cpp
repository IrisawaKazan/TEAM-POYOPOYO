//************************************************************
//
//�@�v���C���[�̏���[player.cpp]
//�@Author:chikada shouya
//
//************************************************************
#include "player.h"
#include "math_T.h"

// �ÓI�����o�ϐ��̒�`

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

	// �ړ����� (�v���g�p) sato Add
	CModelCharacter::SetRot(D3DXVECTOR3(0.0f, TEST_MOVE_ANGLE, 0.0f));
	CModelCharacter::SetRotDest(D3DXVECTOR3(0.0f, TEST_MOVE_ANGLE, 0.0f));
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
	TestMove(); // �v���g�p�ړ����� sato Add
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

//*******************************
// �v���g�p�ړ����� sato Add
//*******************************
void CPlayer::TestMove(void)
{
	// �ړ����� (�v���g�p) sato Add
	D3DXVECTOR3 rot = CModelCharacter::GetRot();
	D3DXVECTOR3 pos = CModelCharacter::GetPos();
	CModelCharacter::SetMove(D3DXVECTOR3(-sinf(rot.y) * TEST_MOVE_SPEED, 0.0f, -cosf(rot.y) * TEST_MOVE_SPEED + (0.0f - pos.z) * 0.01f));
	D3DXVECTOR3 posOld = CModelCharacter::GetPosOld();
	if (std::abs(pos.x) > TEST_MOVE_LIMIT && std::abs(posOld.x) <= TEST_MOVE_LIMIT)
	{// �ړ������𒴂�����
		rot.y += D3DX_PI; // �p�x�𔽓]
		Normalize(rot.y, &rot.y);
		CModelCharacter::SetRotDest(rot);
	}
}