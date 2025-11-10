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
	m_pBackground = CObject2D::Create({ 640.0f,360.0f,0.0f }, VEC3_NULL, { 640.0f,360.0f });

	m_pBoard = CObject2D::Create({ 640.0f,360.0f,0.0f }, VEC3_NULL, { 640.0f,360.0f });

	m_pBoard->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\tutorial_001.png"));
	m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.4f });
	m_pBoard->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_Isprogress = true;
	return S_OK;
}

void CTutorialBoard::Uninit(void)
{
	delete this;
}

//----------------------------------------
// 更新処理
//----------------------------------------
void CTutorialBoard::Update(void)
{
	if (m_Isprogress == true)
	{
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
}