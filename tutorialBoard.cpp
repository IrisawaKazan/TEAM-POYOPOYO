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

	m_pBackground = CObject2D::Create({ SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f,0.0f }, VEC3_NULL, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, 7); // sato 変更

	m_pBoard = CObject2D::Create(m_pos, VEC3_NULL, { SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f }, 7);                                                  // sato 変更

	m_pBoard->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\tutorial_001.png"));
	m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.4f });
	m_pBoard->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Isprogress = true;

	// 現在のフレーム数 Misaki
	m_fCountFrame = 0.0f;
	// 最大フレーム数 Misaki
	m_fMaxFrame = 60.0f;
	// 次のボードがあるかどうか
	m_bNextBoard = true;
	// カウント
	m_nCount = 0;

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
		if (m_fCountFrame < m_fMaxFrame)
		{
			// カウントを進める
			m_fCountFrame++;

			// 目標位置から初期位置の差を求める
			D3DXVECTOR3 Diff = m_Dest - m_pos;

			// イージング
			float fEasing = EaseInOutSine(m_fCountFrame / m_fMaxFrame);

			// 初期位置に加算する
			Setpos = m_pos + (Diff * fEasing);

			// 位置を設定
			m_pBoard->SetPosition(Setpos);
		}

		if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_B) == true)
		{// 決定ボタンを押した時

			if (m_fCountFrame < m_fMaxFrame)
			{// 上がりきっていなかった場合 Misaki
				// フレームを最大にする
				m_fCountFrame = m_fMaxFrame - 1.0f;
			}
			else if (m_bNextBoard == true)
			{// 次のチュートリアルを表示する場合
				switch (m_nCount)
				{
				case 0:
					// チュートリアルを表示
					SetUp("data\\TEXTURE\\tutorial_006.png", true);

					m_nCount = 1;

					break;

				case 1:
					// チュートリアルを表示
					SetUp("data\\TEXTURE\\tutorial_007.png", false);

					m_nCount = 2;

					break;

				case 2:
					// チュートリアルを表示
					SetUp("data\\TEXTURE\\tutorial_005.png", false);

					m_nCount = 0;

					break;

				}

			}
			else
			{// 上がりきっていた場合 Misaki
				m_Isprogress = false;

				m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
				m_pBoard->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
			}
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

void CTutorialBoard::SetUp(std::string boardpath, const bool bNextBoard)
{
	if (CFade::GetFadeSingle()->GetFade() != CFade::FADE_NONE) return;

	m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.4f });
	m_pBoard->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });
	m_pBoard->SetTexIndx(CTextureManager::Instance()->Register(boardpath));

	m_Isprogress = true;

	if (m_bNextBoard == false)
	{// 次のチュートリアルがない場合
		// 位置を設定 Misaki
		m_pBoard->SetPosition(m_pos);

		// 現在のフレームを初期化
		m_fCountFrame = 0;
	}
	else
	{// 次のチュートリアルがある場合
		// 位置を設定 Misaki
		m_pBoard->SetPosition(m_Dest);
	}

	// 次があるかどうかを代入
	m_bNextBoard = bNextBoard;

}