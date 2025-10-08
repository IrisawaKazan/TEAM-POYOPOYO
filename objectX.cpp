//*************************************
//
//�@���f������[ObjectX.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "objectX.h"
#include "manager.h"

//***************************************
// �R���X�g���N�^
//***************************************
CObjectX::CObjectX(const int Priority) : CObject(Priority)
{
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_Scale = VEC3_NULL;
	m_pMtxParent = NULL;
	m_fAlpha = 1.0f;
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxRot);
}

//***************************************
// �f�X�g���N�^
//***************************************
CObjectX::~CObjectX()
{
}

//***************************************
// ����������
//***************************************
HRESULT CObjectX::Init(void)
{
	m_Scale = { 1.0f,1.0f,1.0f };
	return S_OK;
}

//***************************************
// �I������
//***************************************
void CObjectX::Uninit(void)
{
	Release();
}

//***************************************
// �X�V����
//***************************************
void CObjectX::Update(void)
{
}

//***************************************
// �`�揈��
//***************************************
void CObjectX::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTextureManager* pTexManager = CTextureManager::Instance();
	CModelManager* pModelTexManager = CModelManager::Instance();

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;	// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					// ���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL* pMat;						// �}�e���A���ւ̃|�C���^

	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(m_FilePath));

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// �g�嗦�𔽉f
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A���f�[�^�ւ̃|�C���^
	pMat = (D3DXMATERIAL*)modelinfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL pCol = pMat[nCntMat];
		pCol.MatD3D.Diffuse.a = m_fAlpha;
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pCol.MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, pTexManager->GetAddress(modelinfo.pTexture[nCntMat]));

		// ���f��(�p�[�c)�̕`��
		modelinfo.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&matDef);
}

//***************************************
// ����
//***************************************
CObjectX* CObjectX::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	CObjectX* pObjectX = NULL;
	// �������m��
	pObjectX = new CObjectX;

	if (pObjectX != nullptr)
	{
		pObjectX->SetPosition(Pos);
		pObjectX->SetRotasion(Rot);
		pObjectX->m_FilePath = Path;
		pObjectX->Init();
		return pObjectX;
	}
	else
	{
		return nullptr;
	}
}