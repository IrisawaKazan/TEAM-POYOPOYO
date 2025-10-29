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
	m_type = ITEM_NOEN;							// �A�C�e���̎��
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
	m_fWidth = 0.0f;							// ����
	m_fDepth = 0.0f;							// ���s
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);	// �g�嗦
	m_nModelIdx = 0;							// �C���f�b�N�X
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
	// �I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CItem::Uninit(void)
{
	// �I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();

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

		// �E�ɐi�ގw���̏ꍇ
	case ITEM_RIGHT:


		break;

		// ��O�ɐi�ގw���̏ꍇ
	case ITEM_FRONT:


		break;

		// ���ɐi�ގw���̏ꍇ
	case ITEM_LEFT:


		break;

	}


}

//***************************************
// �`�揈��
//***************************************
void CItem::Draw(void)
{
	// �I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();

}

//***************************************
// ��������
//***************************************
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fDepth, const D3DXVECTOR3 scale,  const std::string FileName)
{
	// �A�C�e���̃|�C���^
	CItem* pItem = nullptr;

	// ����
	pItem = new CItem;

	if (pItem == nullptr)
	{// �|�C���^��nullptr��������
		return nullptr;
	}

	// �I�u�W�F�N�gX�̃Z�b�^�[�Ɉ����̒l����
	pItem->SetPosition(pos);	// �ʒu
	pItem->SetRotasion(rot);	// ����
	pItem->SetScale(scale);		// �g�嗦
	pItem->SetIdx(FileName);	// ���f���̃t�@�C����

	// �����̒l�����ꂼ��̃����o�ϐ��ɑ�� 
	pItem->m_type = type;					// �A�C�e���̎��
	pItem->m_pos = pos;						// �ʒu
	pItem->m_rot = rot;						// ����
	pItem->m_fWidth = fWidth;				// ����
	pItem->m_fDepth = fDepth;				// ���s
	pItem->m_scale = scale;					// �g�嗦
	pItem->m_nModelIdx = pItem->GetIndx();	// �C���f�b�N�X

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

	// X���ł̓����蔻��
	if (SubjectXR <= ItemXL || SubjectXL >= ItemXR)
	{// �������ĂȂ��ꍇ
		return false;
	}

	// Z���ł̓����蔻��
	if (SubjectZB <= ItemZF || SubjectZF >= ItemZB)
	{// �������ĂȂ��ꍇ
		return false;
	}

	// �������Ă���Ɣ��肷��
	return true;
}
