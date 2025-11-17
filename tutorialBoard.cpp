//==============================================================
//
// [tutorialBoard.cpp]
// Author: Irisawa Kazan
//
//==============================================================
// インクルード
#include "tutorialBoard.h"
#include "object2D.h"
#include "input.h"
#include "fade.h"
#include "math_T.h"

//----------------------------------------
// コンストラクタ
//----------------------------------------
CTutorialBoard::CTutorialBoard()
{

}

//----------------------------------------
// デストラクタ
//----------------------------------------
CTutorialBoard::~CTutorialBoard()
{

}

//----------------------------------------
// 初期化処理
//----------------------------------------
HRESULT CTutorialBoard::Init(void)
{
	// 位置 Misaki
	m_Dest = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f };	// 目標位置
	m_pos = { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 1.5f,0.0f };	// 初期位置

	m_pBackground = CObject2D::Create({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f }, VEC3_NULL, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f });

	m_pBoard = CObject2D::Create(m_pos, VEC3_NULL, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f });

	m_pBoard->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\tutorial_001.png"));
	m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.4f });
	m_pBoard->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Isprogress = true;

	m_nCountFrame = 0;

	return S_OK;
}

//----------------------------------------
// 終了処理
//----------------------------------------
void CTutorialBoard::Uninit(void)
{
	delete this;
}

//----------------------------------------
// 更新処理
//----------------------------------------
void CTutorialBoard::Update(void)
{
	// イージングした後の位置
	D3DXVECTOR3 Setpos = {};

	if (m_Isprogress == true)
	{
		// イージングの処理　Misaki
		if (m_nCountFrame < 60.0f)
		{
			// カウントを進める
			m_nCountFrame++;

			// 目標位置から初期位置の差を求める
			D3DXVECTOR3 Diff = m_Dest - m_pos;

			// イージング
			float fEasing = EaseInOutSine((float)m_nCountFrame / 60.0f);

			// 初期位置に加算する
			Setpos = m_pos + (Diff * fEasing);

			// 位置を設定
			m_pBoard->SetPosition(Setpos);
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_B) == true)
		{
			m_Isprogress = false;

			m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
			m_pBoard->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
		}
	}

	CManager::SetTutorial(m_Isprogress);
}

//----------------------------------------
// 生成処理
//----------------------------------------
CTutorialBoard* CTutorialBoard::Create(void)
{
	CTutorialBoard* pTutorialBoard = nullptr;

	pTutorialBoard = new CTutorialBoard;

	if (pTutorialBoard != nullptr)
	{
		// オブジェクト設定
		pTutorialBoard->Init();

		return pTutorialBoard;
	}
	else
	{
		return nullptr;
	}
}

void CTutorialBoard::SetUp(std::string boardpath)
{
	if (CFade::GetFadeSingle()->GetFade() != CFade::FADE_NONE) return;

	m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.4f });
	m_pBoard->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });
	m_pBoard->SetTexIndx(CTextureManager::Instance()->Register(boardpath));

	m_Isprogress = true;

	// 位置を設定 Misaki
	m_pBoard->SetPosition(m_pos);

	// フレーム
	m_nCountFrame = 0;
}