//****************************************************************
//
// レンダー処理[renderer.h]
// Author Kensaku Hatori
//
//****************************************************************

// 二重インクルード防止
#ifndef _CRENDERER_H_
#define _CRENDERER_H_

// インクルード
#include "main.h"

// 前方宣言
class CDebugProc;

// レンダーの基本クラスと定義
class CRenderer
{
// パブリック
public:
	// コンストラクタ・デストラクタ
	CRenderer();
	~CRenderer();

	// メンバ関数
	HRESULT Init(HWND hWnd,bool bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// セッター
	void onWireFrame();
	void offWireFrame();
	void ToggleFullscreen(HWND hWnd, bool* isFullScrean);
	void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
	void SetSize(UINT W, UINT H) { m_Width = W, m_Height = H; }

	// ゲッター
	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }
	// マルチテクスチャ用
	//LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_pTextureMT; };
	// ウィンドウのサイズを取得
	RECT GetWindowsRect(void) { return m_RectWindow; }
	bool NeedReset(void) { return (m_Width != NULL && m_Height != NULL); }
// プライベート
private:
	// 静的メンバ変数
	static CDebugProc* m_pDebugProc;	// デバッグ表示用
	// メンバ変数
	LPDIRECT3D9 m_pD3D;
	LPDIRECT3DDEVICE9 m_pD3DDevice;						// デバイス
	RECT m_RectWindow;									// ウィンドウの大きさ
	D3DPRESENT_PARAMETERS m_d3dpp;						// プレゼンテーションパラメータ
	UINT m_Width;										// Imgui用のWindowの大きさ(W)
	UINT m_Height;										// Imgui用のWindowの大きさ(H)
};
#endif // !_CRENDERER_H_