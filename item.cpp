//****************************************************************
//
// �A�C�e���̏���
// Author:Takahashi Misaki
//
//****************************************************************

// �C���N���[�h
#include "item.h"

//***************************************
// �R���X�g���N�^
//***************************************
CItem::CItem()
{
	m_type = ITEM_NOEN;						// �A�C�e���̎��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ����
	m_fWidth = 0.0f;						// ����
	m_fDepth = 0.0f;						// ���s
}

//***************************************
// �f�X�g���N�^
//***************************************
CItem::~CItem()
{
}

//***************************************
// ����������
//***************************************
HRESULT CItem::Init(void)
{
	// �I�u�W�F�N�g3D�̏���������
	CObject3D::Init();

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CItem::Uninit(void)
{
	// �I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();

}

//***************************************
// �X�V����
//***************************************
void CItem::Update(void)
{
	switch (m_type)
	{
		// ���ɐi�ގw���̏ꍇ
	case ITEM_BACK:


		break;

		// �O�i����w���̏ꍇ
	case ITEM_FORWARD:


		break;

		// ��O�ɐi�ގw���̏ꍇ
	case ITEM_FRONT:


		break;
	}

	// �I�u�W�F�N�g3D�̍X�V����
	CObject3D::Update();

}

//***************************************
// �`�揈��
//***************************************
void CItem::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();

}

//***************************************
// �����蔻�菈��
//***************************************
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const float fWidth, const float fDepth)
{
	// �A�C�e���̃|�C���^
	CItem* pItem;

	// �����̒l�����ꂼ��̕ϐ��ɑ�� 
	pItem->m_type = type;		// �A�C�e���̎��
	pItem->m_pos = pos;			// �ʒu
	pItem->m_fWidth = fWidth;	// ����
	pItem->m_fDepth = fDepth;	// ���s

	// ����������
	pItem->Init();

	// �|�C���^��Ԃ�
	return pItem;
}

//***************************************
// �����蔻�菈��
//***************************************
bool CItem::CollisionItem(const D3DXVECTOR3 pos, const float fWidth, const float fDepth)
{
	// �Ώۂ̓����蔻��
	float SubjectXL = pos.x - fWidth;	// X���ł̍����̓����蔻��
	float SubjectXR = pos.x + fWidth;	// X���ł̉E���̓����蔻��
	float SubjectZF = pos.z - fDepth;	// Z���ł̎�O���̓����蔻��
	float SubjectZB = pos.z + fDepth;	// Z���ł̉����̓����蔻��

	// �A�C�e���̓����蔻��
	float ItemXL = m_pos.x - m_fWidth;	// X���ł̍����̓����蔻��
	float ItemXR = m_pos.x + m_fWidth;	// X���ł̉E���̓����蔻��
	float ItemZF = m_pos.z - m_fDepth;	// Z���ł̎�O���̓����蔻��
	float ItemZB = m_pos.z + m_fDepth;	// Z���ł̉����̓����蔻��

	// �����������ǂ����̔���
	bool bCollX = false;	// X���ł̔���
	bool bCollZ = false;	// Z���ł̔���

	// X���ł̓����蔻��
	if (SubjectXR >= ItemXL || SubjectXL <= ItemXR)
	{// ���������ꍇ
		// X���œ������Ă���Ƃ���
		bCollX = true;
	}
	else
	{
		return false;
	}

	// Z���ł̓����蔻��
	if (SubjectZB >= ItemZF || SubjectZF <= ItemZB)
	{// ���������ꍇ
		// Z���œ������Ă���Ƃ���
		bCollZ = true;
	}
	else
	{
		return false;
	}

	//  X����Z���œ������Ă���ꍇ
	if (bCollX == true && bCollZ == true)
	{
		// �������Ă���Ɣ��肷��
		return true;
	}
}
