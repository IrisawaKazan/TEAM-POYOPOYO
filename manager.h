//*************************************
//
//�@Manager[manager.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _MANAGER_H_
#define _MANAGER_H_

// �C���N���[�h
#include "main.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"

// �O���錾
class CObject3D;
class CShaderFade;

// �N���X���`
class CManager
{
public:
	// �K��l
	struct Config {
		struct FilePath {
			static constexpr const char* TexList = "data\\TEXT\\TexFileList.txt";		// �e�N�X�`�����X�g
			static constexpr const char* ModelList = "data\\TEXT\\ModelList.txt";		// ���f�����X�g
		};
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CManager();
	~CManager();

	// �����o�֐�
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ÓI�����o�֐�
	// �Z�b�^�[
	static void ChangePause(void) { m_isPause = !m_isPause; };
	static void OffPause(void) { m_isPause = false; };
	static void OnPause(void) { m_isPause = true; };
	static void SetClear(const bool isClear) { m_isClear = isClear; };

	// �Q�b�^�[
	static CRenderer* GetRenderer(void) { return m_Renderer; };
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; };
	static CInputJoypad* GetInputJoypad(void) { return m_pInputJoypad; };
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; };
	static CSound* GetSound(void) { return m_pSound; };
	static CCamera* GetCamera(void) { return m_pCamera; };
	static bool GetIsClear(void) { return m_isClear; };
	static CShaderFade* GetShaderFade(void) { return m_pFadeShader; };
	static CLight* GetLight(void) { return m_pLight; };

	// �v���C���[�����X�|�[��������
	static void RespawPlayer(void);

	// �������̊֐���
	static bool isPause(void) { return m_isPause; };
private:
	// �ÓI�����o�ϐ�
	static CRenderer* m_Renderer;				// �����_���[�����p
	static CInputKeyboard* m_pInputKeyboard;	// �L�[�{�[�h�̃C���X�^���X
	static CInputJoypad* m_pInputJoypad;		// �Q�[���p�b�h�̃C���X�^���X
	static CInputMouse* m_pInputMouse;			// �}�E�X�̃C���X�^���X
	static CSound* m_pSound;					// ���̃C���X�^���X
	static CCamera* m_pCamera;					// �J�����̃C���X�^���X
	static CLight* m_pLight;					// ���C�g�̃C���X�^���X
	static CShaderFade* m_pFadeShader;			// �V�F�[�_�t�F�[�h
	static bool m_isPause;						// �|�[�Y�����ǂ���
	static bool m_isClear;						// �G��S�ł��������ǂ���
};
#endif // !_MANAGER_H_