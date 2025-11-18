//==============================================================
//
// [timer.h]
// Author: Irisawa Kazan
//
//==============================================================
#ifndef _TIMER_H_ // このマクロ定義がされてなかったら
#define _TIMER_H_ // 2重インクルード防止のマクロ定義

#include"main.h"
#include"object.h"

// 前方宣言
class CNumber;

// マクロ定義
#define MAX_TIMER (2)
#define MAX_TIMEOVER (10800)
#define MAX_HOUR (216000)

// タイマークラス
class CTimer : public CObject
{
public:

	CTimer(int nPriority = 7);
	~CTimer();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CTimer* Create(D3DXVECTOR3 pos);

	// セッター
	void SetPosition(D3DXVECTOR3 pos);
	void SubNs(int nValue);
	void SubMin(int nValue);

	// ゲッター
	D3DXVECTOR3 GetPos(void);
	int GetNs(void);
	int GetMin(void);
	int GetTime(void) { return m_nTime; };
	static int GetTimer(void) { return m_nTimer; };

	// インスタンス生成
	static CTimer* Instance(void) {
		static CTimer* pInstance = new CTimer;
		return pInstance;
	}

private:
	static CNumber* m_pNumber1[MAX_TIMER];	// ナンバーのポインタ(秒)
	static CNumber* m_pNumber2[MAX_TIMER];	// ナンバーのポインタ(分)
	static CNumber* m_pNumber3;				// ナンバーのポインタ(:)
	D3DXVECTOR3 m_pos;						// 位置
	int m_nTime;							// 値
	int m_nNs;								// 秒
	int m_nMin;								// 分
	int m_nHour;							// 時
	static int m_nTimer;					// タイマーの取得
	bool m_bFinish;							// 終了処理
};

#endif