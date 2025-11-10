//--------------------------------
//
// クライムマーカー [climb.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "climb.h"
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
CClimb* CClimb::Create(D3DXVECTOR3 pos, D3DXMATRIX rotMtx, float angle, const char* filePath, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CClimb* pClimb = new CClimb;
	if (pClimb == nullptr)
	{
		return nullptr;
	}

	pClimb->SetType(CNavi::TYPE::Climb);
	pClimb->SetTexIdx(CTextureManager::Instance()->Register(filePath));
	pClimb->SetSize(size);
	pClimb->SetPos(pos);
	pClimb->SetRotMtx(rotMtx);
	pClimb->SetAngle(angle);
	pClimb->SetLength(D3DXVec2Length(&size));
	pClimb->SetChengeLength(D3DXVec2Length(&size) * CHENGE_LENGTH_MAGNIFICATION);

	// 初期化
	if (FAILED(pClimb->Init()))
	{
		delete pClimb;
		return nullptr;
	}
	return pClimb;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CClimb::Init(void)
{
	// 親クラスの初期化
	CNaviObject::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CClimb::Uninit(void)
{
	CNaviObject::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CClimb::Update(void)
{
	// 親クラスの更新
	CNaviObject::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CClimb::Draw(void)
{
	CNaviObject::Draw(); // 親クラスの描画
}

//--------------------------------
// ナビゲーションアクション
//--------------------------------
void CClimb::Activate(D3DXVECTOR3* outPos, float* outAngle) const
{
	if (outPos != nullptr)
	{// nullでなければ
		*outPos = GetPos(); // 角度を渡す
	}
	if (outAngle != nullptr)
	{// nullでなければ
		*outAngle = GetAngle(); // 角度を渡す
	}
}