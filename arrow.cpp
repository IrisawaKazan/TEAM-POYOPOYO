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

	pArrow->SetType(CNavi::Type::Arrow);
	pArrow->SetFilePath(filePath);
	pArrow->SetSize(size);
	pArrow->SetPosition(pos);
	pArrow->SetRotasion(rot);
	pArrow->SetLength(D3DXVec2Length(&size));
	pArrow->SetChengeLength(D3DXVec2Length(&size) * CHENGE_LENGTH_MAGNIFICATION);
	pArrow->SetBiasIdx(idx);

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
	CNaviObject::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CArrow::Uninit(void)
{
	CNaviObject::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CArrow::Update(void)
{
	// 親クラスの更新
	CNaviObject::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CArrow::Draw(void)
{
	CNaviObject::Draw(); // 親クラスの描画
}

//--------------------------------
// ナビゲーションアクション
//--------------------------------
void CArrow::Activate(D3DXVECTOR3* rot) const
{
	if (rot != nullptr)
	{// nullでなければ
		*rot = GetRot(); // 角度を渡す
	}
}