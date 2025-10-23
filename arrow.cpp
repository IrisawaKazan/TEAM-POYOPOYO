//--------------------------------
//
// やじるし [arrow.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "arrow.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CArrow* CArrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filePath, D3DXVECTOR2 size, size_t idx)
{
	// インスタンスの生成
	CArrow* pArrow = new CArrow;
	if (pArrow == nullptr)
	{
		return nullptr;
	}

	pArrow->SetFilePath(filePath);
	pArrow->SetSize(size);
	pArrow->SetPosition(pos);
	pArrow->SetRotasion(rot);
	pArrow->SetLength(D3DXVec2Length(&size));
	pArrow->SetChengeLength(D3DXVec2Length(&size) * CHENGE_LENGTH_MAGNIFICATION);
	pArrow->SetIdx(idx);

	// 初期化
	if (FAILED(pArrow->Init()))
	{
		delete pArrow;
		return nullptr;
	}
	return pArrow;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CArrow::Init(void)
{
	// 親クラスの初期化
	CObject3D::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CArrow::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CArrow::Update(void)
{
	// 親クラスの更新
	CObject3D::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CArrow::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f;                                  //Zバッファをカメラ方向にオフセットする値
	depthBias *= m_idx;                                            // オブジェクトID分だけオフセット
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

	CObject3D::Draw(); // 親クラスの描画

	// Depth Bias 設定を解除 sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// 角度を変える
//--------------------------------
void CArrow::ChengeAngle(const D3DXVECTOR3& pos, D3DXVECTOR3* rot) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float length = D3DXVec3Length(&space);
	if (length < m_chengeLength)
	{
		*rot = GetRot();
	}
}

//--------------------------------
// 矢印が近い
//--------------------------------
bool CArrow::ReleaseHit(const D3DXVECTOR3& pos, float length) const
{
	D3DXVECTOR3 space = pos - GetPos();
	float spaceLength = D3DXVec3Length(&space);
	return (spaceLength < (m_length + length));
}