//*************************************
//
//�@Manager[manager.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "object3D.h"
#include "object2D.h"
#include "objectX.h"

// ���O���
using namespace std;

// �ÓI�����o�ϐ��錾
CRenderer* CManager::m_Renderer = NULL;
CInputKeyboard* CManager::m_pInputKeyboard = NULL;
CInputJoypad* CManager::m_pInputJoypad = NULL;
CInputMouse* CManager::m_pInputMouse = NULL;
CSound* CManager::m_pSound = NULL;
CCamera* CManager::m_pCamera = NULL;
CLight* CManager::m_pLight = NULL;
CPlayer* CManager::m_pPlayer = NULL;
bool CManager::m_isPause = false;
bool CManager::m_isClear = false;

//***************************************
// �R���X�g���N�^
//***************************************
CManager::CManager()
{
}

//***************************************
// �f�X�g���N�^
//***************************************
CManager::~CManager()
{

}

//***************************************
// ����������
//***************************************
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWnd)
{
	HRESULT hr;

	// �ÓI�����o�ϐ��̈ꕔ���������m��
	m_Renderer = new CRenderer;
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputJoypad = new CInputJoypad;
	m_pInputMouse = new CInputMouse;
	m_pSound = new CSound;
	m_pCamera = new CCamera;
	m_pLight = new CLight;

	// �������m�ۂł�����
	if (m_Renderer != NULL)
	{
		// ����������
		hr = m_Renderer->Init(hWnd, bWnd);
	}

	// �������m�ۂł�����
	if (m_pInputKeyboard != NULL)
	{
		// ����������
		hr = m_pInputKeyboard->Init(hInstance, hWnd);
	}
	// �������m�ۂł�����
	if (m_pInputJoypad != NULL)
	{
		// ����������
		hr = m_pInputJoypad->Init(hInstance, hWnd);
	}
	// �������m�ۂł�����
	if (m_pInputMouse != NULL)
	{
		// ����������
		hr = m_pInputMouse->Init(hInstance, hWnd);
	}
	// �������m�ۂł�����
	if (m_pSound != NULL)
	{
		// ����������
		hr = m_pSound->Init(hWnd);
	}

	// �RD�ɕK�v�Ȃ��̂�������
	m_pCamera->Init();
	m_pLight->Init();

	CObject3D::Create(VEC3_NULL, VEC3_NULL);
	//CObjectX::Create(VEC3_NULL, VEC3_NULL, "data\\MODEL\\ie.x");
	m_pPlayer->Create(VEC3_NULL, VEC3_NULL);
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CManager::Uninit()
{
	// �J�������g���Ă�����
	if (m_pCamera != NULL)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera = NULL;
	}

	// �����_���[�N���X���g���Ă�����
	if (m_Renderer != NULL)
	{
		m_Renderer->Uninit();
		delete m_Renderer;
		m_Renderer = NULL;
	}

	// �L�[�{�[�h���g���Ă�����
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}
	// �Q�[���p�b�h���g���Ă�����
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Uninit();
		delete m_pInputJoypad;
		m_pInputJoypad = NULL;
	}
	// Mouse���g���Ă�����
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Uninit();
		delete m_pInputMouse;
		m_pInputMouse = NULL;
	}

	// �T�E���h�N���X���g���Ă�����
	if (m_pSound != NULL)
	{
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = NULL;
	}

	// ���C�g���g���Ă�����
	if (m_pLight != NULL)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = NULL;
	}

	// �e�N�X�`���}�l�[�W���[�̔j��
	CTextureManager::Instance()->Unload();

	// ���f���}�l�[�W���[�̔j��
	CModelManager::Instance()->Unload();
}

//***************************************
// �X�V����
//***************************************
void CManager::Update()
{
	// �J�����̃A�b�v�f�[�g
	m_pCamera->Update();

	// ���C�g�̃A�b�v�f�[�g
	m_pLight->Update();

	if (m_isPause == false)
	{
		if (m_Renderer != NULL)
		{
			m_Renderer->Update();
		}
	}

	// �L�[�{�[�h���g���Ă�����X�V
	if (m_pInputKeyboard != NULL)
	{
		m_pInputKeyboard->Update();
	}
	// �Q�[���p�b�h���g���Ă�����X�V
	if (m_pInputJoypad != NULL)
	{
		m_pInputJoypad->Update();
	}
	// Mouse���g���Ă�����X�V
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Update();
	}
}

//***************************************
// �`�揈��
//***************************************
void CManager::Draw()
{
	if (m_Renderer != NULL)
	{
		m_Renderer->Draw();
	}
}

//***************************************
// �v���C���[�̃��X�|�[��
//***************************************
void CManager::RespawPlayer(void)
{
}