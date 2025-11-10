//****************************************************************
//
// ランキングの処理[ranking.cpp]
// Author Kensaku Hatori
//
//****************************************************************
#include "ranking.h"
#include "timer.h"

using namespace std;

//****************************************************************
// コンストラクタ
//****************************************************************
CRanking::CRanking()
{
	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		m_nTime[nCnt] = NULL;
		m_nMin[nCnt] = NULL;
		m_nSec[nCnt] = NULL;
	}
}

//****************************************************************
// デストラクタ
//****************************************************************
CRanking::~CRanking()
{

}

//****************************************************************
// 初期化
//****************************************************************
HRESULT CRanking::Init(void)
{
	// 読み込み
	LoadFile();

	// ナンバーの初期化
	InitNum();

	return S_OK;
}

//****************************************************************
// 破棄
//****************************************************************
void CRanking::Uninit(void)
{
	for (int nNum = 0; nNum < MAX_NUM; nNum++)
	{
		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			if (m_pNumber1[nCnt][nNum] != nullptr)
			{
				m_pNumber1[nCnt][nNum]->Uninit();

				delete m_pNumber1[nCnt][nNum];
				m_pNumber1[nCnt][nNum] = nullptr;
			}

			if (m_pNumber2[nCnt][nNum] != nullptr)
			{
				m_pNumber2[nCnt][nNum]->Uninit();

				delete m_pNumber2[nCnt][nNum];
				m_pNumber2[nCnt][nNum] = nullptr;
			}
		}

		if (m_pNumber3[nNum] != nullptr)
		{
			m_pNumber3[nNum]->Uninit();

			delete m_pNumber3[nNum];
			m_pNumber3[nNum] = nullptr;
		}
	}
}

//****************************************************************
// 更新
//****************************************************************
void CRanking::Update(void)
{
	// ソート処理
	Sort();

	// 変換
	Change();
}

//****************************************************************
// 描画
//****************************************************************
void CRanking::Draw(void)
{

	for (int nNum = 0; nNum < MAX_NUM; nNum++)
	{
		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			m_pNumber1[nCnt][nNum]->Draw();
			m_pNumber2[nCnt][nNum]->Draw();
		}

		m_pNumber3[nNum]->Draw();
	}
}

//****************************************************************
// ソート
//****************************************************************
void CRanking::Sort(void)
{
	// 今の総数
	int nNowTime = CTimer::GetTimer();

	// ソート用ローカル変数
	int nCnt = 0;
	int nData = 0;

	if (m_nTime[MAX_NUM - 1] > nNowTime)
	{
		m_nTime[MAX_NUM - 1] = nNowTime;
		nCnt = MAX_NUM - 1;
	}

	for (int nCnt1 = 0; nCnt1 < MAX_NUM; nCnt1++)
	{
		for (int nCnt2 = nCnt1 + 1; nCnt2 < MAX_NUM; nCnt2++)
		{
			if (m_nTime[nCnt1] >= m_nTime[nCnt2])
			{
				nData = m_nTime[nCnt1];
				m_nTime[nCnt1] = m_nTime[nCnt2];
				m_nTime[nCnt2] = nData;
			}
		}
	}

	for (int nCnt3 = 0; nCnt3 < MAX_NUM; nCnt3++)
	{
		if (nNowTime == m_nTime[nCnt3])
		{
			nCnt = nCnt3;
			break;
		}
	}

	// 書き込み
	WriteFile();
}

//****************************************************************
// 変換(総タイムから分秒に変換)
//****************************************************************
void CRanking::Change(void)
{
	for (int nCnt = 0; nCnt < MAX_NUM + 1; nCnt++)
	{
		m_nMin[nCnt] = m_nTime[nCnt] / MAX_MINUTES;
		m_nSec[nCnt] = m_nTime[nCnt] % MAX_SECOND;
	}


	int aPosTexU[MAX_TIMER] = {};
	int nData = 100;
	int nData1 = 10;

	for (int nNum = 0; nNum < MAX_NUM; nNum++)
	{
		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			// 0番目以外
			aPosTexU[nCnt] = (m_nSec[nNum] % nData) / nData1;
			nData = nData / 10;
			nData1 = nData1 / 10;

			m_pNumber1[nCnt][nNum]->SetNumber(aPosTexU[nCnt], 4);
		}
	}

	for (int nNum = 0; nNum < MAX_NUM; nNum++)
	{
		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			// 0番目以外
			aPosTexU[nCnt] = (m_nMin[nNum] % nData) / nData1;
			nData = nData / 10;
			nData1 = nData1 / 10;

			m_pNumber2[nCnt][nNum]->SetNumber(aPosTexU[nCnt], 4);
		}
	}
}

//****************************************************************
// ファイル読み込み
//****************************************************************
void CRanking::LoadFile(void)
{
	ifstream pFile("data\\Ranking.txt");
	string line = {};

	// ファイルが正常に開けたら
	if (pFile.is_open())
	{
		int nCnt = 0;

		while (getline(pFile, line))
		{
			//getline(pFileMin, Minline);

			istringstream iss(line);

			iss >> m_nTime[nCnt];

			nCnt++;
		}

		// ファイルを閉じる
		pFile.close();
	}
}

//****************************************************************
// ファイル書き込み
//****************************************************************
void CRanking::WriteFile(void)
{
	ofstream outFile("data\\Ranking.txt");
	string line = {};

	// ファイルが正常に開けたら
	if (outFile.is_open())
	{
		for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
		{
			outFile << m_nTime[nCnt] << endl;
		}

		// ファイルを閉じる
		outFile.close();
	}
}

//****************************************************************
// ナンバー初期化
//****************************************************************
void CRanking::InitNum(void)
{
	for (int nNum = 0; nNum < MAX_NUM; nNum++)
	{
		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			m_pNumber1[nCnt][nNum] = new CNumber;

			if (m_pNumber1[nCnt][nNum] != nullptr)
			{
				m_pNumber1[nCnt][nNum]->Init(200.0f, 200.0f, 0.0f, 50.0f, nCnt, nNum, 50.0f, 50.0f, 50.0f, MAX_TIMER, 4, "data\\TEXTURE\\number005.png", 0.1f);
			}

			m_pNumber2[nCnt][nNum] = new CNumber;

			if (m_pNumber2[nCnt][nNum] != nullptr)
			{
				m_pNumber2[nCnt][nNum]->Init(50.0f, 50.0f, 0.0f, 50.0f, nCnt, nNum, 50.0f, 50.0f, 50.0f, MAX_TIMER, 4, "data\\TEXTURE\\number005.png", 0.1f);
			}
		}

		m_pNumber3[nNum] = new CNumber;

		if (m_pNumber3[nNum] != nullptr)
		{
			m_pNumber3[nNum]->Init(150.0f, 200.0f, 0.0f, 50.0f, 0, nNum, 1.0f, 0.0f, 50.0f, 1, 0, "data\\TEXTURE\\coron.png", 1.0f);
		}
	}
}