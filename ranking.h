//****************************************************************
//
// ランキングの処理[ranking.h]
// Author Kensaku Hatori
//
//****************************************************************
#pragma once

//  インクルード
#include "object.h"

class CNumber;   // 宣言
class CObject2D; //

//---------------------
//  ランキングクラス
//---------------------
class CRanking : CObject
{
public:
	CRanking(size_t timeCount, int priority) : CObject(priority), m_timeCount{ timeCount }, m_isColorFlag{}, m_counter{}, m_colCounter{}, m_pBrackboard{}, m_pNow{}, m_pRankings{}, m_pRankNumbers{}, m_pos{}, m_rankEndPos{}, m_isRankNoveEnds{} {}
	~CRanking() = default;

	static CRanking* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, size_t timeCount, int priority = 7);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

private:
	void SetMove();
	void SetRank();
	bool LoadFile(void);
	void WriteFile(void);
	void InitNum(void);
	void SetDefultFile();

	// 位置
	static constexpr float NOW_TIME_HEIGHT_OFFSET = -0.3f;       // 今の時間の高さのオフセット
	static constexpr float RANKING_START_HEIGHT_OFFSET = -0.15f; // ランキングの始まりの高さのオフセット

	// ランキング数と背景
	static constexpr const char* RANKING_FILE_PATH = "data/DATA/Ranking.bin";
	static constexpr size_t MAX_RANKING = 5u; // ランキング数
	static const D3DXCOLOR BOARD_COLOR;       // 背景カラー

	// 使う数字について
	static constexpr const char* TEXTURE_PATH = "data\\TEXTURE\\number001.png"; // テクスチャ
	static constexpr float NUMBER_SCALE = 0.0008f;                              // 大きさ
	static constexpr float NUMBER_MOVE_START_OFFSET = 0.5f;                     // 動き始めのオフセット
	static constexpr size_t NUMBER_MOVE_C = 120;                                // カラーカウント
	static constexpr size_t NUMBER_COLOR_C = 30;                                // カラーカウント
	static const D3DXCOLOR NUMBER_COLOR;                                        // カラー
	static const D3DXVECTOR2 TEXTURE_SIZE;                                      // テクスチャサイズ
	static const D3DXVECTOR2 TEXTURE_UV_COUNT;                                  // テクスチャ分割

	// 使う数字について (ランクナンバー)
	static constexpr const char* RN_TEXTURE_PATH = "data\\TEXTURE\\RankNum.png";  // テクスチャ
	static constexpr float RN_NUMBER_SCALE = 0.00016f;                            // 大きさ
	static constexpr float RN_NUMBER_WIDTH_OFFSET = 0.02f;                        // オフセット
	static const D3DXVECTOR2 RN_TEXTURE_SIZE;                                     // テクスチャサイズ
	static const D3DXVECTOR2 RN_TEXTURE_UV_COUNT;                                 // テクスチャ分割

	// Defaultランキング
	static constexpr std::array<int, MAX_RANKING> DEFAULT_FILE_DATA =
	{
		200,210,220,230,240
	};
	// 動く順番
	static constexpr std::array<size_t, MAX_RANKING + 1u> MOVE_ORDER =
	{
		5,4,3,2,1,0
	};

	const size_t m_timeCount;                                 // 1.秒 2.分:秒 3.時:分:秒
	CObject2D* m_pBrackboard;                                 // 黒板
	CNumber* m_pNow;		                                  // 今のタイムオブジェクト
	std::array<CNumber*, MAX_RANKING> m_pRankings;            // ランキングのタイムオブジェクト配列
	std::array<D3DXVECTOR3, MAX_RANKING + 1u> m_rankEndPos;   // 最終位置
	std::array<bool, MAX_RANKING + 1u> m_isRankNoveEnds;      // 動き終わったか?
	std::array<CNumber*, MAX_RANKING> m_pRankNumbers;         // ランキングの1,2,3,4,5配列
	size_t m_counter;                                         // カウンター
	size_t m_colCounter;                                      // カウンター     
	bool m_isColorFlag;                                       // 空フラグ
	D3DXVECTOR3 m_pos;				                          // 位置
	D3DXVECTOR2 m_size;				                          // 大きさ
};