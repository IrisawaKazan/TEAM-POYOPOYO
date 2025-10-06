//*************************************
//
//�@���͏���[input.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "input.h"

// �ÓI�����o�ϐ��錾
LPDIRECTINPUT8 CInput::m_pInput = NULL;
LPDIRECTINPUT8 CInputMouse::m_pInputMouse = NULL;

//*********************************************
// �C���v�b�g�N���X�̃R���X�g���N�^
//*********************************************
CInput::CInput()
{
	m_pDevice = NULL;
}

//*********************************************
// �C���v�b�g�N���X�̃f�X�g���N�^
//*********************************************
CInput::~CInput()
{

}

//*********************************************
// �C���v�b�g�N���X�̏���������
//*********************************************
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	if (m_pInput == NULL)
	{
		//�L�[�{�[�h������
		if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			return E_FAIL;
		}
		//
		if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
		{
			return E_FAIL;
		}
		//�f�[�^�t�H�[�}�b�g��ݒ�
		if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
		{
			return E_FAIL;
		}
		//�������[�h��ݒ�
		if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
		{
			return E_FAIL;
		}

		//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
		m_pDevice->Acquire();

		return S_OK;
	}
	return E_FAIL;
}

//*********************************************
// �C���v�b�g�N���X�̏I������
//*********************************************
void CInput::Uninit(void)
{
	//���̓f�o�C�X�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}


//*********************************************
//*		 �h���N���X�̏���[�}�E�X]			  *
//*********************************************

//*********************************************
// �C���v�b�g�}�E�X�N���X�̃R���X�g���N�^
//*********************************************
CInputMouse::CInputMouse()
{
	m_pInputMouse = NULL;
	m_pDeviceMouse = NULL;
	//�������̃N���A
	memset(&m_MouseOldState, NULL, sizeof(DIMOUSESTATE));
	memset(&m_MouseState, NULL, sizeof(DIMOUSESTATE));
}

//*********************************************
// �C���v�b�g�}�E�X�N���X�̃f�X�g���N�^
//*********************************************
CInputMouse::~CInputMouse()
{

}

//*********************************************
// �C���v�b�g�}�E�X�N���X�̏���������
//*********************************************
HRESULT CInputMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//�L�[�{�[�h������
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInputMouse, NULL)))
	{
		return E_FAIL;
	}
	//
	if (FAILED(m_pInputMouse->CreateDevice(GUID_SysMouse, &m_pDeviceMouse, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDeviceMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(m_pDeviceMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDeviceMouse->Acquire();

	//�������̃N���A
	memset(&m_MouseOldState, NULL, sizeof(DIMOUSESTATE));
	memset(&m_MouseState, NULL, sizeof(DIMOUSESTATE));

	return S_OK;
}

//*********************************************
// �C���v�b�g�}�E�X�N���X�̏I������
//*********************************************
void CInputMouse::Uninit(void)
{
	//���̓f�o�C�X�̔j��
	if (m_pDeviceMouse != NULL)
	{
		m_pDeviceMouse->Unacquire();
		m_pDeviceMouse->Release();
		m_pDeviceMouse = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInputMouse != NULL)
	{
		m_pInputMouse->Release();
		m_pInputMouse = NULL;
	}
}


//*********************************************
// �C���v�b�g�}�E�X�N���X�̍X�V����
//*********************************************
void CInputMouse::Update(void)
{
	// �X�V�O�ɍŐV�}�E�X����ۑ�����
	m_MouseOldState = m_MouseState;

	// �ŐV�̃}�E�X�̏�Ԃ��X�V
	HRESULT	hr = m_pDeviceMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
	if (FAILED(hr))
	{
		m_pDeviceMouse->Acquire();
	}
	POINT p;
	GetCursorPos(&p);
	ScreenToClient(FindWindowA(CLASS_NAME, WINDOW_NAME), &p);
	m_MouseState.lX = p.x;
	m_MouseState.lY = p.y;
}

//*********************************************
// �}�E�X�̃v���X�����擾
//*********************************************
bool CInputMouse::OnPress(int button_type)
{
	return(m_MouseState.rgbButtons[button_type] & (0x80)) ? true : false;
}

//*********************************************
// �}�E�X�̉������u��
//*********************************************
bool CInputMouse::OnDown(int button_type)
{
	if (!(m_MouseOldState.rgbButtons[button_type] & (0x80)) &&
		m_MouseState.rgbButtons[button_type] & (0x80))
	{
		return true;
	}

	return false;
}

//*********************************************
// �}�E�X�̗������u��
//*********************************************
bool CInputMouse::OnUp(int button_type)
{
	if (m_MouseOldState.rgbButtons[button_type] & (0x80) &&
		!(m_MouseState.rgbButtons[button_type] & (0x80)))
	{
		return true;
	}

	return false;
}

//*********************************************
// �}�E�X�̈ʒu���擾
//*********************************************
D3DXVECTOR2 CInputMouse::GetPos(void)
{
	return D3DXVECTOR2((float)m_MouseState.lX, (float)m_MouseState.lY);
}

//*********************************************
// �}�E�X�������Ă��邩�ǂ���
//*********************************************
bool CInputMouse::IsMove(void)
{
	if (m_MouseState.lX == m_MouseOldState.lX &&
		m_MouseState.lY == m_MouseOldState.lY)
	{
		return false;
	}
	return true;
}

//*********************************************
// �}�E�X�̐̂̈ʒu���擾
//*********************************************
D3DXVECTOR2 CInputMouse::GetPosOld(void)
{
	return D3DXVECTOR2((float)m_MouseOldState.lX, (float)m_MouseOldState.lY);
}


//*********************************************
//*		 �h���N���X�̏���[�L�[�{�[�h]		  *
//*********************************************

//*********************************************
// �C���v�b�g�L�[�{�[�h�N���X�̃R���X�g���N�^
//*********************************************
CInputKeyboard::CInputKeyboard()
{
	for (int nCount = 0; nCount < InputConfig::NumKey; nCount++)
	{
		m_aKeyState[nCount] = NULL;
		m_OldaKeyState[nCount] = NULL;
	}
}

//*********************************************
// �C���v�b�g�L�[�{�[�h�N���X�̃f�X�g���N�^
//*********************************************
CInputKeyboard::~CInputKeyboard()
{

}

//*********************************************
// �C���v�b�g�L�[�{�[�h�N���X�̏���������
//*********************************************
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = CInput::Init(hInstance, hWnd);
	return hr;
}

//*********************************************
// �C���v�b�g�L�[�{�[�h�N���X�̏I������
//*********************************************
void CInputKeyboard::Uninit(void)
{
	CInput::Uninit();
}


//*********************************************
// �C���v�b�g�L�[�{�[�h�N���X�̍X�V����
//*********************************************
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[InputConfig::NumKey] = {};
	int nCntKey;

	for (nCntKey = 0; nCntKey < InputConfig::NumKey; nCntKey++)
	{
		m_OldaKeyState[nCntKey] = m_aKeyState[nCntKey];
	}

	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < InputConfig::NumKey; nCntKey++)
		{
			m_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//*********************************************
//�L�[�{�[�h�̃v���X�����擾
//*********************************************
bool CInputKeyboard::GetPress(int nKey)
{
	return(m_aKeyState[nKey] & 0x080) ? true : false;
}

//*********************************************
//�L�[�{�[�h�̃g���K�[�����擾
//*********************************************
bool CInputKeyboard::GetTrigger(int nKey)
{
	bool Trigger = false;
	if (m_aKeyState[nKey] & 0x80 && !(m_OldaKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//*********************************************
//�L�[�{�[�h�̃��s�[�g�����擾
//*********************************************
bool CInputKeyboard::GetRepeat(int nKey)
{
	bool Trigger = false;
	if (m_OldaKeyState[nKey] & 0x80 && (m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//*********************************************
//�L�[�{�[�h�̃����[�X�����擾
//*********************************************
bool CInputKeyboard::GetRelease(int nKey)
{
	bool Trigger = false;
	if (m_OldaKeyState[nKey] & 0x80 && !(m_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}


//*********************************************
//*		 �h���N���X�̏���[�W���C�p�b�g]		  *
//*********************************************

//*********************************************
// �C���v�b�g�W���C�p�b�g�N���X�̃R���X�g���N�^
//*********************************************
CInputJoypad::CInputJoypad()
{
	//�������̃N���A
	memset(&m_JoyKeyState, NULL, sizeof(XINPUT_STATE));
	memset(&m_JoyKeyStateOld, NULL, sizeof(XINPUT_STATE));
}

//*********************************************
// �C���v�b�g�W���C�p�b�g�N���X�̃f�X�g���N�^
//*********************************************
CInputJoypad::~CInputJoypad()
{

}

//*********************************************
// �C���v�b�g�W���C�p�b�g�N���X�̏���������
//*********************************************
HRESULT CInputJoypad::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = CInput::Init(hInstance, hWnd);
	//�������̃N���A
	memset(&m_JoyKeyState, NULL, sizeof(XINPUT_STATE));
	memset(&m_JoyKeyStateOld, NULL, sizeof(XINPUT_STATE));

	//XInput�̃X�e�[�g��ݒ�(�L��)
	XInputEnable(true);

	return hr;
}

//*********************************************
// �C���v�b�g�W���C�p�b�g�N���X�̏I������
//*********************************************
void CInputJoypad::Uninit(void)
{
	CInput::Uninit();
	//XInput�̃X�e�[�g��ݒ�(����)
	XInputEnable(false);
}


//*********************************************
// �C���v�b�g�W���C�p�b�g�N���X�̍X�V����
//*********************************************
void CInputJoypad::Update(void)
{
	XINPUT_STATE joykeyState;
	//
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		m_JoyKeyStateOld = m_JoyKeyState;
		m_JoyKeyState = joykeyState;
	}
}

//*********************************************
// �W���C�p�b�g�̃v���X�����擾
//*********************************************
bool CInputJoypad::GetPress(JOYKEY nKey)
{
	switch (nKey)
	{
	case JOYKEY_L2:
		return (m_JoyKeyState.Gamepad.bLeftTrigger) ? true : false;
	case JOYKEY_R2:
		return (m_JoyKeyState.Gamepad.bRightTrigger) ? true : false;
	default:
		break;
	}
	return (m_JoyKeyState.Gamepad.wButtons & (0x01 << nKey)) ? true : false;
}

//*********************************************
// �W���C�p�b�g�̃g���K�[�����擾
//*********************************************
bool CInputJoypad::GetTrigger(JOYKEY nKey)
{
	switch (nKey)
	{
	case JOYKEY_L2:
		return (m_JoyKeyState.Gamepad.bLeftTrigger >= 30 && m_JoyKeyStateOld.Gamepad.bLeftTrigger < 30);
	case JOYKEY_R2:
		return (m_JoyKeyState.Gamepad.bRightTrigger >= 30 && m_JoyKeyStateOld.Gamepad.bRightTrigger < 30);
	default:
		break;
	}
	return ((m_JoyKeyState.Gamepad.wButtons & (0x01 << nKey)) && !(m_JoyKeyStateOld.Gamepad.wButtons & (0x01 << nKey))) ? true : false;
}

//*********************************************
// �W���C�p�b�g�̃����[�X�����擾
//*********************************************
bool CInputJoypad::GetRelease(JOYKEY nKey)
{
	switch (nKey)
	{
	case JOYKEY_L2:
		return (m_JoyKeyState.Gamepad.bLeftTrigger < 30 && m_JoyKeyStateOld.Gamepad.bLeftTrigger >= 30);
	case JOYKEY_R2:
		return (m_JoyKeyState.Gamepad.bRightTrigger < 30 && m_JoyKeyStateOld.Gamepad.bRightTrigger >= 30);
	default:
		break;
	}
	return (!(m_JoyKeyState.Gamepad.wButtons & (0x01 << nKey)) && (m_JoyKeyStateOld.Gamepad.wButtons & (0x01 << nKey))) ? true : false;
}

//*********************************************
// �W���C�p�b�g���f�b�h�]�[�����Ȃ���(L)
//*********************************************
bool CInputJoypad::GetJoyStickL(void)
{
	bool joykey = false;

	if (m_JoyKeyState.Gamepad.sThumbLX >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbLX <= -InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbLY >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbLY <= -InputConfig::DeadZone)
	{
		joykey = true;
	}
	return joykey;
}

//*********************************************
// �W���C�p�b�g���f�b�h�]�[�����Ȃ���(R)
//*********************************************
bool CInputJoypad::GetJoyStickR(void)
{
	bool joykey = false;

	if (m_JoyKeyState.Gamepad.sThumbRX >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbRX <= -InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbRY >= InputConfig::DeadZone
		|| m_JoyKeyState.Gamepad.sThumbRY <= -InputConfig::DeadZone)
	{
		joykey = true;
	}
	return joykey;
}