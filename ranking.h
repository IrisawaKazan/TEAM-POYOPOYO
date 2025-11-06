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
#define MAX_NUMBER (2)

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

private:
	int m_nMin;	// 分
	int m_nSec;	// 秒
};

#endif // !_RANKING_H_