//*************************************
//
//�@�r���{�[�h����[ObjectBillBoard.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "objectbillboard.h"

//***************************************
// �R���X�g���N�^
//***************************************
CObjectBillBoard::CObjectBillBoard(const int Priority) : CObject(Priority)
{
	m_pVertex = NULL;
	m_nTexIndx = NULL;
	m_Size = VEC2_NULL;
	m_Pos = VEC3_NULL;
}

//***************************************
// �f�X�g���N�^
//***************************************
CObjectBillBoard::~CObjectBillBoard()
{
}

//***************************************
// ����������
//***************************************
HRESULT CObjectBillBoard::Init(void)
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
		pVtx[0].pos = D3DXVECTOR3(-m_Size.x, m_Size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Size.x, m_Size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_Size.x, -m_Size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Size.x, -m_Size.y, 0.0f);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// �e���_�̖@���̐ݒ�i��ȏ�j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVertex->Unlock();

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CObjectBillBoard::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	Release();
}

//***************************************
// �X�V����
//***************************************
void CObjectBillBoard::Update(void)
{
	VERTEX_3D* pVtx = NULL;

	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_Size.x, m_Size.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_Size.x, m_Size.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_Size.x, -m_Size.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_Size.x, -m_Size.y, 0.0f);

		pVtx[0].col = m_Col;
		pVtx[1].col = m_Col;
		pVtx[2].col = m_Col;
		pVtx[3].col = m_Col;

		// �e���_�̖@���̐ݒ�i��ȏ�j
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVertex->Unlock();
}

//***************************************
// �`�揈��
//***************************************
void CObjectBillBoard::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;

	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	// �r���[�}�g���b�N�X�擾
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �J�����̋t�s���ݒ�
	m_mtxWorld._11 = mtxView._11;
	m_mtxWorld._12 = mtxView._21;
	m_mtxWorld._13 = mtxView._31;
	m_mtxWorld._21 = mtxView._12;
	m_mtxWorld._22 = mtxView._22;
	m_mtxWorld._23 = mtxView._32;
	m_mtxWorld._31 = mtxView._13;
	m_mtxWorld._32 = mtxView._23;
	m_mtxWorld._33 = mtxView._33;

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// ���_�o�b�t�@���f�o�C�X����f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//***************************************
// ����
//***************************************
CObjectBillBoard* CObjectBillBoard::Create(D3DXVECTOR3 Pos,D3DXVECTOR2 Size)
{
	CObjectBillBoard* pObjectBillBoard = NULL;
	// �������m��
	pObjectBillBoard = new CObjectBillBoard;
	pObjectBillBoard->SetPosition(Pos);
	pObjectBillBoard->SetSize(Size);
	pObjectBillBoard->Init();
	return pObjectBillBoard;
}