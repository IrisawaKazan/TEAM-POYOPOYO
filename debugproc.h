//****************************************************************
//
// �f�o�b�O�\���̏���[debugproc.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��\�C���N���[�h�h�~
#ifndef _DEBUGPROC_H_
#define _DEBUGPROC_H_

// �C���N���[�h
#include "main.h"

// �N���X���`
class CDebugProc
{
public:
	// �f�o�b�O�t�H���g�̋K��l��ݒ�
	struct Config {
		static constexpr int BufferSize = 1024;
		static constexpr int FontSize = 20;
	};
	// �R���X�g���N�^�E�f�X�g���N�^
	CDebugProc();
	~CDebugProc();

	// �����o�֐�
	void Init(void);
	void Uninit(void);

	// �ÓI�����o�֐�
	static void print(const char* fmt, ...);
	static void Draw(int X,int Y);
	static void End(void) { m_nowIndx = 0; };

	// �Q�b�^�[
	static bool GetEnable(void) { return m_Enable; };
	// �Z�b�^�[
	static void SetEnable(bool Enable) { m_Enable = Enable; };
private:
	// �ÓI�����o�ϐ�
	static int m_nowIndx;								// ���̃o�b�t�@�̃C���f�b�N�X
	static LPD3DXFONT m_pFont;							// �������o�͂���p
	static char m_aStr[Config::BufferSize];	// �o�b�t�@
	static bool m_Enable;								// �L�����ǂ���
};
#endif // !_DEBUGPROC_H_