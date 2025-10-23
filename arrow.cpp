//--------------------------------
//
// �₶�邵 [arrow.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "arrow.h"
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
CArrow* CArrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filePath, D3DXVECTOR2 size, size_t idx)
{
	// �C���X�^���X�̐���
	CArrow* pArrow = new CArrow;
	if (pArrow == nullptr)
	{
		return nullptr;
	}

	pArrow->SetFilePath(filePath);
	pArrow->SetSize(size);
	pArrow->SetPosition(pos);
	pArrow->SetRotasion(rot);
	pArrow->SetLength(D3DXVec2Length(&size));
	pArrow->SetChengeLength(D3DXVec2Length(&size) * CHENGE_LENGTH_MAGNIFICATION);
	pArrow->SetIdx(idx);

	// ������
	if (FAILED(pArrow->Init()))
	{
		delete pArrow;
		return nullptr;
	}
	return pArrow;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CArrow::Init(void)
{
	// �e�N���X�̏�����
	CObject3D::Init();
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CArrow::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CArrow::Update(void)
{
	// �e�N���X�̍X�V
	CObject3D::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CArrow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_idx;                                            // �I�u�W�F�N�gID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	CObject3D::Draw(); // �e�N���X�̕`��

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// �p�x��ς���
//--------------------------------
void CArrow::ChengeAngle(const D3DXVECTOR3& pos, D3DXVECTOR3* rot) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float length = D3DXVec3Length(&space);
	if (length < m_chengeLength)
	{
		*rot = GetRot();
	}
}

//--------------------------------
// ��󂪋߂�
//--------------------------------
bool CArrow::ReleaseHit(const D3DXVECTOR3& pos, float length) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float spaceLength = D3DXVec3Length(&space);
	return (spaceLength < (m_length + length));
}