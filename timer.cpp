//==============================================================
//
// [timer.cpp]
// Author: Irisawa Kazan
//
//==============================================================
#include"timer.h"
#include"number.h"
#include"manager.h"
#include "mapmanager.h"

// 静的メンバ変数宣言
CNumber* CTimer::m_pNumber1[MAX_TIMER] = {};
CNumber* CTimer::m_pNumber2[MAX_TIMER] = {};
CNumber* CTimer::m_pNumber3 = {};
int CTimer::m_nTimer = NULL;

//----------------------------------------
// コンストラクタ
//----------------------------------------
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nTime = NULL;
	m_nNs = NULL;
	m_nMin = NULL;
	Init();
}

//----------------------------------------
// デストラクタ
//----------------------------------------
CTimer::~CTimer()
{

}

//----------------------------------------
// タイマーの生成処理
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
// タイマーの初期化処理
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
			m_pNumber1[nCnt]->Init(200.0f, 200.0f, 0.0f, 50.0f, nCnt, 0, 50.0f, 50.0f, 0.0f, MAX_TIMER, 4, "data\\TEXTURE\\number005.png", 0.1f);
		}

		m_pNumber2[nCnt] = new CNumber;

		if (m_pNumber2[nCnt] != nullptr)
		{
			m_pNumber2[nCnt]->Init(50.0f, 50.0f, 0.0f, 50.0f, nCnt, 0, 50.0f, 50.0f, 0.0f, MAX_TIMER, 4, "data\\TEXTURE\\number005.png", 0.1f);
		}
	}

	m_pNumber3 = new CNumber;

	if (m_pNumber3 != nullptr)
	{
		m_pNumber3->Init(150.0f, 200.0f, 0.0f, 50.0f ,0, 0, 1.0f, 0.0f, 0.0f, 1, 0, "data\\TEXTURE\\coron.png", 1.0f);
	}

	return S_OK;
}

//----------------------------------------
// タイマーの終了処理
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
// タイマーの更新処理
//----------------------------------------
void CTimer::Update(void)
{
	bool bTime = CMapManager::GetGoal();

	// 秒の加算
	m_nNs++;

	// 1秒経過
	if (m_nNs > 60)
	{
		SubNs(1);

		m_nNs = 0;
	}

	// 1分経過
	if (m_nTime >= 60)
	{
		SubNs(-60);
		SubMin(1);

		m_nTime = 0;
	}

	// ゴールしていなかったら
	if (bTime != true)
	{
		// 総タイムを加算
		m_nTimer++;
	}
}

//----------------------------------------
// タイマーの描画処理
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
// タイマーの位置の設定処理
//----------------------------------------
void CTimer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//----------------------------------------
// タイマーの位置の取得処理
//----------------------------------------
D3DXVECTOR3 CTimer::GetPos(void)
{
	return m_pos;
}

//----------------------------------------
// タイマーの設定処理
//----------------------------------------
void CTimer::SetTime(int nTime)
{
	// 秒数
	int aPosTexU[MAX_TIMER] = {};	// 各桁の数字を格納
	int nData = 100;				// 3桁
	int nData1 = 10;				// 2桁

	m_nTime += nTime;
	
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		if (nCnt == 0)
		{
			// 0番目だったら
			aPosTexU[0] = m_nMin / nData;
		}
		else
		{
			// 0番目以外
			aPosTexU[nCnt] = (m_nMin % nData) / nData1;
			nData = nData / 10;
			nData1 = nData1 / 10;
		}

		m_pNumber1[nCnt]->SetNumber(aPosTexU[nCnt], 4);
	}
}

//----------------------------------------
// タイマーの秒減算処理
//----------------------------------------
void CTimer::SubNs(int nValue)
{
	int aPosTexU[MAX_TIMER] = {};
	int nData = 100;
	int nData1 = 10;

	m_nTime += nValue;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// 0番目以外
		aPosTexU[nCnt] = (m_nTime % nData) / nData1;
		nData = nData / 10;
		nData1 = nData1 / 10;

		m_pNumber1[nCnt]->SetNumber(aPosTexU[nCnt], 4);
	}
}

//----------------------------------------
// タイマーの分減算処理
//----------------------------------------
void CTimer::SubMin(int nValue)
{
	int aPosTexU[MAX_TIMER] = {};
	int nData = 100;
	int nData1 = 10;

	m_nMin += nValue;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// 0番目以外
		aPosTexU[nCnt] = (m_nMin % nData) / nData1;
		nData = nData / 10;
		nData1 = nData1 / 10;

		m_pNumber2[nCnt]->SetNumber(aPosTexU[nCnt], 4);
	}
}

//----------------------------------------
// タイマーの秒数取得処理
//----------------------------------------
int CTimer::GetNs(void)
{
	return m_nNs;
}

//----------------------------------------
// タイマーの分数取得処理
//----------------------------------------
int CTimer::GetMin(void)
{
	return m_nMin;
}