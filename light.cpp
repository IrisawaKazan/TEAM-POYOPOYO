//****************************************************************
//
// ���C�g�̏���[ligt.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "light.h"
#include "manager.h"

const D3DXVECTOR3 CLight::Config::DefoultDir = { 0.2f,-0.8f,-0.4f };
const D3DXCOLOR CLight::Config::Evening = { 0.7f,0.5f,0.3f ,1.0f };

//***************************************
// �R���X�g���N�^
//***************************************
CLight::CLight()
{
	//���C�g���N���A����
	ZeroMemory(&m_Light, sizeof(m_Light));
}

//***************************************
// �f�X�g���N�^
//***************************************
CLight::~CLight()
{
}

//***************************************
// ����������
//***************************************
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	m_nCntLight = 0;

	D3DXVECTOR3 vecDir[Config::NumLIght];

	for (int nCnt = 0; nCnt < Config::NumLIght; nCnt++)
	{
		// ���C�g�̎�ނ�ݒ�
		// ���s����
		m_Light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		// ���C�g�̐F
		m_Light[nCnt].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		// ���C�g�̕�����ݒ�
		vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		// ���C�g�̕�����ݒ�
		vecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, 0.8f);
		// ���C�g�̕�����ݒ�
		vecDir[2] = D3DXVECTOR3(-1, -0.57f, -0.3f);

		//���K������i�傫���P�̃x�N�g���ɂ���j
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_Light[nCnt].Direction = vecDir[nCnt];

		//���C�g��ݒ�
		pDevice->SetLight(nCnt, &m_Light[nCnt]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCnt, TRUE);
	}

	// �J�E���^���C���N�������g
	m_nCntLight = Config::NumLIght - 1;

	return S_OK;
}

//***************************************
// �I������
//***************************************
void CLight::Uninit(void)
{
}

//***************************************
// �X�V����
//***************************************
void CLight::Update(void)
{

}

//***************************************
// ���C�g�̐ݒ菈��
//***************************************
int CLight::SetLight(D3DLIGHT9 Setter)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	// ���C�g�����Z
	m_nCntLight++;

	//���C�g��ݒ�
	pDevice->SetLight(m_nCntLight, &Setter);

	//���C�g��L���ɂ���
	pDevice->LightEnable(m_nCntLight, TRUE);

	// �C���f�b�N�X��Ԃ�
	return m_nCntLight;
}

//***************************************
// ���C�g�̍폜����
//***************************************
void CLight::DeleteLight(const int Indx)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	//���C�g��L���ɂ���
	pDevice->LightEnable(Indx, FALSE);

	// ���������炷
	m_nCntLight--;
}

//***************************************
// ���s�����̐F�����Z�b�g
//***************************************
void CLight::ResetCol(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir[Config::NumLIght];

	for (int nCnt = 0; nCnt < Config::NumLIght; nCnt++)
	{
		// ���C�g�̎�ނ�ݒ�
		// ���s����
		m_Light[nCnt].Type = D3DLIGHT_DIRECTIONAL;

		// ���C�g�̊g�U����ݒ�
		// ���C�g�̐F
		m_Light[nCnt].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���C�g�̕�����ݒ�
		vecDir[0] = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
		// ���C�g�̕�����ݒ�
		vecDir[1] = D3DXVECTOR3(-0.2f, 0.8f, 0.8f);
		// ���C�g�̕�����ݒ�
		vecDir[2] = D3DXVECTOR3(-1, -0.57f, -0.3f);

		//���K������i�傫���P�̃x�N�g���ɂ���j
		D3DXVec3Normalize(&vecDir[nCnt], &vecDir[nCnt]);
		m_Light[nCnt].Direction = vecDir[nCnt];

		//���C�g��ݒ�
		pDevice->SetLight(nCnt, &m_Light[nCnt]);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCnt, TRUE);
	}
}