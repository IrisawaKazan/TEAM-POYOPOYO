//=================================================
//
//	switch.h
//
// Author:chikada shouya
//
//=================================================
#ifndef _SWITCH_H_
#define _SWITCH_H_
#include "main.h"
#include "block.h"
//マクロ
#define HALF (0.5f)					// 半分の値

class CSwitch :public CBlock
{
public:
	struct Config {
		static constexpr float Depth = -8.5f;		// どれだけ沈むか(位置)
		static constexpr float PressSpeed = 0.05f; // 押される速度
		static constexpr float ReturnSpeed = 0.1f;// 戻る速度
	};

	// コンストラクタ・デストラクタ
	CSwitch(int nPriority = 3);
	~CSwitch();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// ゲッター
	bool IsPress(void) { return m_IsPressed; }

	// セッター
	void SetPressed(const bool Condition) { m_IsPressed = Condition; }

	// 静的メンバ関数
	// 生成
	static CSwitch* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
	bool m_IsPressed;		// 押されているかどうか
	D3DXVECTOR3 m_ApperPos;	// 初期位置s
};

// クラスを定義
class CDoor :public CBlock
{
public:
	struct Config {
		static constexpr float Limit = 100.0f;		// どれだけ沈むか(位置)
		static constexpr float UpSpeed = 0.5f; // 押される速度
		static constexpr float DownSpeed = 0.1f;// 戻る速度
	};

	// コンストラクタ・デストラクタ
	CDoor(int nPriority = 3);
	~CDoor();

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 起動
	void Begin(void) { m_IsFunc = true; }

	// シャットダウン
	void End(void) { m_IsFunc = false; }

	// 静的メンバ関数
	// 生成
	static CDoor* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
	bool m_IsFunc;			// 起動中かどうか
	D3DXVECTOR3 m_ApperPos;	// 初期位置
};
#endif