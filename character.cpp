//************************************************************
//
//�@�K�w�\��������L�����N�^�[�̏���[modelcharactor.cpp]
//�@Author:Hatori Kensaku
//
//************************************************************
// �C���N���[�h
#include "character.h"
#include "manager.h"
//***************************************
// �R���X�g���N�^
//***************************************
CModelCharacter::CModelCharacter(int Priority) : CObject(Priority)
{

}
//***************************************
// �R���X�g���N�^
//***************************************
CModelCharacter::~CModelCharacter()
{

}
//***************************************
// ����������
//***************************************
HRESULT CModelCharacter::Init(const char* MotionFilePath, float Speed)
{
	// ����������
	m_fSpeed = Speed;
	m_Enable = true;

	// ���[�V�����𐶐�
	m_MotionInfo = CMotion::LoadMotionFile(MotionFilePath, m_apModel);
	m_MotionInfo->SetMotion(1, false);
	return S_OK;
}
//***************************************
// �I������
//***************************************
void CModelCharacter::Uninit(void)
{
	if (m_MotionInfo != NULL)
	{
		m_MotionInfo->Uninit();
		m_MotionInfo = NULL;
	}
	for (auto itr = m_apModel.begin(); itr != m_apModel.end(); itr++)
	{
		if ((*itr) != NULL)
		{
			(*itr)->Uninit();
		}
	}
	m_apModel.clear();

	// �������g�̔j��
	Release();
}
//***************************************
// �X�V����
//***************************************
void CModelCharacter::Update(void)
{
	// �ړ��ʂ�0�ɋ߂Â���
	m_Move.x += (0.0f - m_Move.x) * 0.1f;
	//m_Move.y -= 0.5f;
	m_Move.z += (0.0f - m_Move.z) * 0.1f;
	// �̂̈ʒu��ۑ�
	m_OldPos = m_Pos;
	// ������
	m_Pos.x += m_Move.x;
	m_Pos.y += m_Move.y;
	m_Pos.z += m_Move.z;

	// �p�x�̐��K��
	if (m_Rot.y > D3DX_PI)
	{
		m_Rot.y = -D3DX_PI + (m_Rot.y - D3DX_PI);
	}
	else if (m_Rot.y < -D3DX_PI)
	{
		m_Rot.y = D3DX_PI + (m_Rot.y + D3DX_PI);
	}
	if (m_RotDest.y > D3DX_PI)
	{
		m_RotDest.y = -D3DX_PI + (m_RotDest.y - D3DX_PI);
	}
	else if (m_RotDest.y < -D3DX_PI)
	{
		m_RotDest.y = D3DX_PI + (m_RotDest.y + D3DX_PI);
	}
	// �p�x�̋ߓ�
	if (m_RotDest.y - m_Rot.y >= D3DX_PI)
	{
		m_Rot.y += D3DX_PI * 2.0f;
	}
	else if (m_RotDest.y - m_Rot.y <= -D3DX_PI)
	{
		m_Rot.y -= D3DX_PI * 2.0f;
	}
	// ���̊p�x�����X�ɋ߂Â���
	m_Rot.y += (m_RotDest.y - m_Rot.y) * 0.2f;
	// ���[�V�����̍X�V
	m_MotionInfo->Update(m_apModel);
}
//***************************************
// �`�揈��
//***************************************
void CModelCharacter::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;				// �v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						// ���݂̃}�e���A���̕ۑ��p
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	D3DXMATRIX mtxParent;	// �v�Z�p�}�g���b�N�X
	// ���݂̃}�e���A���̎擾
	pDevice->GetMaterial(&matDef);
	// �L���������烂�f����`�悷��
	if (m_Enable == true)
	{
		for (auto itr = m_apModel.begin(); itr != m_apModel.end(); itr++)
		{
			if ((*itr) != NULL)
			{
				(*itr)->Draw();
			}
		}
	}
	pDevice->SetMaterial(&matDef);
}
