//************************************************************
//
//�@�^�C�g���̏���[title.cpp]
//�@Author:Hatori Kensaku
//
//************************************************************

// �C���N���[�h
#include "title.h"
#include "manager.h"
#include "modelmanager.h"
#include "object2D.h"
#include "titlelogo.h"
#include "titlemanager.h"

// �K��l��ݒ�
// �J����
const D3DXVECTOR3 CTitle::Config::Camera::PosV = { -231.75f,79.0f,-1025.75f };
const D3DXVECTOR3 CTitle::Config::Camera::PosR = { -430.0f,75.0f,-1000.0f };
const D3DXVECTOR3 CTitle::Config::Camera::Rot = { -0.03f,1.7f,0.0f };

// �v���C���[
const D3DXVECTOR3 CTitle::Config::Player::Pos = { -340.0f,0.0f,-1045.0f };

// �ÓI�����o�ϐ�
CTitleManager* CTitle::m_pTitleManager = NULL;

//***************************************
// �R���X�g���N�^
//***************************************
CTitle::CTitle() : CScene(MODE_TITLE)
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CTitle::~CTitle()
{
}

//***************************************
// ����������
//***************************************
HRESULT CTitle::Init(void)
{
	// ���s�����̐F�����Z�b�g
	CManager::GetLight()->ResetCol();
		
	m_pTitleManager = CTitleManager::CreateSingleton();

	return S_OK;
}

//***************************************
// �X�V����
//***************************************
void CTitle::Update(void)
{
}

//***************************************
// �I������
//***************************************
void CTitle::Uninit(void)
{
	delete this;
}

//***************************************
// �`�揈��
//***************************************
void CTitle::Draw(void)
{
}