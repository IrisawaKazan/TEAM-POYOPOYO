//****************************************************************
//
// タイトル背景の処理
// Author:Takahashi Misaki
//
//****************************************************************

// 二重インクルード防止
#ifndef _TITLEBG_H_
#define _TITLEBG_H_

// インクルード
#include "object2D.h"

// タイトル背景クラス
class CTitleBG : public CObject2D
{
public:
	// コンストラクタ・デストラクタ
	CTitleBG();
	~CTitleBG();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	static CTitleBG* Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, const D3DXVECTOR2 Size, const char* FilePath);

private:

};
#endif