//--------------------------------
//
// �i�r�Q�[�V�����I�u�W�F�N�g [naviobject.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviobject.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

size_t CNaviObject::m_nextIdx = 0; // ���̃C���f�b�N�X (Trigger�n���p)

//--------------------------------
// ����������
//--------------------------------
HRESULT CNaviObject::Init(void)
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
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNaviObject::Uninit(void)
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
void CNaviObject::Update(void)
{

}

//--------------------------------
// �`�揈��
//--------------------------------
void CNaviObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_biasIdx;                                        // �o�C�A�XID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxWorld, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// �A���O���̓K��
	const D3DXVECTOR3 up = VEC_UP;
	D3DXMatrixRotationAxis(&mtxWorld, &up, m_angle);

	// �����𔽓]
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);

	// �ʒu�𔽉f
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

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// �G�ꂽ��N������
//--------------------------------
CNavi::TYPE CNaviObject::ActivateTrigger(const D3DXVECTOR3& pos, float* pAngle, size_t* pIdx) const
{
	D3DXVECTOR3 space = pos - m_pos;
	float hight(space.y);
	space.y = 0.0f;
	float length = D3DXVec3Length(&space);
	if (length <= m_chengeLength && std::abs(hight) <= CHENGE_HEIGHT)
	{
		Activate(pAngle);
		if (pIdx != nullptr)
		{
			*pIdx = m_idx;
		}
		return m_type;
	}
	else
	{
		return CNavi::TYPE::None;
	}
}

//--------------------------------
// �G�ꂽ�����
//--------------------------------
bool CNaviObject::ReleaseTrigger(const D3DXVECTOR3& pos, float length) const
{
	D3DXVECTOR3 space = pos - m_pos;
	float hight(space.y);
	space.y = 0.0f;
	float spaseLength = D3DXVec3Length(&space);
	return (spaseLength < (m_length + length) && std::abs(hight) <= CHENGE_HEIGHT);
}