//****************************************************************
//
// チュートリアルの処理
// Author:Takahashi Misaki
//
//****************************************************************

#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

// インクルード
#include "main.h"

// チュートリアルクラス
class CTutorial
{
public:

	typedef enum
	{
		TUTORIAL_NOEN = 0,	// なし
		TUTORIAL_LEFT,		// 左に進む指示
		TUTORIAL_JUMP,		// ジャンプする指示
		TUTORIAL_CLIMB,		// 壁をよじ登る指示
		TUTORIAL_MAX,		// 上限
	}TUTORIAL;

	// コンストラクタ・デストラクタ
	CTutorial();
	~CTutorial();

	// メンバ関数
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);
	void SetTutorial(const TUTORIAL type);		// チュートリアルの設定処理

	// 静的メンバ関数

private:

	// メンバ変数
	TUTORIAL m_type;	// チュートリアルの種類
	float m_fWidth;		// 横幅
	float m_fHeigth;	// 縦幅
};

#endif