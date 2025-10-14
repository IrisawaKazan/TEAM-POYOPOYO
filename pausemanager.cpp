//****************************************************************
//
// �|�[�Y�}�l�[�W���[�̏���[pausemanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "pausemanager.h"
#include "manager.h"
#include "math_T.h"

// �V���O���g����錾
CPauseManager* CPauseManager::m_Singleton = NULL;

//***************************************
// �f�X�g���N�^
//***************************************
CPauseManager::~CPauseManager()
{
}

//***************************************
// ����������
//***************************************
HRESULT CPauseManager::Init(void)
{
	m_SelectMenu = CPause::CONTINUE;
	for (int nCount = 0; nCount < CPause::MAX; nCount++)
	{
		CPause* pPause = CPause::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f + Config::OffSet * nCount, 0.0f), (CPause::Menu)nCount);
		m_apPauseMenu.push_back(pPause);
	}
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CPauseManager::Uninit(void)
{
	if (m_Singleton != NULL)
	{
		m_apPauseMenu.clear();
		delete m_Singleton;
		m_Singleton = NULL;
	}
}

//***************************************
// �X�V����
//***************************************
void CPauseManager::Update(void)
{
	if (CManager::GetCamera()->IsAnim() == false)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
		{
			CManager::ChangePause();
			m_SelectMenu = CPause::CONTINUE;
		}
	}

	if (CManager::isPause() == true)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_UP) == true)
		{
			m_SelectMenu = (CPause::Menu)Wrap(m_SelectMenu - 1, 0, (int)CPause::MAX - 1);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_DOWN) == true)
		{
			m_Singleton->m_SelectMenu = (CPause::Menu)Wrap(m_SelectMenu + 1, 0, (int)CPause::MAX - 1);
		}
		for (auto iter = m_apPauseMenu.begin(); iter != m_apPauseMenu.end(); iter++)
		{
			(*iter)->Update();
		}
	}
}

//***************************************
// �V���O���g���𐶐�
//***************************************
CPauseManager* CPauseManager::CreateSingleton(void)
{
	if (m_Singleton != NULL) return m_Singleton;
	m_Singleton = new CPauseManager;
	m_Singleton->Init();
	return m_Singleton;
}