//****************************************************************
//
// �|�[�Y�̏���[pause.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "pause.h"
#include "manager.h"
#include "pausemanager.h"
#include "math_T.h"
#include "scene.h"
#include "manager.h"
#include "fade.h"
#include "game.h"
#include "title.h"
#include "texturemanager.h"

// �K��l��ݒ�
const D3DXCOLOR CPause::Config::DefoultColor = { 1.0f,1.0f,1.0f,0.5f };
const D3DXCOLOR CPause::Config::SelectColor = { 1.0f,1.0f,1.0f,1.0f };

//***************************************
// �|�[�Y���j���[�̊��N���X
//***************************************
//***************************************
// �R���X�g���N�^
//***************************************
CPause::CPause() : CObject2D(7)
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CPause::~CPause()
{
}

//***************************************
// ����������
//***************************************
HRESULT CPause::Init(void)
{
	CObject2D::Init();
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CPause::Uninit(void)
{
	CObject2D::Uninit();
}

//***************************************
// �X�V����
//***************************************
void CPause::Update(void)
{
	CObject2D::Update();
}

//***************************************
// �`�揈��
//***************************************
void CPause::Draw(void)
{
	if (CManager::isPause() == false) return;
	CObject2D::Draw();
}

//***************************************
// ����
//***************************************
CPause* CPause::Create(D3DXVECTOR3 Pos, Menu Menu)
{
	if (Menu >= CPause::MAX) return NULL;
	CPause* pPause = NULL;
	switch (Menu)
	{
	case CONTINUE:
		pPause = new CContinue;
		break;
	case RETRY:
		pPause = new CRetry;
		break;
	case QUIT:
		pPause = new CQuit;
		break;
	}
	pPause->Init();
	pPause->SetPosition(Pos);
	pPause->SetSize({ CPauseManager::Config::Width,CPauseManager::Config::Height });
	return pPause;
}


//***************************************
// �|�[�Y���j���[�̔h���N���X
//***************************************
//***************************************
// �R���X�g���N�^
//***************************************
CContinue::CContinue()
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CContinue::~CContinue()
{
}

//***************************************
// ����������
//***************************************
HRESULT CContinue::Init(void)
{
	CPause::Init();
	CObject2D::SetTexIndx(CTextureManager::Instance()->Register(Config::FilePath));
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CContinue::Uninit(void)
{
	CPause::Uninit();
}

//***************************************
// �X�V����
//***************************************
void CContinue::Update(void)
{
	// �����̑傫��
	D3DXVECTOR2 Apper = { CPauseManager::Config::Width,CPauseManager::Config::Height };
	// �ڕW�̑傫��
	D3DXVECTOR2 Dest = { CPauseManager::Config::DestWidth,CPauseManager::Config::DestHeight };

	if (CManager::isPause() == false) return;
	if (CPauseManager::GetSingleton()->GetSelectMenu() == CONTINUE)
	{
		// �A�j���[�V�����J�E���^�[��i�߂�
		m_nAnimCounter++;

		// �ݒ肷��傫���̕ϐ�
		D3DXVECTOR2 Size = {};
		// ���̃A�j���[�V�����̐i�s�������v�Z
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CPauseManager::Config::Frame);
		// �ŏI�I�ȑ傫�����珉���̑傫������̍���
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// ���̑傫�����v�Z
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CPause::SetCol(CPause::Config::SelectColor);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			//CManager::GetSound()->Play(CSound::LABEL_ENTER);
			CManager::OffPause();
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CPause::SetCol(CPause::Config::DefoultColor);
	}
	CPause::Update();
}

//***************************************
// �`�揈��
//***************************************
void CContinue::Draw(void)
{
	CPause::Draw();
}


//***************************************
// �|�[�Y���j���[�̔h���N���X
//***************************************
//***************************************
// �R���X�g���N�^
//***************************************
CRetry::CRetry()
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CRetry::~CRetry()
{
}

//***************************************
// ����������
//***************************************
HRESULT CRetry::Init(void)
{
	CPause::Init();
	CObject2D::SetTexIndx(CTextureManager::Instance()->Register(Config::FilePath));
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CRetry::Uninit(void)
{
	CPause::Uninit();
}

//***************************************
// �X�V����
//***************************************
void CRetry::Update(void)
{
	// �����̑傫��
	D3DXVECTOR2 Apper = { CPauseManager::Config::Width,CPauseManager::Config::Height };
	// �ڕW�̑傫��
	D3DXVECTOR2 Dest = { CPauseManager::Config::DestWidth,CPauseManager::Config::DestHeight };

	if (CManager::isPause() == false) return;
	if (CPauseManager::GetSingleton()->GetSelectMenu() == RETRY)
	{
		// �A�j���[�V�����J�E���^�[��i�߂�
		m_nAnimCounter++;

		// �ݒ肷��傫���̕ϐ�
		D3DXVECTOR2 Size = {};
		// ���̃A�j���[�V�����̐i�s�������v�Z
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CPauseManager::Config::Frame);
		// �ŏI�I�ȑ傫�����珉���̑傫������̍���
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// ���̑傫�����v�Z
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CPause::SetCol(CPause::Config::SelectColor);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			if (CManager::GetScene()->GetMode() == CScene::MODE::MODE_GAME)
			{
				//CManager::GetSound()->Play(CSound::LABEL_ENTER);
				CFade::SetFade(new CGame);
			}
			CManager::OffPause();
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CPause::SetCol(CPause::Config::DefoultColor);
	}
	CPause::Update();
}

//***************************************
// �`�揈��
//***************************************
void CRetry::Draw(void)
{
	CPause::Draw();
}


//***************************************
// �|�[�Y���j���[�̔h���N���X
//***************************************
//***************************************
// �R���X�g���N�^
//***************************************
CQuit::CQuit()
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CQuit::~CQuit()
{
}

//***************************************
// ����������
//***************************************
HRESULT CQuit::Init(void)
{
	CPause::Init();
	CObject2D::SetTexIndx(CTextureManager::Instance()->Register(Config::FilePath));
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CQuit::Uninit(void)
{
	CPause::Uninit();
}

//***************************************
// �X�V����
//***************************************
void CQuit::Update(void)
{
	// �����̑傫��
	D3DXVECTOR2 Apper = { CPauseManager::Config::Width,CPauseManager::Config::Height };
	// �ڕW�̑傫��
	D3DXVECTOR2 Dest = { CPauseManager::Config::DestWidth,CPauseManager::Config::DestHeight };

	if (CManager::isPause() == false) return;
	if (CPauseManager::GetSingleton()->GetSelectMenu() == QUIT)
	{
		// �A�j���[�V�����J�E���^�[��i�߂�
		m_nAnimCounter++;

		// �ݒ肷��傫���̕ϐ�
		D3DXVECTOR2 Size = {};
		// ���̃A�j���[�V�����̐i�s�������v�Z
		float Ratio = EaseInOutSine((float)m_nAnimCounter / CPauseManager::Config::Frame);
		// �ŏI�I�ȑ傫�����珉���̑傫������̍���
		D3DXVECTOR2 Diff = { Dest.x - Apper.x,Dest.y - Apper.y };
		// ���̑傫�����v�Z
		Size = Apper + Diff * Ratio;

		CObject2D::SetSize(Size);
		CPause::SetCol(CPause::Config::SelectColor);
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_A) == true)
		{
			//CManager::GetSound()->Play(CSound::LABEL_ENTER);
			CFade::SetFade(new CTitle);
			CManager::OffPause();
		}
	}
	else
	{
		m_nAnimCounter = 0;
		CObject2D::SetSize(Apper);
		CPause::SetCol(CPause::Config::DefoultColor);
	}
	CPause::Update();
}

//***************************************
// �`�揈��
//***************************************
void CQuit::Draw(void)
{
	CPause::Draw();
}