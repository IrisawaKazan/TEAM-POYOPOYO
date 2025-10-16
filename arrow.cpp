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
CArrow* CArrow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filePath, D3DXVECTOR2 size)
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
	pArrow->SetChengeSize(size.x);

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
	CObject3D::Draw();
}

//--------------------------------
// 角度を変える
//--------------------------------
void CArrow::ChengeAngle(D3DXVECTOR3* pos, D3DXVECTOR3* rot) const
{
	D3DXVECTOR3 space = *pos - GetPos();
	float length = D3DXVec3Length(&space);
	if (length < m_chengeSize)
	{
		*rot = GetRot();
	}
}