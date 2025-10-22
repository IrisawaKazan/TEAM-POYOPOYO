//==============================================================
//
// [timer.cpp]
// Author: Irisawa Kazan
//
//==============================================================
#include"timer.h"
#include"number.h"
#include"manager.h"

// �ÓI�����o�ϐ��錾
CNumber* CTimer::m_pNumber1[MAX_TIMER] = {};
CNumber* CTimer::m_pNumber2[MAX_TIMER] = {};
CNumber* CTimer::m_pNumber3 = {};
int CTimer::m_nTimer = NULL;

//----------------------------------------
// �R���X�g���N�^
//----------------------------------------
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nTime = NULL;
	m_nNs = NULL;
	m_nMin = NULL;
}

//----------------------------------------
// �f�X�g���N�^
//----------------------------------------
CTimer::~CTimer()
{

}

//----------------------------------------
// �^�C�}�[�̐�������
//----------------------------------------
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer* pTimer = nullptr;

	if (pTimer == nullptr)
	{
		pTimer = new CTimer;
	}

	pTimer->m_pos = pos;

	pTimer->Init();

	return pTimer;
}

//----------------------------------------
// �^�C�}�[�̏���������
//----------------------------------------
HRESULT CTimer::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nMin = 0;

	m_nTimer = 0;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt] = new CNumber;

		if (m_pNumber1[nCnt] != nullptr)
		{
			m_pNumber1[nCnt]->Init(200.0f, 200.0f, nCnt, 50.0f, 50.0f, MAX_TIMER, 4, "data\\TEXTURE\\number005.png", 0.1f);
		}

		m_pNumber2[nCnt] = new CNumber;

		if (m_pNumber2[nCnt] != nullptr)
		{
			m_pNumber2[nCnt]->Init(50.0f, 50.0f, nCnt, 50.0f, 50.0f, MAX_TIMER, 4, "data\\TEXTURE\\number005.png", 0.1f);
		}
	}

	m_pNumber3 = new CNumber;

	if (m_pNumber3 != nullptr)
	{
		m_pNumber3->Init(150.0f, 200.0f, 0, 1.0f, 0.0f, 1, 0, "data\\TEXTURE\\coron.png", 1.0f);
	}

	return S_OK;
}

//----------------------------------------
// �^�C�}�[�̏I������
//----------------------------------------
void CTimer::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (m_pNumber1[nCnt] != nullptr)
		{
			m_pNumber1[nCnt]->Uninit();

			delete m_pNumber1[nCnt];
			m_pNumber1[nCnt] = nullptr;
		}

		if (m_pNumber2[nCnt] != nullptr)
		{
			m_pNumber2[nCnt]->Uninit();

			delete m_pNumber2[nCnt];
			m_pNumber2[nCnt] = nullptr;
		}
	}

	if (m_pNumber3 != nullptr)
	{
		m_pNumber3->Uninit();

		delete m_pNumber3;
		m_pNumber3 = nullptr;
	}

	CObject::Release();
}

//----------------------------------------
// �^�C�}�[�̍X�V����
//----------------------------------------
void CTimer::Update(void)
{
	m_nNs++;

	// 1�b�o��
	if (m_nNs > 60)
	{
		SubNs(1);

		m_nNs = 0;
	}

	// 1���o��
	if (m_nTime >= 60)
	{
		SubNs(-60);
		SubMin(1);

		m_nTime = 0;
	}
}

//----------------------------------------
// �^�C�}�[�̕`�揈��
//----------------------------------------
void CTimer::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt]->Draw();
		m_pNumber2[nCnt]->Draw();
	}

	m_pNumber3->Draw();
}

//----------------------------------------
// �^�C�}�[�̈ʒu�̐ݒ菈��
//----------------------------------------
void CTimer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//----------------------------------------
// �^�C�}�[�̈ʒu�̎擾����
//----------------------------------------
D3DXVECTOR3 CTimer::GetPos(void)
{
	return m_pos;
}

//----------------------------------------
// �^�C�}�[�̐ݒ菈��
//----------------------------------------
void CTimer::SetTime(int nTime)
{
	// �b��
	int aPosTexU[MAX_TIMER] = {};	// �e���̐������i�[
	int nData = 100;				// 3��
	int nData1 = 10;				// 2��

	m_nTime -= nTime;

	for (int nCnt1 = 0; nCnt1 < MAX_TIMER; nCnt1++)
	{
		if (nCnt1 == 0)
		{
			// 0�Ԗڂ�������
			aPosTexU[0] = m_nTime / nData;
		}
		else
		{
			// 0�ԖڈȊO
			aPosTexU[nCnt1] = (m_nTime % nData) / nData1;
			nData = nData / 10;
			nData1 = nData1 / 10;
		}

		m_pNumber2[nCnt1]->SetNumber(aPosTexU[nCnt1], 4);
	}

	int nData2 = 100;
	int nData3 = 10;
	
	for (int nCnt2 = 0; nCnt2 < MAX_TIMER; nCnt2++)
	{
		if (nCnt2 == 0)
		{
			// 0�Ԗڂ�������
			aPosTexU[0] = m_nMin / nData2;
		}
		else
		{
			// 0�ԖڈȊO
			aPosTexU[nCnt2] = (m_nMin % nData2) / nData3;
			nData2 = nData2 / 10;
			nData3 = nData3 / 10;
		}

		m_pNumber1[nCnt2]->SetNumber(aPosTexU[nCnt2], 4);
	}
}

//----------------------------------------
// �^�C�}�[�̕b���Z����
//----------------------------------------
void CTimer::SubNs(int nValue)
{
	int aPosTexU[MAX_TIMER] = {};
	int nData = 100;
	int nData1 = 10;

	m_nTime = nValue;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// 0�ԖڈȊO
		aPosTexU[nCnt] = (m_nTime % nData) / nData1;
		nData = nData / 10;
		nData1 = nData1 / 10;

		m_pNumber1[nCnt]->SetNumber(aPosTexU[nCnt], 4);
	}
}

//----------------------------------------
// �^�C�}�[�̕����Z����
//----------------------------------------
void CTimer::SubMin(int nValue)
{
	int aPosTexU[MAX_TIMER] = {};
	int nData = 100;
	int nData1 = 10;

	m_nMin = nValue;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// 0�ԖڈȊO
		aPosTexU[nCnt] = (m_nMin % nData) / nData1;
		nData = nData / 10;
		nData1 = nData1 / 10;

		m_pNumber2[nCnt]->SetNumber(aPosTexU[nCnt], 4);
	}
}

//----------------------------------------
// �^�C�}�[�̕b���擾����
//----------------------------------------
int CTimer::GetNs(void)
{
	return m_nNs;
}

//----------------------------------------
// �^�C�}�[�̕����擾����
//----------------------------------------
int CTimer::GetMin(void)
{
	return m_nMin;
}