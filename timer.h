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

// タイマークラス
class CTimer : public CObject
{
public:
	CTimer(int nPriority = 7);
	~CTimer();

	static CTimer* Create(D3DXVECTOR3 pos);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);

	void SetTime(int nTime);
	void SubNs(int nValue);
	void SubMin(int nValue);

	int GetNs(void);
	int GetMin(void);
	static int GetTime(void) { return m_nTimer; }

private:
	static CNumber* m_pNumber1[MAX_TIMER];	// ナンバーのポインタ(秒)
	static CNumber* m_pNumber2[MAX_TIMER];	// ナンバーのポインタ(分)
	static CNumber* m_pNumber3;				// ナンバーのポインタ(:)
	D3DXVECTOR3 m_pos;						// 位置
	int m_nTime;							// 値
	int m_nNs;								// 秒
	int m_nMin;								// 分
	static int m_nTimer;					// タイマーの取得
};

#endif