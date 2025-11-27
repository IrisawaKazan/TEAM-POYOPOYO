//****************************************************************
//
// [timer.cpp]
// Author: Irisawa Kazan
//
//****************************************************************
#include "timer.h"
#include "number.h"
#include "file.h"

const D3DXVECTOR2 CTimer::TEXTURE_SIZE = { 566,80 };   // テクスチャサイズ
const D3DXVECTOR2 CTimer::TEXTURE_UV_COUNT = { 11,1 }; // テクスチャ分割

//****************************************************************
// タイマーの生成処理
//****************************************************************
CTimer* CTimer::Create(D3DXVECTOR3 pos, size_t timeCount, int startTime, int limitTime, int nPriority)
{
	// インスタンスの生成
	CTimer* pNumber = new CTimer(timeCount, nPriority); // 表示桁数
	if (pNumber == nullptr)
	{
		return nullptr;
	}

	pNumber->SetPos(pos);             // 位置
	pNumber->SetStartTime(startTime); // 開始秒数 (カウントDown用)
	pNumber->SetLimitTime(limitTime); // 制限時間 (カウントUp用)

	// 初期化
	if (FAILED(pNumber->Init()))
	{
		delete pNumber;
		return nullptr;
	}
	return pNumber;
}

//****************************************************************
// タイマーの初期化処理
//****************************************************************
HRESULT CTimer::Init(void)
{
	m_timeOver = false;
	m_count = COUNT::None;

	// スクリーンのサイズ
	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);
	m_pNumber = CNumber::Create(m_timeCount, CNumber::TYPE::Time, TEXTURE_PATH, TEXTURE_UV_COUNT, m_pos, D3DXVECTOR2((TEXTURE_SIZE.x / TEXTURE_UV_COUNT.x) * screenSize.x * NUMBER_SCALE, (TEXTURE_SIZE.y / TEXTURE_UV_COUNT.y) * screenSize.x * NUMBER_SCALE), m_nTime, GetPriority());

	return S_OK;
}

//****************************************************************
// タイマーの終了処理
//****************************************************************
void CTimer::Uninit(void)
{
	// 書き込み
	WriteFile();

	if (m_pNumber != nullptr)
	{// 数字オブジェクト
		m_pNumber->Uninit();
		m_pNumber = nullptr;
	}
	Release(); // 自分
}

//****************************************************************
// タイマーの更新処理
//****************************************************************
void CTimer::Update(void)
{
	m_counter += CManager::GetGameSpeed(); // カウント
	if (m_counter / FPS >= 1)
	{// 1秒経過
		switch (m_count)
		{
			// カウントしない
		case CTimer::COUNT::None:
			break;
			// カウントアップ
		case CTimer::COUNT::Up:
			++m_nTime;
			if (m_nTime>=m_nLimitTime)
			{
				m_timeOver = true;
			}
			break;
			// カウントダウン
		case CTimer::COUNT::Down:
			--m_nTime;
			if (m_nTime < 0)
			{
				m_timeOver = true;
			}
			break;
		}
		m_counter = 0;
	}

	if (m_pNumber != nullptr)
	{
		m_pNumber->SetNumber(m_nTime); // 表示切替
	}
}

//****************************************************************
// タイマーの描画処理
//****************************************************************
void CTimer::Draw(void)
{

}

//****************************************************************
// ファイル書き込み
//****************************************************************
void CTimer::WriteFile(void)
{
	CFile* pFile = new CFile(FILE_PATH);
	pFile->WriteBinary(m_timeOver);
	pFile->AddWriteBinary(m_nTime);
	delete pFile;
}