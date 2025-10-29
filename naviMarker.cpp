//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [naviMarker.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviMarker.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNaviMarker* CNaviMarker::Create(const char* filePath, D3DXVECTOR2 size)
{
	// �C���X�^���X�̐���
	CNaviMarker* pNaviMarker = new CNaviMarker;
	if (pNaviMarker == nullptr)
	{
		return nullptr;
	}

	pNaviMarker->SetTexIdx(CTextureManager::Instance()->Register(filePath));
	pNaviMarker->SetSize(size);

	// ������
	if (FAILED(pNaviMarker->Init()))
	{
		delete pNaviMarker;
		return nullptr;
	}
	return pNaviMarker;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CNaviMarker::Init(void)
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
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.y);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.y);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.y);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.y);

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVertex->Unlock();

	m_pos = { 0,0,0 };
	D3DXMatrixIdentity(&m_mtxRot);
	m_nBiasID = 0;
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviMarker::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// �������̊J��
	Release();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNaviMarker::Update(void)
{

}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviMarker::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_nBiasID;                                        // �o�C�A�XID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld, mtxTrans;

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	//�����𔽓]
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// ���_�o�b�t�@���f�o�C�X����f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, CTextureManager::Instance()->GetAddress(m_texIdx));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Depth Bias �ݒ������
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}