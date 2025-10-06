//*************************************
//
//�@2D�|���S������[Object2D.cpp]
//�@Author:Hatori Kensaku
//
//*************************************

// �C���N���[�h
#include "Object2D.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"

//*********************************************
// �R���X�g���N�^
//*********************************************
CObject2D::CObject2D(int Priority) : CObject(Priority)
{
	// �����o�ϐ���NUL�ɂ���
	m_pVertex = NULL;
	m_fAngle = NULL;
	m_fLength = NULL;
	m_fWidth = NULL;
	m_fHeigth = NULL;
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_nTexIndx = NULL;
}

//*********************************************
// �f�X�g���N�^
//*********************************************
CObject2D::~CObject2D()
{

}

//*********************************************
// ����������
//*********************************************
HRESULT CObject2D::Init()
{
	// �f�o�C�X���擾
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// ���_�o�b�t�@�����b�N
	VERTEX_2D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = VEC3_NULL;
		pVtx[1].pos = VEC3_NULL;
		pVtx[2].pos = VEC3_NULL;
		pVtx[3].pos = VEC3_NULL;

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVertex->Unlock();

	return S_OK;
}

//*********************************************
// �I������
//*********************************************
void CObject2D::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// �������̉��
	Release();
}

//*********************************************
// �X�V����
//*********************************************
void CObject2D::Update()
{
	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		//���_���W�̍X�V
		pVtx[0].pos.x = m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.y = m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;
		pVtx[2].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;
		pVtx[3].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;
		pVtx[3].pos.z = 0.0f;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}

	m_pVertex->Unlock();
}

//*********************************************
// �|���S���̈ʒu��ݒ�
//*********************************************
void CObject2D::SetPosition(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;

	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		//���_���W�̍X�V
		pVtx[0].pos.x = m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[0].pos.y = m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[0].pos.z = 0.0f;//0.0f;
		pVtx[1].pos.x = m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[1].pos.y = m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;//0.0f;
		pVtx[2].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[2].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;//0.0f;
		pVtx[3].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[3].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[3].pos.z = 0.0f;
	}
}

//*********************************************
// �|���S���̌�����ݒ�
//*********************************************
void CObject2D::SetRotasion(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;

	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		//���_���W�̍X�V
		pVtx[0].pos.x = m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[0].pos.y = m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[0].pos.z = 0.0f;//0.0f;
		pVtx[1].pos.x = m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[1].pos.y = m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;//0.0f;
		pVtx[2].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[2].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;//0.0f;
		pVtx[3].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[3].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[3].pos.z = 0.0f;
	}
}

//*********************************************
// �|���S���̑傫����ݒ�
//*********************************************
void CObject2D::SetSize(const D3DXVECTOR2 Size)
{
	m_fWidth = Size.x;
	m_fHeigth = Size.y;

	// ����ύX���ă|���S���̒��S�_����̊p�x���Đݒ�
	m_fAngle = atan2f(Size.x, Size.y);
	// �傫�����Đݒ�
	m_fLength = sqrtf((Size.x * Size.x) + (Size.y * Size.y));
}

//*********************************************
// �|���S����UV���W��ݒ�
//*********************************************
void CObject2D::SetUv(const D3DXVECTOR2 Uv, const float SizeU, const float SizeV)
{
	// Uv���J�E���g��Size���e�N�X�`�����W�̑傫��

	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		pVtx[0].tex = D3DXVECTOR2(SizeU * Uv.x, SizeV * Uv.y);
		pVtx[1].tex = D3DXVECTOR2(SizeU + (Uv.x * SizeU), SizeV * Uv.y);
		pVtx[2].tex = D3DXVECTOR2(SizeU * Uv.x, SizeV + (Uv.y * SizeV));
		pVtx[3].tex = D3DXVECTOR2(SizeU + (Uv.x * SizeU), SizeV + (Uv.y * SizeV));
	}

	m_pVertex->Unlock();
}

//*********************************************
// �|���S���̐F��ݒ�
//*********************************************
void CObject2D::SetCol(const D3DXCOLOR Col)
{
	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		pVtx[0].col = Col;
		pVtx[1].col = Col;
		pVtx[2].col = Col;
		pVtx[3].col = Col;
	}

	m_pVertex->Unlock();
}

//*********************************************
// �`�揈��
//*********************************************
void CObject2D::Draw()
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	// �t�H�O��؂�
	pRenderer->offFog();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_2D));

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);
	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �t�H�O������
	pRenderer->onFog();
}

//*********************************************
// 2D�|���S�������
//*********************************************
CObject2D* CObject2D::Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot)
{
	CObject2D* pObject2D;
	pObject2D = new CObject2D(4);
	pObject2D->Init();
	pObject2D->SetPosition(Pos);
	pObject2D->SetRotasion(Rot);
	pObject2D->SetSize(D3DXVECTOR2(100.0f, 100.0f));
	pObject2D->SetCol(WHITE);
	return pObject2D;
}

//*********************************************
// 2D�|���S�������e�N�X�`���w��
//*********************************************
CObject2D* CObject2D::Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, const D3DXVECTOR2 Size)
{
	CObject2D* pObject2D;
	pObject2D = new CObject2D(7);
	pObject2D->Init();
	pObject2D->SetPosition(Pos);
	pObject2D->SetRotasion(Rot);
	pObject2D->SetSize(Size);
	pObject2D->SetCol(WHITE);
	return pObject2D;
}