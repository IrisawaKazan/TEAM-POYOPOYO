//****************************************************************
//
// リザルト処理[title.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _RESULT_H_
#define _RESULT_H_

// インクルード
#include "scene.h"

//	リザルトクラスを定義
class CResult	:public CScene
{
public:
	//	コンストラクタ・デストラクタラクタ
	CResult();
	~CResult();

	//	メンバ関数
	HRESULT Init(void)	override;
	void Update(void)	override;
	void Uninit(void)	override;
	void Draw(void)		override;

private:


};

#endif // !_RESULT_H_


