//--------------------------------
//
// ジャンプマーカー [jump.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "jump.h"
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
CJump* CJump::Create(D3DXVECTOR3 pos, D3DXMATRIX rotMtx, float angle, const char* filePath, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CJump* pJump = new CJump;
	if (pJump == nullptr)
	{
		return nullptr;
	}

	pJump->SetType(CNavi::TYPE::Jump);
	pJump->SetTexIdx(CTextureManager::Instance()->Register(filePath));
	pJump->SetSize(size);
	pJump->SetPos(pos);
	pJump->SetRotMtx(rotMtx);
	pJump->SetAngle(angle);
	pJump->SetLength(D3DXVec2Length(&size) * RELEASE_LENGTH_MAGNIFICATION);
	pJump->SetChengeLength(D3DXVec2Length(&size) * CHENGE_LENGTH_MAGNIFICATION);

	// 初期化
	if (FAILED(pJump->Init()))
	{
		delete pJump;
		return nullptr;
	}
	return pJump;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CJump::Init(void)
{
	// 親クラスの初期化
	CNaviObject::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CJump::Uninit(void)
{
	CNaviObject::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CJump::Update(void)
{
	// 親クラスの更新
	CNaviObject::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CJump::Draw(void)
{
	CNaviObject::Draw(); // 親クラスの描画
}

//--------------------------------
// ナビゲーションアクション
//--------------------------------
void CJump::Activate(D3DXVECTOR3* outPos, float* outAngle) const
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