//*************************************
//
//�@�v���R���p�C���ς݃w�b�^�[[pch.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _PCH_H_
#define _PCH_H_

// �r���h���x���Ώ��}�N��
#define _CRT_SECURE_NO_WARNINGS

// �C���N���[�h
#include<Windows.h>
#include "d3dx9.h"

// �r���h���x���Ώ��}�N��
#define DIRECTINPUT_VERSION (0x0800)

// �C���N���[�h
#include<stdio.h>

#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"

#include "string.h"
#include "stdlib.h"
#include "debugproc.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <shlwapi.h>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
#pragma comment(lib, "shlwapi.lib")

// �}�N����`
// ���_���̃X�g���[�����}�N����`
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// �ʒu�A���W�ϊ��p�ϐ��A�F�A�e�N�X�`��
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���W�E�@���E�J���[�E�e�N�X�`��
#define FVF_VERTEX_LINE (D3DFVF_XYZ | D3DFVF_DIFFUSE)								// ���W�E�J���[

// �Q�[���Ɏg���}�N��
#define CLASS_NAME "WindowsClass"
#define WINDOW_NAME "TeamGame"

// �E�B���h�E
#define SCREEN_WIDTH UINT(1280)
#define SCREEN_HEIGHT UINT(720)
#define SCREEN_CENTER D3DXVECTOR2(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f)

// �ő�l
#define MAX_WORD (4096)

// �^�̏����l
#define VEC3_NULL D3DXVECTOR3(0.0f,0.0f,0.0f)
#define VEC_UP D3DXVECTOR3(0.0f,1.0f,0.0f)
#define VEC2_NULL D3DXVECTOR2(0.0f,0.0f)
#define VEC3 D3DXVECTOR3
#define VEC2 D3DXVECTOR2

// �F
#define FOG D3DXCOLOR(0.8f,0.8f,0.8f,0.6f)
#define WHITE D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)
#define BLACK D3DXCOLOR(0.0f,0.0f,0.0f,1.0f)
#define BURNTSCUM D3DXCOLOR(0.4f,0.4f,0.4f,1.0f)
#define RED D3DXCOLOR(1.0f,0.0f,0.0f,1.0f)
#define GREEN D3DXCOLOR(0.0f,1.0f,0.0f,1.0f)
#define BLUE D3DXCOLOR(0.0f,0.0f,1.0f,1.0f)
#define BROWN D3DXCOLOR(0.62f,0.28f,0.16f,1.0f)
#define PURPLE D3DXCOLOR(0.8f,0.0f,0.9f,1.0f)
#define FIRE D3DXCOLOR(1.0f,0.3f,0.1f,1.0f)
#define WATERRIPPLE D3DXCOLOR(0.7f,0.7f,0.8f,0.7f)
#define WATERRIPPLEOUT D3DXCOLOR(0.7f,0.7f,0.8f,1.0f)
#define FIRESMOKE D3DXCOLOR(0.5f,0.5f,0.5f,1.0f)
#define ARROWORBIT D3DXCOLOR(0.9f,0.8f,0.0f,1.0f)

// �ϊ�
#define TOSECOND (60)
#define TOPI (float)(0.0174f)

// �O���錾
class CRenderer;
class CObject;

//���_���(2D)�̍\����
typedef struct
{
	D3DXVECTOR3 pos;// ���_���W
	float rhw;		// ���W�ϊ��p�W��
	D3DCOLOR col;	// ���_�J���[
	D3DXVECTOR2 tex;// �e�N�X�`�����W
}VERTEX_2D;

//���_���(3D)�̍\����
typedef struct
{
	D3DXVECTOR3 pos;// ���_���W
	D3DXVECTOR3 nor;// �@���x�N�g��
	D3DCOLOR col;	// ���_�J���[
	D3DXVECTOR2 tex;// �e�N�X�`�����W
}VERTEX_3D;

//���_���(Line)�̍\����
typedef struct
{
	D3DXVECTOR3 pos;// ���_���W
	D3DCOLOR col;	// ���_�J���[
}VERTEX_LINE;

#endif // !_PCH_H_