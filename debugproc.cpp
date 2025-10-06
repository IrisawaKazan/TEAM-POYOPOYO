//****************************************************************
//
// �f�o�b�O�\���̏���[debugproc.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "debugproc.h"
#include "manager.h"

// �ÓI�����o�ϐ��錾
LPD3DXFONT CDebugProc::m_pFont = NULL;
char CDebugProc::m_aStr[CDebugProc::Config::BufferSize] = {};
int CDebugProc::m_nowIndx = 0;
bool CDebugProc::m_Enable = true;

//*********************************************
// �R���X�g���N�^
//*********************************************
CDebugProc::CDebugProc()
{

}

//*********************************************
// �f�X�g���N�^
//*********************************************
CDebugProc::~CDebugProc()
{

}

//*********************************************
// ����������
//*********************************************
void CDebugProc::Init()
{
	m_nowIndx = 0;
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, CDebugProc::Config::FontSize, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &m_pFont);

	memset(m_aStr, NULL, sizeof(m_aStr));
}

//*********************************************
// �I������
//*********************************************
void CDebugProc::Uninit(void)
{
	if (m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//*********************************************
// �o�b�t�@�ɏ���o�^
//*********************************************
void CDebugProc::print(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	// ���̕����̈ʒu���ő吔���傫��������
	if (m_nowIndx > CDebugProc::Config::BufferSize)
	{
		// ���̕����̈ʒu���[���ɖ߂�
		m_nowIndx = 0;
		return;
	}
	// �o�b�t�@�ɐςݏグ��
	m_nowIndx += vsprintf(&m_aStr[m_nowIndx], fmt, args);
	va_end(args);
}

//*********************************************
// ���W���w�肵�ĕ`��
//*********************************************
void CDebugProc::Draw(int X, int Y)
{
	if (m_Enable == true)
	{
		// �w�肳�ꂽ�ʒu����`��
		RECT rect = { X,Y,SCREEN_WIDTH,SCREEN_HEIGHT };

		// �e�L�X�g�\��
		m_pFont->DrawText(NULL, &m_aStr[0], -1, &rect, DT_LEFT, WHITE);
		// �o�b�t�@���N���A
		memset(&m_aStr[0], NULL, sizeof(m_aStr));
	}
}