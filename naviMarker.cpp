//--------------------------------
//
// ナビゲーションマーカー [naviMarker.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviMarker.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CNaviMarker* CNaviMarker::Create(const char* filePath, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNaviMarker* pNaviMarker = new CNaviMarker;
	if (pNaviMarker == nullptr)
	{
		return nullptr;
	}

	pNaviMarker->SetFilePath(filePath);
	pNaviMarker->SetSize(size);

	// 初期化
	if (FAILED(pNaviMarker->Init()))
	{
		delete pNaviMarker;
		return nullptr;
	}
	return pNaviMarker;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviMarker::Init(void)
{
	CObject3D::Init();
	m_nBiasID = 0;
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviMarker::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNaviMarker::Update(void)
{
	CObject3D::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviMarker::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f;                                  //Zバッファをカメラ方向にオフセットする値
	depthBias *= m_nBiasID;                                        // バイアスID分だけオフセット
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

	CObject3D::Draw(); // 親クラスの描画

	// Depth Bias 設定を解除 sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}