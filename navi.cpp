//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNavi* CNavi::Create(void)
{
	CNavi* pNavi = new CNavi;
	if (pNavi == NULL)
	{
		return NULL;
	}
	if (FAILED(pNavi->Init()))
	{
		delete pNavi;
		return NULL;
	}
	return pNavi;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CNavi::Init(void)
{
	CObjectBillBoard::Init();
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNavi::Uninit(void)
{
	CObjectBillBoard::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNavi::Update(void)
{
	CreateRay(CManager::GetInputMouse()->GetPos());
	CObjectBillBoard::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNavi::Draw(void)
{
	CObjectBillBoard::Draw();
}

//--------------------------------
// ���C���쐬
//--------------------------------
void CNavi::CreateRay(D3DXVECTOR2 mousePos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	// �ˉe�s��̎擾
	D3DXMATRIX mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�}�g���b�N�X�̎擾
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �n�_�ƏI�_
	D3DXVECTOR3 vNear, vFar;

	// �j�A�N���b�v�ʏ��3D���W���v�Z
	D3DXVECTOR3 screenNear(mousePos.x, mousePos.y, 0.0f);
	D3DXVec3Unproject(
		&vNear,
		&screenNear,
		&viewPort,
		&mtxProj,
		&mtxView,
		NULL
	);

	// �t�@�[�N���b�v�ʏ��3D���W���v�Z
	D3DXVECTOR3 screenFar(mousePos.x, mousePos.y, 1.0f);
	D3DXVec3Unproject(
		&vFar,
		&screenFar,
		&viewPort,
		&mtxProj,
		&mtxView,
		NULL
	);

	// ���C�̎n�_�ƕ���������
	D3DXVECTOR3 rayOrigin = vNear;                   // ���C�̎n�_�̓j�A�N���b�v�ʏ�̓_
	D3DXVECTOR3 rayDirection = vFar - vNear;         // �j�A����t�@�[�֌������x�N�g��
	D3DXVec3Normalize(&rayDirection, &rayDirection); // �����x�N�g���Ȃ̂Ő��K������
}