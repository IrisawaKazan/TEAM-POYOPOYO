//****************************************************************
//
// �A�C�e���̏���
// Author:Takahashi Misaki
//
//****************************************************************

// �C���N���[�h
#include "item.h"
#include "modelmanager.h"

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

	// ���f���}�l�[�W���[�̃|�C���^
	CModelManager* pModelManager;

	// �C���X�^���X����
	pModelManager = CModelManager::Instance();

	// ���f���̏ڍ׏��
	CModelManager::ModelInfo ModelInfo = pModelManager->GetAddress(m_nModelIdx);

	// ���b�V�����
	LPD3DXMESH pMesh = ModelInfo.pMesh;

	// ���[�J���ϐ�
	int nNumVtx;				// ���_�̑���
	DWORD sizeFVF;				// ���_�̃T�C�Y
	BYTE* pVtxBuff;				// ���_�̃|�C���^
	D3DXVECTOR3 max, min;		// ���_�̍ő�l�ƍŏ��l
	D3DXVECTOR3 Vtx;			// ���_���W

	// �ő�l�ƍŏ��l�̏�����
	max = VEC3_NULL;
	min = VEC3_NULL;

	// ���_�̍ő吔���擾
	nNumVtx = pMesh->GetNumVertices();

	// ���_�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_�̐�������
	for (int nCount = 0; nCount < nNumVtx; nCount++)
	{
		// ���_���W��������
		Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ***  ���_���W�iX���j�̔�r  ***

		if (Vtx.x > max.x)
		{// �ő�l���傫���ꍇ
			// �ő�l�Ɍ��݂̒l��������
			max.x = Vtx.x;
		}

		if (Vtx.x < min.x)
		{// �ő�l��菬�����ꍇ
			// �ŏ��l�Ɍ��݂̒l��������
			min.x = Vtx.x;
		}


		// ***  ���_���W�iZ���j�̔�r  ***

		// �ő�l���傫���ꍇ
		if (Vtx.z > max.z)
		{
			// �ő�l�Ɍ��݂̒l��������
			max.z = Vtx.z;
		}

		if (Vtx.z < min.z)
		{// �ő�l��菬�����ꍇ
			// �ŏ��l�Ɍ��݂̒l��������
			min.z = Vtx.z;
		}

		// ���_�̃T�C�Y���i�߂�
		pVtxBuff += sizeFVF;
	}

	// X��Z�̍ő�l����
	m_fWidth = max.x;	// �����ɑ��
	m_fDepth = max.z;	// ���s�ɑ��

	// ���_�o�b�t�@���A�����b�N
	pMesh->UnlockVertexBuffer();

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
	// ���ɏ����Â�]
	m_rot.z += 0.05f;

	// �X�V���������������ɑ��
	SetRotasion(m_rot);

	switch (m_type)
	{
		// ���ɐi�ގw���̏ꍇ
	case ITEM_LEFT:


		break;

		// �W�����v����w���̏ꍇ
	case ITEM_JUMP:


		break;

		// �ǂ�o��w���̏ꍇ
	case ITEM_CLIMB:


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
CItem* CItem::Create(const ITEM type, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 scale,  const std::string FileName)
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
