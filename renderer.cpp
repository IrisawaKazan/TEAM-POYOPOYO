//*************************************
//
//�@�����_���[����[renderer.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "renderer.h"
#include "Object.h"
#include "Object2D.h"
#include "debugproc.h"
#include "manager.h"

// ���O���
using namespace std;

// �ÓI�����o�ϐ�
CDebugProc* CRenderer::m_pDebugProc = NULL;

//************************************
// �����_���[�N���X�̃R���X�g���N�^
//************************************
CRenderer::CRenderer()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;
	m_d3dpp = {};
	m_BackBufferCol = BLACK;
}

//************************************
// �����_���[�N���X�̃f�X�g���N�^
//************************************
CRenderer::~CRenderer()
{

}

//************************************
// �����_���[�N���X�̏���������
//************************************
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{
	D3DDISPLAYMODE d3ddm;
	//Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&m_d3dpp, sizeof(m_d3dpp));
	m_d3dpp.BackBufferWidth = SCREEN_WIDTH;
	m_d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.BackBufferCount = 1;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.Windowed = bWindow;
	m_d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3D�f�o�C�X�̐���
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&m_d3dpp,
		&m_pD3DDevice)))
	{//Direct3D�f�o�C�X�̐���
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&m_d3dpp,
			&m_pD3DDevice)))
		{//Direct3D�f�o�C�X�̐���
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&m_d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();

#ifdef _DEBUG
	m_pDebugProc->SetEnable(true);
#else
	m_pDebugProc->SetEnable(false);
#endif // _DEBUG

	return S_OK;
}

//************************************
// �����_���[�N���X�̏I������
//************************************
void CRenderer::Uninit()
{
	//----------------------------
	//�e��I�u�W�F�N�g�̏I������
	//----------------------------

	CObject::ReleaseAll();

	m_pDebugProc->Uninit();
	delete m_pDebugProc;
	m_pDebugProc = NULL;

	//D3D�f�o�C�X�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}
	//D3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//************************************
// �����_���[�N���X�̍X�V����
//************************************
void CRenderer::Update()
{
	CObject::UpdateAll();

	static bool isFullScrean = false;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F11) == true)
	{
		ToggleFullscreen(GetForegroundWindow(), &isFullScrean);
	}
#ifdef _DEBUG
	static bool isWire = false;
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_F1) == true)
	{
		isWire = !isWire;
		if (isWire == true)
		{
			onWireFrame();
		}
		else
		{
			offWireFrame();
		}
	}

	if (CManager::GetInputKeyboard()->GetTrigger(DIK_B) == true)
	{
		CDebugProc::SetEnable(false);
	}
#endif // _DEBUG
}

//************************************
// �����_���[�N���X�̕`�揈��
//************************************
void CRenderer::Draw()
{
	//��ʃN���A
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL),
		m_BackBufferCol, 1.0f, 0);

	//�`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//�`��J�n������
		//-----------------------------
		//�e��I�u�W�F�N�g�̕`�揈��
		//-----------------------------

		// �I�u�W�F�N�g�̕`��
		CObject::DrawAll();

		CDebugProc::print("FPS[%d]\n", GetFPS());
		CDebugProc::print("�J�����̌���X.Y.Z	[%.2f] [%.2f] [%.2f]\n", CManager::GetCamera()->GetRot().x, CManager::GetCamera()->GetRot().y, CManager::GetCamera()->GetRot().z);

		CDebugProc::Draw(0, 0);
		CDebugProc::End();

		//�`��I��
		m_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//*************************************
// ���C���[�t���[�����I���ɂ���
//*************************************
void CRenderer::onWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}

//*************************************
// ���C���[�t���[����؂�
//*************************************
void CRenderer::offWireFrame()
{
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//*************************************
// �t�H�O��L���ɂ���
//*************************************
void CRenderer::onFog()
{
	SetupVertexFog(D3DCOLOR_XRGB(200, 200, 200), D3DFOG_LINEAR, TRUE, 0.01f);
}

//*************************************
// �t�H�O�𖳌��ɂ���
//*************************************
void CRenderer::offFog()
{
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}

//*************************************
// �t���X�N���[����؂�ւ���
//*************************************
void CRenderer::ToggleFullscreen(HWND hWnd, bool* isFullScrean)
{
	// ���݂̃E�B���h�E�X�^�C�����擾
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (*isFullScrean)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, m_RectWindow.left, m_RectWindow.top,
			m_RectWindow.right - m_RectWindow.left, m_RectWindow.bottom - m_RectWindow.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(hWnd, &m_RectWindow);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	bool isScrean = *isFullScrean;
	*isFullScrean = !isScrean;
}

//*************************************
// �t�H�O��ݒ�
//*************************************
void CRenderer::SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density)
{
	float Start = 360.0f,    // Linear fog distances
		End = 5000.0f;

	// Enable fog blending.
	m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	// Set the fog color.
	m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, Color);
	// Set fog parameters.
	if (D3DFOG_LINEAR == Mode)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&Start));
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&End));
	}
	else
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		m_pD3DDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&Density));
	}

	// Enable range-based fog if desired (only supported for
	//   vertex fog). For this example, it is assumed that UseRange
	//   is set to a nonzero value only if the driver exposes the
	//   D3DPRASTERCAPS_FOGRANGE capability.
	// Note: This is slightly more performance intensive
	//   than non-range-based fog.

	if (UseRange) m_pD3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
}