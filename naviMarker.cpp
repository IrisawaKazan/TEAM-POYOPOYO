//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [naviMarker.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviMarker.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNaviMarker* CNaviMarker::Create(const char* filePath, D3DXVECTOR2 size)
{
	// �C���X�^���X�̐���
	CNaviMarker* pNaviMarker = new CNaviMarker;
	if (pNaviMarker == nullptr)
	{
		return nullptr;
	}

	pNaviMarker->SetFilePath(filePath);
	pNaviMarker->SetSize(size);

	// ������
	if (FAILED(pNaviMarker->Init()))
	{
		delete pNaviMarker;
		return nullptr;
	}
	return pNaviMarker;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CNaviMarker::Init(void)
{
	CObject3D::Init();
	m_nBiasID = 0;
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviMarker::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNaviMarker::Update(void)
{
	CObject3D::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviMarker::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_nBiasID;                                        // �o�C�A�XID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	CObject3D::Draw(); // �e�N���X�̕`��

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}