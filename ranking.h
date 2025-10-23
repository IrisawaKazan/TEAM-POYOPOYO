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

//  ランキングクラスを定義
class CRanking
{
public:
	//	コンストラクタ・デストラクタ
	CRanking();
	~CRanking();

	//	メンバ関数
	static CRanking* Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:


};

#endif // !_RANKING_H_