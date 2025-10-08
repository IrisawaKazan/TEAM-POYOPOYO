//*************************************
//
//�@���͏���[input.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _INPUT_H_
#define _INPUT_H_

// �C���N���[�h
#include "renderer.h"

// ���N���X���`[���͂ɋ��ʂ��ĕK�v�ȃN���X]
class CInput
{
public:
	// ���̓f�o�C�X�̋K��l��ݒ�
	struct InputConfig {
		static constexpr int NumKey = 256;
		static constexpr int DeadZone = 10000;
	};
	// �R���X�g���N�^�E�f�X�g���N�^
	CInput();
	virtual ~CInput();

	// �����o�֐�
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit(void);
	virtual void Update(void) = 0;
protected:
	// �ÓI�����o�ϐ�
	static LPDIRECTINPUT8 m_pInput;	// ���͏����ɕK�v�ȋ��ʂ̕ϐ�
	// �����o�ϐ�
	LPDIRECTINPUTDEVICE8 m_pDevice;	// ���ꂼ��̓��͏����ɕK�v�ȕϐ�
private:
};



// �h���N���X���`[�L�[�{�[�h]
class CInputKeyboard : public CInput
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CInputKeyboard();
	~CInputKeyboard();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void Uninit(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetRepeat(int nKey);
protected:
	// �����o�ϐ�
	BYTE m_aKeyState[InputConfig::NumKey];		// ���̃L�[
	BYTE m_OldaKeyState[InputConfig::NumKey];	// 1F�O�̃L�[
private:

};



// �h���N���X���`[�}�E�X]
class CInputMouse
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CInputMouse();
	~CInputMouse();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool OnPress(int button_type);
	bool OnDown(int button_type);
	bool OnUp(int button_type);

	// �Q�b�^�[
	bool IsMove(void);
	D3DXVECTOR2 GetVelocity(void) { return GetPos() - GetPosOld(); };
	D3DXVECTOR2 GetPosOld(void);
	D3DXVECTOR2 GetPos(void);

	// �Z�b�^�[
	DIMOUSESTATE GetMouseState(void) { return m_MouseState; };

protected:
	// �ÓI�����o�ϐ�
	static LPDIRECTINPUT8 m_pInputMouse;	// �}�E�X�̓��͂ɕK�v�ȕϐ�
	// �����o�ϐ�
	LPDIRECTINPUTDEVICE8 m_pDeviceMouse;	// �}�E�X�̓��͏����ɕK�v�ȕϐ�
	DIMOUSESTATE m_MouseState;				// �}�E�X�̃X�e�[�g
	DIMOUSESTATE m_MouseOldState;			// �}�E�X�̃X�e�[�g
};



// �h���N���X���`[�W���C�p�b�g]
class CInputJoypad : public CInput
{
public:
	//�L�[�̎��
	typedef enum
	{
		JOYKEY_UP = 0,
		JOYKEY_DOWN,
		JOYKEY_LEFT,
		JOYKEY_RIGET,
		JOYKEY_START,
		JOYKEY_BACK,
		JOYKEY_L3,
		JOYKEY_R3,
		JOYKEY_L1,
		JOYKEY_R1,
		JOYKEY_L2,
		JOYKEY_R2,
		JOYKEY_A,
		JOYKEY_B,
		JOYKEY_X,
		JOYKEY_Y,
		JOYKEY_MAX
	}JOYKEY;

	// �R���X�g���N�^�E�f�X�g���N�^
	CInputJoypad();
	~CInputJoypad();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Update(void);
	void Uninit(void);

	// �Q�b�^�[
	bool GetPress(JOYKEY nKey);
	bool GetTrigger(JOYKEY nKey);
	bool GetRelease(JOYKEY nKey);
	bool GetJoyStickL(void);
	bool GetJoyStickR(void);
	XINPUT_STATE* GetJoyStickAngle(void) {	return &m_JoyKeyState;	};

protected:
	// �����o�ϐ�
	XINPUT_STATE m_JoyKeyState;			// �W���C�p�b�g�̃X�e�[�g
	XINPUT_STATE m_JoyKeyStateOld;		// �W���C�p�b�h�̐̂̏��
private:

};
#endif // !_INPUT_H_