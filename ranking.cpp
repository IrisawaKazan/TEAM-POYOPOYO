//****************************************************************
//
// ランキングの処理[ranking.cpp]
// Author Kensaku Hatori
//
//****************************************************************
#include "ranking.h"
#include "object2D.h"
#include "number.h"
#include "timer.h"
#include "file.h"

const D3DXCOLOR CRanking::BOARD_COLOR = { 0.0f, 0.0f, 0.0f, 0.5f }; // 背景カラー

const D3DXVECTOR2 CRanking::TEXTURE_SIZE = { 566,80 };   // テクスチャサイズ
const D3DXVECTOR2 CRanking::TEXTURE_UV_COUNT = { 11,0 }; // テクスチャ分割

const D3DXVECTOR2 CRanking::RN_TEXTURE_SIZE = { 1309,218 }; // テクスチャサイズ
const D3DXVECTOR2 CRanking::RN_TEXTURE_UV_COUNT = { 10,0 }; // テクスチャ分割

//****************************************************************
// 生成
//****************************************************************
CRanking* CRanking::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, size_t timeCount, int priority)
{
	// インスタンスの生成
	CRanking* pRanking = new CRanking(timeCount, priority); // 表示桁数
	if (pRanking == nullptr)
	{
		return nullptr;
	}

	pRanking->SetPos(pos);   // 位置
	pRanking->SetSize(size); // 大きさ

	// 初期化
	if (FAILED(pRanking->Init()))
	{
		delete pRanking;
		return nullptr;
	}
	return pRanking;
}

//****************************************************************
// 初期化
//****************************************************************
HRESULT CRanking::Init(void)
{
	// ナンバーの初期化
	InitNum();

	// 読み込み
	LoadFile();

	// ランキング整理
	SetRank();

	// 書き込み
	WriteFile();

	// 黒ポリゴン
	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);
	m_pBrackboard = CObject2D::Create(m_pos, VEC3_NULL, m_size);
	m_pBrackboard->SetCol(BOARD_COLOR);

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
	for (auto& pRanking : m_pRankings)
	{// ランキング
		if (pRanking != nullptr)
		{
			pRanking->Uninit();
			pRanking = nullptr;
		}
	}
	if (m_pNow != nullptr)
	{// 今
		m_pNow->Uninit();
		m_pNow = nullptr;
	}
	if (m_pBrackboard != nullptr)
	{// 黒板
		m_pBrackboard->Uninit();
		m_pBrackboard = nullptr;
	}
	Release(); // 自分
}

//****************************************************************
// 更新
//****************************************************************
void CRanking::Update(void)
{

}

//****************************************************************
// 描画
//****************************************************************
void CRanking::Draw(void)
{

}

//****************************************************************
// ランキング整理
//****************************************************************
void CRanking::SetRank()
{
	std::array<int, MAX_RANKING + 1u> rankDatas{}; // 数値抽出用配列
	for (size_t cnt = 0; cnt < m_pRankings.size(); cnt++)
	{// 数値の抽出
		rankDatas[cnt] = m_pRankings[cnt]->GetNumber();
	}
	rankDatas[MAX_RANKING] = m_pNow->GetNumber();    // 今の時間を追加

	// ソート(r(リバースイテレータ)をつけることで降順にできる)
	std::sort(rankDatas.rbegin(), rankDatas.rend());

	for (size_t cnt = 0; cnt < m_pRankings.size(); cnt++)
	{// 数値を戻す
		m_pRankings[cnt]->SetNumber(rankDatas[cnt]);
	}
}

//****************************************************************
// ファイル読み込み
//****************************************************************
void CRanking::LoadFile(void)
{
	CFile* pFile = new CFile(CTimer::FILE_PATH);
	const auto nowData = pFile->ReadBinary<int>(sizeof(bool));
	if (nowData.has_value())
	{
		m_pNow->SetNumber(nowData.value());
	}

	pFile->ChangeFile(RANKING_FILE_PATH);
	for (size_t cnt = 0; cnt < m_pRankings.size(); ++cnt)
	{
		const auto data = pFile->ReadBinary<int>(sizeof(int) * cnt);
		if (data.has_value())
		{
			m_pRankings[cnt]->SetNumber(data.value());
		}
	}
	delete pFile;
}

//****************************************************************
// ファイル書き込み
//****************************************************************
void CRanking::WriteFile(void)
{
	CFile* pFile = new CFile(RANKING_FILE_PATH);
	for (size_t cnt = 0;cnt<m_pRankings.size();++cnt)
	{
		if (!pFile->AddWriteBinary<int>(m_pRankings[cnt]->GetNumber()))break;
	}
	delete pFile;
}

//****************************************************************
// ナンバー初期化
//****************************************************************
void CRanking::InitNum(void)
{
	// スクリーンのサイズ
	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);

	// 数字サイズ
	D3DXVECTOR2 size{ D3DXVECTOR2((TEXTURE_SIZE.x / TEXTURE_UV_COUNT.x) * screenSize.x * NUMBER_SCALE, (TEXTURE_SIZE.y / TEXTURE_UV_COUNT.y) * screenSize.x * NUMBER_SCALE) };
	D3DXVECTOR2 rnSize{ D3DXVECTOR2((RN_TEXTURE_SIZE.x / RN_TEXTURE_UV_COUNT.x) * screenSize.x * RN_NUMBER_SCALE, (RN_TEXTURE_SIZE.y / RN_TEXTURE_UV_COUNT.y) * screenSize.x * RN_NUMBER_SCALE) };

	// 今の時間
	m_pNow = CNumber::Create(m_timeCount, CNumber::TYPE::Time, TEXTURE_PATH, TEXTURE_UV_COUNT, m_pos + D3DXVECTOR3(0.0f, NOW_TIME_HEIGHT_OFFSET * screenSize.y, 0.0f), size, 0, GetPriority());

	// ランキング時間
	size_t cnt{};
	for (auto& pRanking : m_pRankings)
	{
		pRanking = CNumber::Create(m_timeCount, CNumber::TYPE::Time, TEXTURE_PATH, TEXTURE_UV_COUNT, m_pos + D3DXVECTOR3(0.0f, RANKING_START_HEIGHT_OFFSET * screenSize.y+ size.y * cnt, 0.0f), size, 0, GetPriority());
		++cnt;
	}

	// ランキングナンバー
	cnt = 0;
	for (auto& pRankNumber : m_pRankNumbers)
	{
		pRankNumber = CNumber::Create(1u, CNumber::TYPE::Normal, RN_TEXTURE_PATH, RN_TEXTURE_UV_COUNT, m_pRankings[cnt]->GetLeftPos() + D3DXVECTOR3(-rnSize.x * 0.5f, 0.0f, 0.0f), rnSize, cnt + 1, GetPriority());
		++cnt;
	}
}