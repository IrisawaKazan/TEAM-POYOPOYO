//*************************************
//
//�@�E�C���h�E�\������[main.cpp]
//�@Author:Hatori Kensaku
//
//*************************************
// �C���N���[�h
#include "main.h"
#include "renderer.h"
#include "manager.h"
#include "debugproc.h"
#include "resource.h"
#include <crtdbg.h>

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int g_nCountFPS = 0;

//*************************************
// ���C���֐�
//*************************************
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
	//_CrtSetBreakAlloc(98294);
	// ���������[�N���m
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);//���������[�N���m

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,
		CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;

	CManager* pManager = NULL;
	pManager = new CManager;

	if (pManager != NULL)
	{
		//����������
		if (FAILED(pManager->Init(hInstance,hWnd, TRUE)))
		{
			return -1;
		}
	}

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();

	// ������
	DWORD dwFrameCount;					// �t���[���J�E���g
	DWORD dwFPSLastTime;				// �Ō��FPS���v����������
	dwFrameCount = 0;					// �t���[���J�E���g��������
	dwFPSLastTime = timeGetTime();		// ���݂̎������擾

	srand((unsigned int)time(NULL));

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();		// ���ݎ����̎擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;	// �ۑ�
				dwFrameCount = 0;				// 0�ɖ߂�
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;	// �ۑ�

				dwFrameCount++;

				//�X�V����
				pManager->Update();

				//�`�揈��
				pManager->Draw();
			}
		}
	}

	//�I������
	if (pManager != NULL)
	{
		pManager->Uninit();
		delete pManager;
		pManager = NULL;
	}

	//
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//*************************************
// �E�C���h�E�v���V�[�W��
//*************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	CRenderer* pRenderer = CManager::GetRenderer();

	switch (uMsg)
	{
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED) return 0;
		// ������ HIWORD �œn���i�ԈႢ�C���j
		pRenderer->SetSize(LOWORD(lParam), HIWORD(lParam));
	return 0;
	case WM_SYSCOMMAND:
		// Disable ALT application menu
		if ((wParam & 0xfff0) == SC_KEYMENU) 
			return 0;
		break;
	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;
	//�L�[�����̃��b�Z�[�W
	case WM_KEYDOWN:
		switch (wParam)
		{
		//ESC�L�[�������ꂽ�ꍇ
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
				break;
			}
			else
			{
				return 0;
			}
		default:
			break;
		}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//*************************************
// FPS���擾
//*************************************
int GetFPS(void)
{
	return g_nCountFPS;
}