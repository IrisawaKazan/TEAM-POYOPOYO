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
	m_Isprogress = true;

	m_pBackground = CObject2D::Create({ 640.0f,360.0f,0.0f }, VEC3_NULL, { 640.0f,360.0f });
	m_pBackground->SetCol({ 0.0f,0.0f,0.0f,0.4f });

	m_pBoard = CObject2D::Create({ 640.0f,360.0f,0.0f }, VEC3_NULL, { 640.0f,360.0f });
	m_pBoard->SetTexIndx(CTextureManager::Instance()->Register("data\\TEXTURE\\tutorial_001.png"));

	return S_OK;
}

void CTutorialBoard::Uninit(void)
{
	Release();
}

//----------------------------------------
// 更新処理
//----------------------------------------
void CTutorialBoard::Update(void)
{
}	//m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.4f });
	//m_pBoard->SetCol({ 1.0f, 1.0f, 1.0f, 1.0f });

	//if (m_Isprogress == true)
	//{
	//	if (CManager::GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	//	{
	//		m_Isprogress = false;

	//		m_pBackground->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	//		m_pBoard->SetCol({ 0.0f, 0.0f, 0.0f, 0.0f });
	//	}
	//}


void CTutorialBoard::Draw(void)
{
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