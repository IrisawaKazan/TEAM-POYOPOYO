//--------------------------------
//
// ナビゲーションオブジェクト [naviobject.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviobject.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

size_t CNaviObject::m_nextIdx = 0; // 次のインデックス (Trigger渡し用)

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviObject::Init(void)
{
	// 親クラスの初期化
	CObject3D::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviObject::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNaviObject::Update(void)
{
	// 親クラスの更新
	CObject3D::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f;                                  //Zバッファをカメラ方向にオフセットする値
	depthBias *= m_biasIdx;                                        // バイアスID分だけオフセット
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

	CObject3D::Draw(); // 親クラスの描画

	// Depth Bias 設定を解除 sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// 触れたら起動する
//--------------------------------
CNavi::Type CNaviObject::ActivateTrigger(const D3DXVECTOR3& pos, D3DXVECTOR3* rot, size_t* pIdx) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float length = D3DXVec3Length(&space);
	if (length < m_chengeLength)
	{
		Activate(rot);
		if (pIdx != nullptr)
		{
			*pIdx = m_idx;
		}
		return m_type;
	}
	else
	{
		return CNavi::Type::None;
	}
}

//--------------------------------
// 触れたら消す
//--------------------------------
bool CNaviObject::ReleaseTrigger(const D3DXVECTOR3& pos, float length) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float spaceLength = D3DXVec3Length(&space);
	return (spaceLength < (m_length + length));
}