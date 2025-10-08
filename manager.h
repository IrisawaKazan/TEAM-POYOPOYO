//*************************************
//
//　Manager[manager.h]
//　Author:Hatori Kensaku
//
//*************************************

// 二重インクルード防止
#ifndef _MANAGER_H_
#define _MANAGER_H_

// インクルード
#include "main.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "object.h"
#include "texturemanager.h"
#include "modelmanager.h"

// 前方宣言
class CObject3D;
class CShaderFade;

// クラスを定義
class CManager
{
public:
	// 規定値
	struct Config {
		struct FilePath {
			static constexpr const char* TexList = "data\\TEXT\\TexFileList.txt";		// テクスチャリスト
			static constexpr const char* ModelList = "data\\TEXT\\ModelList.txt";		// モデルリスト
		};
	};

	// コンストラクタ・デストラクタ
	CManager();
	~CManager();

	// メンバ関数
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWnd);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 静的メンバ関数
	// セッター
	static void ChangePause(void) { m_isPause = !m_isPause; };
	static void OffPause(void) { m_isPause = false; };
	static void OnPause(void) { m_isPause = true; };
	static void SetClear(const bool isClear) { m_isClear = isClear; };

	// ゲッター
	static CRenderer* GetRenderer(void) { return m_Renderer; };
	static CInputKeyboard* GetInputKeyboard(void) { return m_pInputKeyboard; };
	static CInputJoypad* GetInputJoypad(void) { return m_pInputJoypad; };
	static CInputMouse* GetInputMouse(void) { return m_pInputMouse; };
	static CSound* GetSound(void) { return m_pSound; };
	static CCamera* GetCamera(void) { return m_pCamera; };
	static bool GetIsClear(void) { return m_isClear; };
	static CShaderFade* GetShaderFade(void) { return m_pFadeShader; };
	static CLight* GetLight(void) { return m_pLight; };

	// プレイヤーをリスポーンさせる
	static void RespawPlayer(void);

	// 条件式の関数か
	static bool isPause(void) { return m_isPause; };
private:
	// 静的メンバ変数
	static CRenderer* m_Renderer;				// レンダラー生成用
	static CInputKeyboard* m_pInputKeyboard;	// キーボードのインスタンス
	static CInputJoypad* m_pInputJoypad;		// ゲームパッドのインスタンス
	static CInputMouse* m_pInputMouse;			// マウスのインスタンス
	static CSound* m_pSound;					// 音のインスタンス
	static CCamera* m_pCamera;					// カメラのインスタンス
	static CLight* m_pLight;					// ライトのインスタンス
	static CShaderFade* m_pFadeShader;			// シェーダフェード
	static bool m_isPause;						// ポーズ中かどうか
	static bool m_isClear;						// 敵を全滅させたかどうか
};
#endif // !_MANAGER_H_