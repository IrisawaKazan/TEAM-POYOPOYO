//==============================================================
//
//	modelmanager.cpp
//	Author:chikada shouya
//
//==============================================================
#include "modelmanager.h"
#include "manager.h"
#include <assert.h>

// �ÓI�����o�ϐ�
std::unique_ptr<CModelManager> CModelManager::m_Instance = NULL;

//==============================================================
// �R���X�g���N�^
//==============================================================
CModelManager::CModelManager()
{
	// �x�N�^�[�̃N���A
	m_vModel.clear();

	// �����̏�����
	//m_nNumAll = NULL;
}

//==============================================================
// �f�X�g���N�^
//==============================================================
CModelManager::~CModelManager()
{

}

//==============================================================
// �ǂݍ���
//==============================================================
HRESULT CModelManager::Load(std::string sName)
{
	return S_OK;
}

//==============================================================
// ���f���̖@���Đݒ�
//==============================================================
void CModelManager::ReCalcNormalize(const int Indx)
{
	// �@���̃X���[�X��
	const float Epsilon = 1e-6f;
	std::vector<DWORD> adjacency(m_vModel[Indx].pMesh->GetNumFaces() * 3);
	m_vModel[Indx].pMesh->GenerateAdjacency(Epsilon, adjacency.data());

	HRESULT hr = D3DXComputeNormals(m_vModel[Indx].pMesh, adjacency.data());

	if (FAILED(hr))
	{
		// ���s������
		assert(0 && "���f���̃X���[�X���Ɏ��s���܂���");
	}
}

//==============================================================
// �j��
//==============================================================
void CModelManager::Unload(void)
{
	for (auto Models = m_vModel.begin(); Models != m_vModel.end(); Models++)	// ���R���e�i�ɃA�N�Z�X
	{
		// �e�N�X�`���̔j��
		if (Models->pTexture != nullptr)
		{
			delete[] Models->pTexture;

			Models->pTexture = nullptr;
		}

		// �o�b�t�@�̔j��
		if (Models->pBuffMat != nullptr)
		{
			//�I��
			Models->pBuffMat->Release();
			Models->pBuffMat = nullptr;
		}

		// ���b�V���̔j��
		if (Models->pMesh != nullptr)
		{
			//�I��
			Models->pMesh->Release();
			Models->pMesh = nullptr;
		}

		
	}

	// �x�N�^�[�̃N���A
	m_vModel.clear();
}

//==============================================================
// �ǉ�
//==============================================================
int CModelManager::Register(std::string sName)
{
	// �s���ȕ�����Ȃ�
	if (sName.size() <= 0) return -1;

	//�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < static_cast<int>(m_vModel.size()); nCnt++)
	{
		// ���O�̂��̂���������
		if (m_vModel[nCnt].sName.compare(sName) == 0)
		{
			return nCnt;
		}
	}

	// ���[�J���ϐ�
	ModelInfo Info = {};

	//X�t�@�C���̓ǂݍ���(�S�p�[�c)2
	HRESULT hr = D3DXLoadMeshFromX(sName.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&Info.pBuffMat,
		NULL,
		&Info.dwNumMat,
		&Info.pMesh);

	D3DXMATERIAL* pMat;	//�}�e���A���ւ̃|�C���^

	// �e�N�X�`���̕��̔z��̊m��
	Info.pTexture = new int[Info.dwNumMat];

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)Info.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)Info.dwNumMat; nCntMat++)
	{

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{//�e�N�X�`���t�@�C�������݂���

			// �e�N�X�`���|�C���^�擾
			CTextureManager* pTexture = CTextureManager::Instance();
			Info.pTexture[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
		}
		else
		{
			Info.pTexture[nCntMat] = -1;
		}
	}


	// �ǂݍ��݂Ɏ��s������
	if (FAILED(hr))
	{
		return -1;
	}

	// ���O�̕ۑ�
	Info.sName = sName;

	// ����O����ǉ����Ă���
	m_vModel.push_back(Info);

	// �C���f�b�N�X�ɍŌ�̏��̐���ǉ�
	int nIdx = m_vModel.size() - 1;
	return nIdx;
}

//==============================================================
// �A�h���X���擾
//==============================================================
CModelManager::ModelInfo CModelManager::GetAddress(int nIdx)
{
	if (static_cast<int>(m_vModel.size()) <= nIdx || nIdx < 0)return {};
	return m_vModel[nIdx];
}

//==============================================================
// �C���X�^���X�̐���
//==============================================================
CModelManager* CModelManager::Instance(void)
{
	if (m_Instance != NULL) return m_Instance.get();
	m_Instance.reset(new CModelManager);
	return m_Instance.get();
}

