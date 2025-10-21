//--------------------------------
//
// ナビゲーションUI [naviUI.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviUi.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "navi.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CNaviUI* CNaviUI::Create(const char* filePath, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNaviUI* pNaviUI = new CNaviUI;
	if (pNaviUI == nullptr)
	{
		return nullptr;
	}
	CTextureManager* pTexmanager = CTextureManager::Instance();
	pNaviUI->SetTexIndx(pTexmanager->Register(filePath));
	pNaviUI->SetSize(size);
	pNaviUI->SetPosition(pos);

	// 初期化
	if (FAILED(pNaviUI->Init()))
	{
		delete pNaviUI;
		return nullptr;
	}
	return pNaviUI;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviUI::Init(void)
{
	// 親クラスの初期化
	CObject2D::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviUI::Uninit(void)
{
	CObject2D::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNaviUI::Update(void)
{
	// 親クラスの更新
	CObject2D::Update();

	switch (CGame::GetNavi().GetDirection())
	{
	case CNavi::ARROW_DIRECTION::Left:
		SetTexIndx(CTextureManager::Instance()->Register("data/TEXTURE/UI/ArrowMark002.png"));
		break;
	case CNavi::ARROW_DIRECTION::Front:
		SetTexIndx(CTextureManager::Instance()->Register("data/TEXTURE/UI/ArrowMark001.png"));
		break;
	case CNavi::ARROW_DIRECTION::Back:
		SetTexIndx(CTextureManager::Instance()->Register("data/TEXTURE/UI/ArrowMark000.png"));
		break;
	}
}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviUI::Draw(void)
{
	CObject2D::Draw(); // 親クラスの描画
}