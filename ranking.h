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
	CRanking(size_t timeCount, int priority) : CObject(priority), m_timeCount{ timeCount }, m_pBrackboard{}, m_pNow{}, m_pRankings{}, m_pRankNumbers{}, m_pos{}, m_Dest{} {}
	~CRanking() = default;

	static CRanking* Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, size_t timeCount, int priority = 7);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

private:
	void SetRank();
	void LoadFile(void);
	void WriteFile(void);
	void InitNum(void);

	// 位置
	static constexpr float NOW_TIME_HEIGHT_OFFSET = -0.25f;     // 今の時間の高さのオフセット
	static constexpr float RANKING_START_HEIGHT_OFFSET = -0.1f; // ランキングの始まりの高さのオフセット

	// ランキング数と背景
	static constexpr const char* RANKING_FILE_PATH = "data/Ranking.bin";
	static constexpr size_t MAX_RANKING = 5u; // ランキング数
	static const D3DXCOLOR BOARD_COLOR;       // 背景カラー

	// 使う数字について
	static constexpr const char* TEXTURE_PATH = "data\\TEXTURE\\number001.png"; // テクスチャ
	static constexpr float NUMBER_SCALE = 0.1f;                                 // 大きさ
	static const D3DXVECTOR2 TEXTURE_SIZE;                                      // テクスチャサイズ
	static const D3DXVECTOR2 TEXTURE_UV_COUNT;                                  // テクスチャ分割

	// 使う数字について (ランクナンバー)
	static constexpr const char* RN_TEXTURE_PATH = "data\\TEXTURE\\RankNum.png";  // テクスチャ
	static constexpr float RN_NUMBER_SCALE = 0.1f;                                // 大きさ
	static const D3DXVECTOR2 RN_TEXTURE_SIZE;                                     // テクスチャサイズ
	static const D3DXVECTOR2 RN_TEXTURE_UV_COUNT;                                 // テクスチャ分割

	const size_t m_timeCount;                         // 1.秒 2.分:秒 3.時:分:秒
	CObject2D* m_pBrackboard;                         // 黒板
	CNumber* m_pNow;		                          // 今のタイムオブジェクト
	std::array<CNumber*, MAX_RANKING> m_pRankings;    // ランキングのタイムオブジェクト配列
	std::array<CNumber*, MAX_RANKING> m_pRankNumbers; // ランキングの1,2,3,4,5配列
	D3DXVECTOR3 m_pos;				                  // 位置
	D3DXVECTOR3 m_Dest;				                  // 目標の位置
	D3DXVECTOR2 m_size;				                  // 大きさ
	//int m_nData;					                  // カラー用の数値の保存用変数
	//int m_nRankIdx;					              // アニメーション中のランキングのインデックス
	//int m_nAnimCount;				                  // モーションカウンタ
	//int m_nCoolDownCount;			                  // クールダウン
	//bool m_bAct;					                  // 発動したかどうか
	//int m_nNum;						              // 総数分の計算用変数
};