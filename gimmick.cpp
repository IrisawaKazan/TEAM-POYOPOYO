//=================================================
//
//	block.cpp
// 
// Author:chikada shouya
//
//=================================================

//***************************************
// �C���N���[�h�t�@�C���錾
//***************************************
#include "gimmick.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"

//***************************************
// �R���X�g���N�^
//***************************************
CGimmick::CGimmick(int nPriority) :CBlock(nPriority)
{
	m_type = TYPE_NONE;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_VtxMax = { -10000.0f,-10000.0f,-10000.0f };
	m_VtxMin = { 10000.0f,10000.0f,10000.0f };
	m_size = VEC3_NULL;
	m_RBOffset = VEC3_NULL;
	sNamePath = {};
}

//***************************************
// �f�X�g���N�^
//***************************************
CGimmick::~CGimmick()
{

}

//***************************************
// �u���b�N�̐���
//***************************************
CGimmick* CGimmick::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CGimmick* pGimmick = nullptr;
	pGimmick = new CGimmick;

	if (pGimmick != nullptr)
	{
		// �I�u�W�F�N�g�ݒ�
		pGimmick->SetFilepath(sName);
		pGimmick->SetPosition(pos);
		pGimmick->SetRotasion(rot);
		pGimmick->SetScale(Scale);
		pGimmick->Init();
		return pGimmick;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
//�@�u���b�N����������
//***************************************
HRESULT CGimmick::Init(void)
{
	// ������
	CBlock::Init();
	
	return S_OK;
}

//***************************************
//�@�u���b�N�I������
//***************************************
void CGimmick::Uninit(void)
{
	// �j��
	CBlock::Uninit();
}

//***************************************
//�@�u���b�N�X�V����
//***************************************
void CGimmick::Update(void)
{
	// �X�V
	CBlock::Update();

	//// �i�r�Ƀ��C�L���X�g�I�u�W�F�N�g��o�^ sato ��
	//CModelManager* pModelTexManager = CModelManager::Instance();
	//CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));
	//CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, GetWorldMtx());
}

//***************************************
//�@�u���b�N�`�揈��
//***************************************
void CGimmick::Draw(void)
{
	// �`��
	CBlock::Draw();
}