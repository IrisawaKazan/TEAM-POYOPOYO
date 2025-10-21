////==============================================================
////
//// [timer.h]
//// Author: Irisawa Kazan
////
////==============================================================
//#ifndef _TIMER_H_ // このマクロ定義がされてなかったら
//#define _TIMER_H_ // 2重インクルード防止のマクロ定義
//
//#include"main.h"
//#include"object.h"
//
//// 前方宣言
//class CNumber;
//
//// マクロ定義
//#define MAX_TIMER (3)
//
//// タイマークラス
//class CTimer : public CObject
//{
//public:
//	CTimer(int nPriority = 7);
//	~CTimer();
//
//	static CTimer* Create(D3DXVECTOR3 pos, float xsize, float ysize);
//
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//
//	void SetPosition(D3DXVECTOR3 pos);
//	D3DXVECTOR3 GetPos(void);
//
//	void AddTimer(void);
//
//private:
//	D3DXVECTOR3 m_pos;
//	float m_fxSize;
//	float m_fySize;
//	static CNumber* m_apNumber[MAX_TIMER];
//	int m_nTimer;
//	int m_nContTimer;
//};
//
//#endif