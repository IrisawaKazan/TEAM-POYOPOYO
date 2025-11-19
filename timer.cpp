//****************************************************************
//
// [timer.cpp]
// Author: Irisawa Kazan
//
//****************************************************************
#include"timer.h"
#include"number.h"
#include"manager.h"
#include "mapmanager.h"
#include "game.h"
#include "fade.h"
#include "result.h"

// 静的メンバ変数宣言
CNumber* CTimer::m_pNumber1[MAX_TIMER] = {};
CNumber* CTimer::m_pNumber2[MAX_TIMER] = {};
CNumber* CTimer::m_pNumber3 = {};
int CTimer::m_nTimer = NULL;

//****************************************************************
// コンストラクタ
//****************************************************************
CTimer::CTimer(int nPriority) : CObject(nPriority)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nTime = NULL;
	m_nNs = NULL;
	m_nMin = NULL;
	m_nHour = NULL;
	m_bFinish = false;
	Init();
}

//****************************************************************
// デストラクタ
//****************************************************************
CTimer::~CTimer()
{

}

//****************************************************************
// タイマーの生成処理
//****************************************************************
CTimer* CTimer::Create(D3DXVECTOR3 pos)
{
	CTimer* pTimer = nullptr;
	pTimer = new CTimer;

	if (pTimer != nullptr)
	{
		pTimer->m_pos = pos;
		pTimer->Init();
		return pTimer;
	}
	else
	{
		return nullptr;
	}
}

//****************************************************************
// タイマーの初期化処理
//****************************************************************
HRESULT CTimer::Init(void)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nMin = 0;
	m_nTimer = 0;
	m_nHour = 0;
	m_bFinish = false;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt] = new CNumber;

		if (m_pNumber1[nCnt] != nullptr)
		{
			m_pNumber1[nCnt]->Init(200.0f, 200.0f, 0.0f, 50.0f, nCnt, 0, 50.0f, 50.0f, 0.0f, MAX_TIMER, 4, "data\\TEXTURE\\number000.png", 0.1f, CNumber::TYPE_NONE);
		}

		m_pNumber2[nCnt] = new CNumber;

		if (m_pNumber2[nCnt] != nullptr)
		{
			m_pNumber2[nCnt]->Init(50.0f, 50.0f, 0.0f, 50.0f, nCnt, 0, 50.0f, 50.0f, 0.0f, MAX_TIMER, 4, "data\\TEXTURE\\number000.png", 0.1f, CNumber::TYPE_NONE);
		}
	}

	m_pNumber3 = new CNumber;

	if (m_pNumber3 != nullptr)
	{
		m_pNumber3->Init(150.0f, 200.0f, 0.0f, 50.0f ,0, 0, 1.0f, 0.0f, 0.0f, 1, 0, "data\\TEXTURE\\coron000.png", 1.0f, CNumber::TYPE_NONE);
	}

	return S_OK;
}

//****************************************************************
// タイマーの終了処理
//****************************************************************
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

//****************************************************************
// タイマーの更新処理
//****************************************************************
void CTimer::Update(void)
{
	bool bTime = CMapManager::GetGoal();
	bool bTime1 = CGame::GetGoal();

	// 秒の加算
	m_nNs++;

	// 時の計算
	m_nHour++;

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

	if (m_nHour >= MAX_HOUR)
	{
		SubNs(-60);
		SubMin(-60);

		m_nHour = 0;
	}

	// ゴールしていなかったら
	if (bTime != true || bTime1 != true || m_bFinish != true)
	{
		// 総タイムを加算
		m_nTimer++;
	}

	//　3分たったら
	if (m_nTimer >= MAX_TIMEOVER)
	{
		m_bFinish = true;
		CFade::SetFade(new CResult);
	}
}

//****************************************************************
// タイマーの描画処理
//****************************************************************
void CTimer::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt]->Draw();
		m_pNumber2[nCnt]->Draw();
	}

	m_pNumber3->Draw();
}

//****************************************************************
// タイマーの位置の設定処理
//****************************************************************
void CTimer::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//****************************************************************
// タイマーの位置の取得処理
//****************************************************************
D3DXVECTOR3 CTimer::GetPos(void)
{
	return m_pos;
}

//****************************************************************
// タイマーの秒減算処理
//****************************************************************
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

//****************************************************************
// タイマーの分減算処理
//****************************************************************
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

//****************************************************************
// タイマーの秒数取得処理
//****************************************************************
int CTimer::GetNs(void)
{
	return m_nNs;
}

//****************************************************************
// タイマーの分数取得処理
//****************************************************************
int CTimer::GetMin(void)
{
	return m_nMin;
}