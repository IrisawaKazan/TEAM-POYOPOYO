//==============================================================
//
// 倍速UIの処理
// Author: Takahashi Misaki
//
//==============================================================
// インクルード
#include "gemespeedUI.h"
#include "object2D.h"
#include "input.h"

//----------------------------------------
// コンストラクタ
//----------------------------------------
CGameSpeedUI::CGameSpeedUI()
{

}

//----------------------------------------
// デストラクタ
//----------------------------------------
CGameSpeedUI::~CGameSpeedUI()
{

}

//----------------------------------------
// 初期化処理
//----------------------------------------
HRESULT CGameSpeedUI::Init(void)
{
	// スクリーンの大きさ
	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);

	// 位置
	D3DXVECTOR3 pos = D3DXVECTOR3(screenSize.x * 0.9f, screenSize.y * 0.07f, 0.0f);

	// 通常速度状態のテクスチャ
	m_GameSpeedN = CObject2D::Create(pos, VEC3_NULL, { screenSize.x * 0.04f,screenSize.y * 0.04f }, 7);
	m_GameSpeedN->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\speedUP.png"));
	m_GameSpeedN->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

	// 倍速状態のテクスチャ
	m_GameSpeedUP[0] = CObject2D::Create(pos - D3DXVECTOR3(screenSize.x * 0.04f, 0.0f, 0.0f), VEC3_NULL, { screenSize.x * 0.04f,screenSize.y * 0.04f }, 7);
	m_GameSpeedUP[0]->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\speedUP.png"));
	m_GameSpeedUP[0]->SetCol({ 1.0f, 1.0f, 1.0f, 0.0f });

	m_GameSpeedUP[1] = CObject2D::Create(pos + D3DXVECTOR3(screenSize.x * 0.04f, 0.0f, 0.0f), VEC3_NULL, { screenSize.x * 0.04f,screenSize.y * 0.04f }, 7);
	m_GameSpeedUP[1]->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\speedUP.png"));
	m_GameSpeedUP[1]->SetCol({ 1.0f, 1.0f, 1.0f, 0.0f });

	m_bSpeedUP = false;

	return S_OK;
}

//----------------------------------------
// 終了処理
//----------------------------------------
void CGameSpeedUI::Uninit(void)
{
	delete this;
}

//----------------------------------------
// 設定処理
//----------------------------------------
void CGameSpeedUI::SetSpeed(bool GameSpeed)
{
	if (GameSpeed == false)
	{// 通常速度の場合

		// 不透明にする
		m_GameSpeedN->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 透明にする
		m_GameSpeedUP[0]->SetCol({ 1.0f, 1.0f, 1.0f, 0.0f });
		m_GameSpeedUP[1]->SetCol({ 1.0f, 1.0f, 1.0f, 0.0f });

		// 通常速度状態にする
		m_bSpeedUP = false;
	}
	else if (GameSpeed == true)
	{// 倍速の場合

		// 透明にする
		m_GameSpeedN->SetCol({ 1.0f, 1.0f, 1.0f, 0.0f });

		// 不透明にする
		m_GameSpeedUP[0]->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });
		m_GameSpeedUP[1]->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

		// 倍速状態にする
		m_bSpeedUP = true;
	}

}


//----------------------------------------
// 生成処理
//----------------------------------------
CGameSpeedUI* CGameSpeedUI::Create(void)
{
	// ポインタ
	CGameSpeedUI* pGameSpeedUI = nullptr;

	// 生成
	pGameSpeedUI = new CGameSpeedUI;

	if (pGameSpeedUI != nullptr)
	{
		// 初期化処理
		pGameSpeedUI->Init();

		return pGameSpeedUI;
	}
	else
	{
		return nullptr;
	}
}

