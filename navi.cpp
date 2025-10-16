//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//--------------------------------
//
// �i�r�Q�[�V�����}�[�J�[�̃N���X
// 
//--------------------------------

//--------------------------------
// ����
//--------------------------------
CNavi* CNavi::Create(const char* filePath, D3DXVECTOR2 size)
{
	// �C���X�^���X�̐���
	CNavi* pNavi = new CNavi;
	if (pNavi == nullptr)
	{
		return nullptr;
	}

	pNavi->SetFilePath(filePath);
	pNavi->SetSize(size);

	// ������
	if (FAILED(pNavi->Init()))
	{
		delete pNavi;
		return nullptr;
	}
	return pNavi;
}

//--------------------------------
// ����������
//--------------------------------
HRESULT CNavi::Init(void)
{
	// �e�N���X�̏�����
	CObject3D::Init();
	
	// ���C���쐬
	CreateRay(CManager::GetInputMouse()->GetPos());

	// �����ʒu��ݒ�
	SetPosition(PlaneIntersect(HEIGHT));
	return S_OK;
}

//--------------------------------
// �I������
//--------------------------------
void CNavi::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// �X�V����
//--------------------------------
void CNavi::Update(void)
{
	// ���C���쐬
	CreateRay(CManager::GetInputMouse()->GetPos());

	// �ʒu���X�V
	SetPosition(PlaneIntersect(HEIGHT));

	// �e�N���X�̍X�V
	CObject3D::Update();
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNavi::Draw(void)
{
	CObject3D::Draw();
}

//--------------------------------
// ���C���쐬
//--------------------------------
void CNavi::CreateRay(D3DXVECTOR2 mousePos)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �r���[�|�[�g�̎擾
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	// �ˉe�s��̎擾
	D3DXMATRIX mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// �r���[�}�g���b�N�X�̎擾
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// �n�_�ƏI�_
	D3DXVECTOR3 vNear, vFar;

	// �j�A�N���b�v�ʏ��3D���W���v�Z
	D3DXVECTOR3 screenNear(mousePos.x, mousePos.y, 0.0f);
	D3DXVec3Unproject(
		&vNear,
		&screenNear,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// �t�@�[�N���b�v�ʏ��3D���W���v�Z
	D3DXVECTOR3 screenFar(mousePos.x, mousePos.y, 1.0f);
	D3DXVec3Unproject(
		&vFar,
		&screenFar,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// ���C�̎n�_�ƕ���������
	m_RayPos = vNear;                        // ���C�̎n�_�̓j�A�N���b�v�ʏ�̓_
	m_RayDir = vFar - vNear;                 // �j�A����t�@�[�֌������x�N�g��
	D3DXVec3Normalize(&m_RayDir, &m_RayDir); // �����x�N�g���Ȃ̂Ő��K������
}

//--------------------------------
// ���ʂƂ̌�_�����߂�
//--------------------------------
D3DXVECTOR3 CNavi::PlaneIntersect(float fHeight)
{
	// y=fHeight �̕���
	D3DXPLANE plane(0.0f, 1.0f, 0.0f, -fHeight);

	// ��_
	D3DXVECTOR3 intersectionPoint;

	// ray�̏I�_
	D3DXVECTOR3 rayEnd = m_RayPos + m_RayDir * 10000.0f;

	// ���C�ƕ��ʂ̌�_���v�Z
	D3DXPlaneIntersectLine(
		&intersectionPoint, // ���ʂ̌�_
		&plane,             // ����
		&m_RayPos,          // ���C�̎n�_
		&rayEnd             // ���C�̏I�_
	);

	if (m_RayDir.y < 0)
	{// ���C���������̂Ƃ��̂݌�_��L���Ƃ���
		return intersectionPoint;
	}

	// ��_�������ȂƂ��͔��ɒႢ�ʒu��Ԃ�
	return D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
}