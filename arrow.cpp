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
CArrow* CArrow::Create(D3DXVECTOR3 pos, D3DXMATRIX rotMtx, float angle, const char* filePath, D3DXVECTOR2 size, size_t idx)
{
	// インスタンスの生成
	CArrow* pArrow = new CArrow;
	if (pArrow == nullptr)
	{
		return nullptr;
	}

	pArrow->SetType(CNavi::TYPE::Arrow);
	pArrow->SetTexIdx(CTextureManager::Instance()->Register(filePath));
	pArrow->SetSize(size);
	pArrow->SetPos(pos);
	pArrow->SetRotMtx(rotMtx);
	pArrow->SetAngle(angle);
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
void CArrow::Activate(float* angle) const
{
	if (angle != nullptr)
	{// nullでなければ
		*angle = GetAngle(); // 角度を渡す
	}
}