//--------------------------------
//
// �i�r�Q�[�V�����I�u�W�F�N�g [naviobject.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviobject.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

size_t CNaviObject::m_nextIdx = 0; // ���̃C���f�b�N�X (Trigger�n���p)

//--------------------------------
// ����������
//--------------------------------
HRESULT CNaviObject::Init(void)
{
	// �e�N���X�̏�����
	CObject3D::Init();
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviObject::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNaviObject::Update(void)
{
	// �e�N���X�̍X�V
	CObject3D::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_biasIdx;                                        // �o�C�A�XID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	CObject3D::Draw(); // �e�N���X�̕`��

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// �G�ꂽ��N������
//--------------------------------
CNavi::Type CNaviObject::ActivateTrigger(const D3DXVECTOR3& pos, D3DXVECTOR3* rot, size_t* pIdx) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float length = D3DXVec3Length(&space);
	if (length < m_chengeLength)
	{
		Activate(rot);
		if (pIdx != nullptr)
		{
			*pIdx = m_idx;
		}
		return m_type;
	}
	else
	{
		return CNavi::Type::None;
	}
}

//--------------------------------
// �G�ꂽ�����
//--------------------------------
bool CNaviObject::ReleaseTrigger(const D3DXVECTOR3& pos, float length) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float spaceLength = D3DXVec3Length(&space);
	return (spaceLength < (m_length + length));
}