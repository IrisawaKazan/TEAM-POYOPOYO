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