//****************************************************************
//
// ランキングの処理[ranking.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _RANKING_H_
#define _RANKING_H_

//  インクルード
#include "object.h"
#include "object2D.h"
#include "number.h"

//	マクロ定義
#define MAX_TIME (2)
#define MAX_NUM (6)
#define MAX_SECOND (60)
#define MAX_MINUTES (3600)

//  ランキングクラスを定義
class CRanking
{
public:
	//	コンストラクタ・デストラクタ
	CRanking();
	~CRanking();

	//	メンバ関数
	static CRanking* Instance(void) {
		static CRanking* pInstance = new CRanking;
		return pInstance;
	}

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Sort(void);
	void Change(void);
	void LoadFile(void);
	void WriteFile(void);

	void InitNum(void);
private:
	CNumber* m_pNumber1[MAX_TIME][MAX_NUM];		// タイムの変数(秒)
	CNumber* m_pNumber2[MAX_TIME][MAX_NUM];		// タイムの変数(分)
	CNumber* m_pNumber3[MAX_NUM];				// タイムの変数(：)
	int m_nTime[MAX_NUM];						// 総時間
	int m_nMin[MAX_NUM];						// 分
	int m_nSec[MAX_NUM];						// 秒
	int m_nData;								// カラー用の数値の保存用変数
	int m_nMinutes;								// 現在の分の計算用変数
	int m_nSeconds;								// 現在の秒の計算用変数
	bool m_bAct;								// 発動したかどうか
};

#endif // !_RANKING_H_