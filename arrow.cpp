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
CArrow* CArrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filePath, D3DXVECTOR2 size)
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
	pArrow->SetChengeSize(size.x);

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
	CObject3D::Draw();
}

//--------------------------------
// �p�x��ς���
//--------------------------------
void CArrow::ChengeAngle(D3DXVECTOR3* pos, D3DXVECTOR3* rot) const
{
	D3DXVECTOR3 space = *pos - GetPos();
	float length = D3DXVec3Length(&space);
	if (length < m_chengeSize)
	{
		*rot = GetRot();
	}
}