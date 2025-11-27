//==============================================================
//
// 倍速UIの処理
// Author: Takahashi Misaki
//
//==============================================================
#ifndef _GAMESPEEDUI_H_
#define _GAMESPEEDUI_H_

// インクルード

// 前方宣言
class CObject2D;

class CGameSpeedUI
{
public:

	// コンストラクタ
	CGameSpeedUI();
	// デストラクタ
	~CGameSpeedUI();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);

	// セッター
	void SetSpeed(bool GameSpeed);

	// ゲッター
	bool GetSpeedUP(void) { return m_bSpeedUP; }

	// 静的メンバ関数
	static CGameSpeedUI* Create(void);

private:
	// メンバ変数
	CObject2D* m_GameSpeedN;		// 通常速度のテクスチャ
	CObject2D* m_GameSpeedUP[2];	// 倍速状態のテクスチャ
	bool m_bSpeedUP;				// 倍速しているかどうか
};

#endif