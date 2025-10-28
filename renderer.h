//****************************************************************
//
// �����_�[����[renderer.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _CRENDERER_H_
#define _CRENDERER_H_

// �C���N���[�h
#include "main.h"

// �O���錾
class CDebugProc;

// �����_�[�̊�{�N���X�ƒ�`
class CRenderer
{
// �p�u���b�N
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CRenderer();
	~CRenderer();

	// �����o�֐�
	HRESULT Init(HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �Z�b�^�[
	void onWireFrame();
	void offWireFrame();
	void ToggleFullscreen(HWND hWnd, bool* isFullScrean);
	void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
	void SetSize(UINT W, UINT H) { m_Width = W, m_Height = H; }

	// �Q�b�^�[
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	// �}���`�e�N�X�`���p
	//LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT; };
	// �E�B���h�E�̃T�C�Y���擾
	RECT GetWindowsRect(void) { return m_RectWindow; }
	bool NeedReset(void) { return (m_Width != NULL && m_Height != NULL); }
// �v���C�x�[�g
private:
	// �ÓI�����o�ϐ�
	static CDebugProc* m_pDebugProc;	// �f�o�b�O�\���p
	// �����o�ϐ�
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// �f�o�C�X
	RECT m_RectWindow;									// �E�B���h�E�̑傫��
	D3DPRESENT_PARAMETERS m_d3dpp;						// �v���[���e�[�V�����p�����[�^
	UINT m_Width;										// Imgui�p��Window�̑傫��(W)
	UINT m_Height;										// Imgui�p��Window�̑傫��(H)
};
#endif // !_CRENDERER_H_