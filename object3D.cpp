//*************************************
//
//�@3D�|���S������[Object3D.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "object3D.h"

//*********************************************
// �R���X�g���N�^
//*********************************************
CObject3D::CObject3D(int Priority) : CObject(Priority)
{
	m_pVertex = NULL;
	m_fWidth = NULL;
	m_fHeigth = NULL;
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_RotDest = VEC3_NULL;
	m_Col = WHITE;
	m_UVMax = { 1.0f,1.0f };
	m_UVMin = { 0.0f,0.0f };
	m_nTexIndx = -1;
}

//*********************************************
// �f�X�g���N�^
//*********************************************
CObject3D::~CObject3D()
{
}

//*********************************************
// ����������
//*********************************************
HRESULT CObject3D::Init(void)
{
	// �f�o�C�X���擾
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// ���_�o�b�t�@�����b�N
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth,0.0f,m_fHeigth);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeigth);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeigth);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeigth);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		pVtx[0].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f,1.0f,0.0f);

		pVtx[0].tex = D3DXVECTOR2(m_UVMin.x, m_UVMin.y);
		pVtx[1].tex = D3DXVECTOR2(m_UVMax.x, m_UVMin.y);
		pVtx[2].tex = D3DXVECTOR2(m_UVMin.x, m_UVMax.y);
		pVtx[3].tex = D3DXVECTOR2(m_UVMax.x, m_UVMax.y);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVertex->Unlock();

	return S_OK;
}

//*********************************************
// �I������
//*********************************************
void CObject3D::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// �������̊J��
	Release();
}

//*********************************************
// �X�V����
//*********************************************
void CObject3D::Update(void)
{
	// ���_�o�b�t�@�����b�N
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth, 0.0f, m_fHeigth);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth, 0.0f, m_fHeigth);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth, 0.0f, -m_fHeigth);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth, 0.0f, -m_fHeigth);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(m_UVMin.x, m_UVMin.y);
		pVtx[1].tex = D3DXVECTOR2(m_UVMax.x, m_UVMin.y);
		pVtx[2].tex = D3DXVECTOR2(m_UVMin.x, m_UVMax.y);
		pVtx[3].tex = D3DXVECTOR2(m_UVMax.x, m_UVMax.y);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVertex->Unlock();
}

//*********************************************
// �`�揈��
//*********************************************
void CObject3D::Draw(void)
{
	CTextureManager* pTexmanager = CTextureManager::Instance();
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽓]
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// ���_�o�b�t�@���f�o�C�X����f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexmanager->GetAddress(pTexmanager->Register("data\\TEXTURE\\MagicCircle.jpg")));
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//*********************************************
// ����
//*********************************************
CObject3D* CObject3D::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot)
{
	CObject3D* pObject3D = NULL;
	// �������m��
	pObject3D = new CObject3D;
	pObject3D->SetPosition(Pos);
	pObject3D->SetRotasion(Rot);
	pObject3D->SetSize(D3DXVECTOR2(1000.0f, 1000.0f));
	pObject3D->Init();
	return pObject3D;
}