//==============================================================
//
// [timer.h]
// Author: Irisawa Kazan
//
//==============================================================
#pragma once

#include"main.h"
#include"object.h"

// 前方宣言
class CNumber;

//-------------------
// タイマークラス
//-------------------
class CTimer : public CObject
{
public:
	static constexpr const char* FILE_PATH = "data/Goal.bin"; // 書き込みファイル

	// タイプ
	enum class COUNT : unsigned char
	{
		None,
		Up,
		Down,
		Max
	};

	CTimer(size_t timeCount, int nPriority) : CObject(nPriority), m_timeCount{ timeCount }, m_pNumber{}, m_pos{}, m_nTime{}, m_timeOver{}, m_count{}, m_counter{} {}
	~CTimer() = default;

	static CTimer* Create(D3DXVECTOR3 pos, size_t timeCount, int startTime = 0, int limitTime = 60, int nPriority = 7);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void WriteFile(void);

	// セッター
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };
	void SetCount(COUNT count) { m_count = count; m_counter = 0; }

	// ゲッター
	bool IsTimeOver() { return m_timeOver; }
	D3DXVECTOR3 GetPos(void) { return m_pos; };
	COUNT GetCount() { return m_count; }

private:
	static constexpr const char* TEXTURE_PATH = "data\\TEXTURE\\number001.png"; // テクスチャ
	static constexpr float NUMBER_SCALE = 0.0005f;                              // 大きさ
	static const D3DXVECTOR2 TEXTURE_SIZE;                                      // テクスチャサイズ
	static const D3DXVECTOR2 TEXTURE_UV_COUNT;                                  // テクスチャ分割

	void SetStartTime(int time) { m_nTime = time; }
	void SetLimitTime(int time) { m_nLimitTime = time; }

	const size_t m_timeCount; // 1.秒 2.分:秒 3.時:分:秒
	CNumber* m_pNumber;	      // ナンバーのポインタ
	D3DXVECTOR3 m_pos;	      // 位置
	int m_nTime;		      // 秒数
	int m_nLimitTime;         // 秒数
	bool m_timeOver;          // クリア時の書き込み用変数
	COUNT m_count;            // カウント方式
	size_t m_counter;         // カウンター
};