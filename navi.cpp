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
#include "arrow.h"

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

	m_direction = ARROW_DIRECTION::Left;
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

	if (CManager::GetInputMouse()->GetMouseState().lZ > 0.0f)
	{// �z�C�[���A�b�v�Ŗ��̌�����ύX
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + static_cast<unsigned char>(ARROW_DIRECTION::Max) - 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}
	else if (CManager::GetInputMouse()->GetMouseState().lZ < 0.0f)
	{// �z�C�[���_�E���Ŗ��̌�����ύX
		m_direction = static_cast<ARROW_DIRECTION>((static_cast<unsigned char>(m_direction) + 1) % static_cast<unsigned char>(ARROW_DIRECTION::Max));
	}

	if (CManager::GetInputMouse()->OnDown(0))
	{// ���N���b�N�����Ƃ�
		m_clickPos = GetPos(); // �N���b�N�����ʒu��ۑ�

		// ���̊p�x������
		float arrowAngle = 0.0f;
		switch (m_direction)
		{
		case CNavi::ARROW_DIRECTION::Left:
			arrowAngle = D3DXToRadian(-90.0f);
			break;
		case CNavi::ARROW_DIRECTION::Front:
			arrowAngle = D3DXToRadian(0.0f);
			break;
		case CNavi::ARROW_DIRECTION::Back:
			arrowAngle = D3DXToRadian(180.0f);
			break;
		}

		// �����쐬
		m_apArrow.push_back(CArrow::Create(m_clickPos, D3DXVECTOR3(0.0f, arrowAngle, 0.0f), "data/TEXTURE/UI/ArrowMark001.png", { GetWidth(),GetVetical() }, m_apArrow.size()));
	}
}

//--------------------------------
// �`�揈��
//--------------------------------
void CNavi::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g��L��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias �ݒ� sato
	float depthBias = -0.000001f;                                  //Z�o�b�t�@���J���������ɃI�t�Z�b�g����l
	depthBias *= m_apArrow.size();                                 // �I�u�W�F�N�gID�������I�t�Z�b�g
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Z�o�C�A�X�ݒ�

	CObject3D::Draw(); // �e�N���X�̕`��

	// Depth Bias �ݒ������ sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// �A���t�@�e�X�g�𖳌��ɖ߂�
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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