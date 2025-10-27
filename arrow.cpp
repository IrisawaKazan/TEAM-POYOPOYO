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

	pArrow->SetType(CNavi::Type::Arrow);
	pArrow->SetFilePath(filePath);
	pArrow->SetSize(size);
	pArrow->SetPosition(pos);
	pArrow->SetRotasion(rot);
	pArrow->SetLength(D3DXVec2Length(&size));
	pArrow->SetChengeLength(D3DXVec2Length(&size) * CHENGE_LENGTH_MAGNIFICATION);
	pArrow->SetBiasIdx(idx);

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
	CNaviObject::Init();
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CArrow::Uninit(void)
{
	CNaviObject::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CArrow::Update(void)
{
	// �e�N���X�̍X�V
	CNaviObject::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CArrow::Draw(void)
{
	CNaviObject::Draw(); // �e�N���X�̕`��
}

//--------------------------------
// �i�r�Q�[�V�����A�N�V����
//--------------------------------
void CArrow::Activate(D3DXVECTOR3* rot) const
{
	if (rot != nullptr)
	{// null�łȂ����
		*rot = GetRot(); // �p�x��n��
	}
}