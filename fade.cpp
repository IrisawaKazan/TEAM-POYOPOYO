//****************************************************************
//
// �t�F�[�h�̏���[fade.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// �C���N���[�h
#include "fade.h"
#include "manager.h"
#include "math_T.h"

// �K��l��ݒ�
const D3DXCOLOR CFade::Config::DefoultCol = { 0.0f,0.0f,0.0f,1.0f };
// �V���O���g����錾
CFade* CFade::m_pSingleto = NULL;

//************************************
// ����������
//************************************
void CFade::Init(void)
{
	m_pSingleto->m_Fade = FADE_NONE;
	m_pSingleto->m_colorFade = Config::DefoultCol;

	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pSingleto->m_pVertex,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pSingleto->m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�̐ݒ�
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;
	}

	//���_�o�b�t�@���A�����b�N
	m_pSingleto->m_pVertex->Unlock();
}

//************************************
// �I������
//************************************
void CFade::Uninit(void)
{
	// �V���O���g������������Ă�����
	if (m_pSingleto != NULL)
	{
		// ���_�o�b�t�@����������Ă�����
		if (m_pSingleto->m_pVertex != NULL)
		{
			// �V���O���g���̃����o�ϐ���j��
			m_pSingleto->m_pVertex->Release();
			m_pSingleto->m_pVertex = NULL;
		}
		// �V�[����NULL�ɂ���
		m_pSingleto->m_pNexScene = NULL;

		// �V���O���g����j��
		delete m_pSingleto;
		m_pSingleto = NULL;
	}
}

//************************************
// �X�V����
//************************************
void CFade::Update(void)
{
	// ������ԈȊO��������
	if (m_pSingleto->m_Fade != FADE_NONE)
	{
		VERTEX_2D* pVtx;
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pSingleto->m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

		// �t�F�[�h�C��
		if (m_pSingleto->m_Fade == FADE_IN)
		{
			// �A���t�@�l��������
			m_pSingleto->m_colorFade.a = Clamp(m_pSingleto->m_colorFade.a - Config::Speed, 0.0f, 1.0f);
			// �ŏ��l�ɂȂ�����
			if (m_pSingleto->m_colorFade.a <= 0.0f)
			{
				m_pSingleto->m_Fade = FADE_NONE;
			}
		}
		// �t�F�[�h�A�E�g
		else if (m_pSingleto->m_Fade == FADE_OUT)
		{
			// �A���t�@�l��������
			m_pSingleto->m_colorFade.a = Clamp(m_pSingleto->m_colorFade.a + Config::Speed, 0.0f, 1.0f);
			// �ő�l�ɂȂ�����
			if (m_pSingleto->m_colorFade.a >= 1.0f)
			{
				m_pSingleto->m_Fade = FADE_IN;
				CManager::SetScene(m_pSingleto->m_pNexScene);
			}
		}
		//���_�J���[�̐ݒ�
		pVtx[0].col = m_pSingleto->m_colorFade;
		pVtx[1].col = m_pSingleto->m_colorFade;
		pVtx[2].col = m_pSingleto->m_colorFade;
		pVtx[3].col = m_pSingleto->m_colorFade;

		//���_�o�b�t�@���A�����b�N
		m_pSingleto->m_pVertex->Unlock();
	}
}

//************************************
// �`�揈��
//************************************
void CFade::Draw(void)
{
	// �f�o�C�X�̎擾
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pSingleto->m_pVertex, 0, sizeof(VERTEX_2D));

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}

//************************************
// �t�F�[�h�Ǝ��̃V�[����ݒ�
//************************************
void CFade::SetFade(CScene* pNextScene)
{
	// ���̃t�F�[�h��������Ԃ���Ȃ�������
	if (m_pSingleto->m_Fade != FADE_NONE)
	{
		// �|�C���^��j�����������I��
		delete pNextScene;
		pNextScene = nullptr;
		return;
	}
	// ���̃V�[����NULL����Ȃ�������
	if (m_pSingleto->m_pNexScene != NULL)
	{
		m_pSingleto->m_pNexScene = NULL;
	}
	// ���̃V�[���̃|�C���^�ƃ��[�h��ݒ�
	m_pSingleto->m_Fade = FADE_OUT;
	m_pSingleto->m_pNexScene = pNextScene;
}

//************************************
// �V���O���g���𐶐�
//************************************
CFade* CFade::CreateSingle(void)
{
	// �V���O���g������������Ă����琶�����Ȃ�
	if (m_pSingleto != NULL) return m_pSingleto;
	// ����
	m_pSingleto = new CFade;
	m_pSingleto->Init();
	return m_pSingleto;
}

//************************************
// �f�X�g���N�^
//************************************
CFade::~CFade()
{
}