//****************************************************************
//
// ランキングの処理[ranking.cpp]
// Author Kensaku Hatori
//
//****************************************************************
#include "ranking.h"
#include "timer.h"
#include "math_T.h"

using namespace std;

//****************************************************************
// コンストラクタ
//****************************************************************
CRanking::CRanking()
{
	m_pBrackboard = nullptr;

	for (int nCnt = 0; nCnt < MAX_NUM; nCnt++)
	{
		for (int nTime = 0; nTime < MAX_TIMER; nTime++)
		{
			m_pNumber1[nTime][nCnt] = nullptr;
		}
		m_nTime[nCnt] = NULL;
		m_nMin[nCnt] = NULL;
		m_nSec[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_pNumber4[nCnt] = nullptr;
	}

	m_nMinutes = NULL;
	m_nSeconds = NULL;
	m_nData = NULL;
	m_bAct = false;
	m_nNum = NULL;
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
	m_bAct = false;
	m_nRankIdx = MAX_NUM - 1;

	// 読み込み
	LoadFile();

	// ナンバーの初期化
	InitNum();

	// 黒ポリゴン
	m_pBrackboard = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH,0.0f,0.0f), VEC3_NULL, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 1.0f });

	m_pBrackboard->SetCol({ 0.0f, 0.0f, 0.0f, 0.5f });

	// 変換
	Change();

	// サウンドの取得
	CSound* pSound = CManager::GetSound();

	// BGM
	pSound->Play(CSound::LABEL_RANKING_BGM);

	return S_OK;
}

//****************************************************************
// 破棄
//****************************************************************
void CRanking::Uninit(void)
{
	for (int nNum = 0; nNum < MAX_NUM; nNum++)
	{
		// 分秒
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

		// コロン
		if (m_pNumber3[nNum] != nullptr)
		{
			m_pNumber3[nNum]->Uninit();

			delete m_pNumber3[nNum];
			m_pNumber3[nNum] = nullptr;
		}
	}

	// ランキング
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (m_pNumber4[nCnt] != nullptr)
		{
			m_pNumber4[nCnt]->Uninit();

			delete m_pNumber4[nCnt];
			m_pNumber4[nCnt] = nullptr;
		}
	}
}

//****************************************************************
// 更新
//****************************************************************
void CRanking::Update(void)
{
	// 今の総数
	int nNowTime = CTimer::GetTimer();

	// 現在の分秒の計算
	m_nMinutes = nNowTime / MAX_MINUTES;
	m_nSeconds = (nNowTime % MAX_MINUTES) / MAX_SECOND;

	for (m_nNum = 0; m_nNum < MAX_NUM; m_nNum++)
	{
		if (m_bAct != true)
		{
			// 現在の分秒と一致していたら
			if (m_nMin[m_nNum] == m_nMinutes && m_nSec[m_nNum] == m_nSeconds)
			{
				m_bAct = true;
				m_nData = m_nNum;
			}
		}
	}

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt][MAX_NUM - 1]->ColAnim();
		m_pNumber2[nCnt][MAX_NUM - 1]->ColAnim();

		m_pNumber1[nCnt][m_nData]->ColAnim();
		m_pNumber2[nCnt][m_nData]->ColAnim();
	}

	// カラーの設定
	m_pNumber3[MAX_NUM - 1]->ColAnim();
	m_pNumber3[m_nData]->ColAnim();

	static int nCnt = 0;
	static int nData = 0;

	if (nCnt >= 30)
	{
		SetNumUpdate(m_nRankIdx);
		nData++;

		if (nData >= 60)
		{
			//nCnt = 0;
			nData = 0;
			if (m_nRankIdx >= 0)m_nRankIdx--;
		}
	}
	else
	{
		nCnt++;
	}
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

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_pNumber4[nCnt]->Draw();
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
	int nData = 0;

	if (nNowTime < 14400 && nNowTime >= 60)
	{
		// 0秒じゃなかったら
		if (m_nTime[MAX_NUM - 2] >= 60)
		{
			// 配列の最後の数値が今の数値より大きかったら
			if (m_nTime[MAX_NUM - 2] > nNowTime)
			{
				m_nTime[MAX_NUM - 2] = nNowTime;
			}
		}
		else
		{
			if (m_nTime[MAX_NUM - 2] < nNowTime)
			{
				m_nTime[MAX_NUM - 2] = nNowTime;
			}
		}

		for (int nCnt1 = 0; nCnt1 < MAX_NUM - 1; nCnt1++)
		{
			for (int nCnt2 = nCnt1 + 1; nCnt2 < MAX_NUM - 1; nCnt2++)
			{
				// 比較元0秒じゃなかったら
				if (m_nTime[nCnt1] >= 60)
				{
					// 比較元が比較先より大きかったら
					if (m_nTime[nCnt1] >= m_nTime[nCnt2])
					{
						// 比較先が0秒じゃなかったら
						if (m_nTime[nCnt2] >= 60)
						{
							nData = m_nTime[nCnt1];
							m_nTime[nCnt1] = m_nTime[nCnt2];
							m_nTime[nCnt2] = nData;
						}
					}
				}
				else
				{
					nData = m_nTime[nCnt2];
					m_nTime[nCnt2] = m_nTime[nCnt1];
					m_nTime[nCnt1] = nData;
				}
			}
		}

		// 書き込み
		WriteFile();
	}
}

//****************************************************************
// 変換(総タイムから分秒に変換)
//****************************************************************
void CRanking::Change(void)
{
	// 今の総数
	int nNowTime = CTimer::GetTimer();

	// ソート処理
	Sort();

	// 読み込み
	LoadFile();

	for (int nCnt = 0; nCnt < MAX_NUM - 1; nCnt++)
	{
		m_nMin[nCnt] = m_nTime[nCnt] / MAX_MINUTES;
		m_nSec[nCnt] = (m_nTime[nCnt] % MAX_MINUTES) / MAX_SECOND;
	}



	for (int nNum = 0; nNum < MAX_NUM - 1; nNum++)
	{
		int aPosTexU[MAX_TIMER] = {};
		int nData = 100;
		int nData1 = 10;

		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			// 0番目以外(秒)
			aPosTexU[nCnt] = (m_nSec[nNum] % nData) / nData1;
			nData = nData / 10;
			nData1 = nData1 / 10;

			m_pNumber1[nCnt][nNum]->SetNumber(aPosTexU[nCnt], 4);
		}

		int aPosTexU1[MAX_TIMER] = {};
		int nData2 = 100;
		int nData3 = 10;

		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			// 0番目以外(分)
			aPosTexU1[nCnt] = (m_nMin[nNum] % nData2) / nData3;
			nData2 = nData2 / 10;
			nData3 = nData3 / 10;

			m_pNumber2[nCnt][nNum]->SetNumber(aPosTexU1[nCnt], 4);
		}
	}

	m_nMin[MAX_NUM - 1] = nNowTime / MAX_MINUTES;
	m_nSec[MAX_NUM - 1] = (nNowTime % MAX_MINUTES) / MAX_SECOND;

	int aPosTexU3[MAX_TIMER] = {};
	int nData[MAX_TIMER] = {};
	int nData1[MAX_TIMER] = {};

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		nData[nCnt] = 100;
		nData1[nCnt] = 10;
	}

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		// 0番目以外(秒)
		aPosTexU3[nCnt] = (m_nSec[MAX_NUM - 1] % nData[0]) / nData1[0];
		nData[0] = nData[0] / 10;
		nData1[0] = nData1[0] / 10;

		m_pNumber1[nCnt][MAX_NUM - 1]->SetNumber(aPosTexU3[nCnt], 4);

		// 0番目以外(分)
		aPosTexU3[nCnt] = (m_nMin[MAX_NUM - 1] % nData[1]) / nData1[1];
		nData[1] = nData[1] / 10;
		nData1[1] = nData1[1] / 10;

		m_pNumber2[nCnt][MAX_NUM - 1]->SetNumber(aPosTexU3[nCnt], 4);
	}

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		int aPosTexU4[MAX_RANKING] = {};
		int nRank = 10;
		int nRank1 = 1;

		// 0番目以外(秒)
		aPosTexU4[nCnt] = (nCnt + 1 % nRank) / nRank1;
		nRank = nRank / 10;
		nRank1 = nRank1 / 10;

		m_pNumber4[nCnt]->SetNumber(aPosTexU4[nCnt], 4);
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

	// ファイルが正常に開けたら
	if (outFile.is_open())
	{
		for (int nCnt = 0; nCnt < MAX_NUM - 1; nCnt++)
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
	float fMinX = 2010.0f;
	float fSecX = 1850.0f;
	float fRankX = 750.0f;

	// 全体のランキング
	for (int nNum = 0; nNum < MAX_NUM - 1; nNum++)
	{
		for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
		{
			m_pNumber1[nCnt][nNum] = new CNumber;

			if (m_pNumber1[nCnt][nNum] != nullptr)
			{
				m_pNumber1[nCnt][nNum]->Init(fMinX, fMinX, 300.0f, 350.0f, nCnt, nNum, 55.0f, 50.0f, 75.0f, MAX_TIMER, 4, "data\\TEXTURE\\number000.png", 0.1f,CNumber::TYPE_MIN);
			}

			m_pNumber2[nCnt][nNum] = new CNumber;

			if (m_pNumber2[nCnt][nNum] != nullptr)
			{
				m_pNumber2[nCnt][nNum]->Init(fSecX, fSecX, 300.0f, 350.0f, nCnt, nNum, 55.0f, 50.0f, 75.0f, MAX_TIMER, 4, "data\\TEXTURE\\number000.png", 0.1f,CNumber::TYPE_SEC);
			}
		}

		m_pNumber3[nNum] = new CNumber;

		if (m_pNumber3[nNum] != nullptr)
		{
			m_pNumber3[nNum]->Init(1960.0f, 1990.0f, 300.0f, 350.0f, 0, nNum, 1.0f, 0.0f, 75.0f, 1, 0, "data\\TEXTURE\\coron000.png", 1.0f,CNumber::TYPE_CORON);
		}
	}

	// 今回のスコア
	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt][MAX_NUM - 1] = new CNumber;

		if (m_pNumber1[nCnt][MAX_NUM - 1] != nullptr)
		{
			m_pNumber1[nCnt][MAX_NUM - 1]->Init(fMinX, fMinX, 100.0f, 150.0f, nCnt, 0, 55.0f, 50.0f, 75.0f, MAX_TIMER, 4, "data\\TEXTURE\\number000.png", 0.1f,CNumber::TYPE_MIN1);
		}

		m_pNumber2[nCnt][MAX_NUM - 1] = new CNumber;

		if (m_pNumber2[nCnt][MAX_NUM - 1] != nullptr)
		{
			m_pNumber2[nCnt][MAX_NUM - 1]->Init(fSecX, fSecX, 100.0f, 150.0f, nCnt, 0, 55.0f, 50.0f, 75.0f, MAX_TIMER, 4, "data\\TEXTURE\\number000.png", 0.1f, CNumber::TYPE_SEC1);
		}
	}

	m_pNumber3[MAX_NUM - 1] = new CNumber;

	if (m_pNumber3[MAX_NUM - 1] != nullptr)
	{
		m_pNumber3[MAX_NUM - 1]->Init(1960.0f, 1990.0f, 100.0f, 150.0f, 0, 0, 1.0f, 0.0f, 75.0f, 1, 0, "data\\TEXTURE\\coron000.png", 1.0f, CNumber::TYPE_CORON1);
	}

	// ランキング
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		m_pNumber4[nCnt] = new CNumber;

		if (m_pNumber4[nCnt] != nullptr)
		{
			m_pNumber4[nCnt]->Init(fRankX, fRankX, 300.0f, 350.0f, 0, nCnt, 50.0f, 50.0f, 75.0f, MAX_TIMER, 4, "data\\TEXTURE\\RankNum.png", 0.1f, CNumber::TYPE_NONE);
		}
	}
}

//****************************************************************
// ナンバーのイージングセット
//****************************************************************
void CRanking::SetNumUpdate(int nCntNum)
{
	if (nCntNum < 0)return;

	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
	{
		m_pNumber1[nCnt][nCntNum]->Update();
		m_pNumber2[nCnt][nCntNum]->Update();
	}

	m_pNumber3[nCntNum]->Update();
}