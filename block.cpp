//=================================================
//
//	block.cpp
// 
// Author:�ߓc ����
//
//=================================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "block.h"
#include "manager.h"
#include "game.h"
#include "model.h"

// �R���X�g���N�^
CBlock::CBlock(int nPriority):CObjectX(nPriority)
{
	m_type = TYPE_0;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_VtxMax = VEC3_NULL;
	m_VtxMin = VEC3_NULL;
	m_size = VEC3_NULL;
	sNamePath = {};
}

// �f�X�g���N�^
CBlock::~CBlock()
{

}

//=========================
// �u���b�N�̐���
//=========================
CBlock* CBlock::Create(std::string sName,D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBlock* pBlock = nullptr;
	pBlock = new CBlock;

	if (pBlock != nullptr)
	{
		// �I�u�W�F�N�g�ݒ�
		pBlock->SetFilePath(sName);
		pBlock->sNamePath = sName;
		pBlock->SetPosition(pos);
		pBlock->SetRotasion(D3DXToRadian(rot));
		pBlock->Init();
		return pBlock;
	}
	else
	{
		return nullptr;
	}
}

//=========================
//�@�u���b�N����������
//=========================
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));

	LPD3DXMESH pMesh = modelinfo.pMesh;

	// ���[�J���ϐ��錾-----------------------------
	int nNumVtx;		// ���_��
	DWORD sizeFVF;		// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;		// ���_�o�b�t�@�̃|�C���^
	//----------------------------------------------

	// ���_���̎擾
	nNumVtx = pMesh->GetNumVertices();

	// ���_�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	// ���_�o�b�t�@�����b�N
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// ���_������
	for (int nCntBlock = 0; nCntBlock < nNumVtx; nCntBlock++)
	{
		// ���_���W�̑��
		D3DXVECTOR3 Vtx = *(D3DXVECTOR3*)pVtxBuff;

		// ���_���W�̔�r
		if (Vtx.x > m_VtxMax.x)
		{// x���ő�l���傫��������
			// ���
			m_VtxMax.x = Vtx.x;
		}
		if (Vtx.y > m_VtxMax.y)
		{// y���ő�l���傫��������
			// ���
			m_VtxMax.y = Vtx.y;
		}
		if (Vtx.z > m_VtxMax.z)
		{// z���ő�l���傫��������
			// ���
			m_VtxMax.z = Vtx.z;
		}

		if (Vtx.x < m_VtxMin.x)
		{// x���ŏ��l��菬����������
			// ���
			m_VtxMin.x = Vtx.x;
		}
		if (Vtx.y < m_VtxMin.y)
		{// y���ŏ��l��菬����������
			// ���
			m_VtxMin.y = Vtx.y;
		}
		if (Vtx.z < m_VtxMin.z)
		{// z���ŏ��l��菬����������
			// ���
			m_VtxMin.z = Vtx.z;
		}

		// ���_�t�H�[�}�b�g�̃T�C�Y���i�߂�
		pVtxBuff += sizeFVF;
	}

	// �T�C�Y��������
	m_size.x = m_VtxMax.x - m_VtxMin.x;	// size��x
	m_size.y = m_VtxMax.y - m_VtxMin.y;	// size��y
	m_size.z = m_VtxMax.z - m_VtxMin.z;	// size��z

	// �A�����b�N
	pMesh->UnlockVertexBuffer();

	return S_OK;
}
//=========================
//�@�u���b�N�I������
//=========================
void CBlock::Uninit(void)
{
	// �j��
	CObjectX::Uninit();
}
//=========================
//�@�u���b�N�X�V����
//=========================
void CBlock::Update(void)
{
	// �X�V
	CObjectX::Update();
}
//=========================
//�@�u���b�N�`�揈��
//=========================
void CBlock::Draw(void)
{
	// �`��
	CObjectX::Draw();
}