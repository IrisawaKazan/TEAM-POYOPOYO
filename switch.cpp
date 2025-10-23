//=================================================
//
//	switch.cpp
//
// Author:chikada shouya
//
//=================================================
// 
//***************************************
// �C���N���[�h�t�@�C���錾
//***************************************
#include "switch.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"

//***************************************
// �R���X�g���N�^
//***************************************
CSwitch::CSwitch(int nPriority) :CBlock(nPriority)
{
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	sNamePath = {};
}

//***************************************
// �f�X�g���N�^
//***************************************
CSwitch::~CSwitch()
{
}

//***************************************
// �X�C�b�`����
//***************************************
CSwitch* CSwitch::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CSwitch* pSwitch = nullptr;
	pSwitch = new CSwitch;
	if (pSwitch != nullptr)
	{
		// �I�u�W�F�N�g�ݒ�
		pSwitch->SetFilepath(sName);
		pSwitch->SetPosition(pos);
		pSwitch->SetRotasion(rot);
		pSwitch->SetScale(Scale);
		pSwitch->Init();
		return pSwitch;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
//�@�X�C�b�`����������
//***************************************
HRESULT CSwitch::Init(void)
{
	// ������
	CBlock::Init();

	return S_OK;
}

//***************************************
//�@�X�C�b�`�I������
//***************************************
void CSwitch::Uninit(void)
{
	// �j��
	CBlock::Uninit();
}

//***************************************
//�@�X�C�b�`�X�V����
//***************************************
void CSwitch::Update(void)
{
	// �X�V
	CBlock::Update();
}

//***************************************
//�@�X�C�b�`�`�揈��
//***************************************
void CSwitch::Draw(void)
{
	// �`��
	CBlock::Draw();
}