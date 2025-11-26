//****************************************************************
//
// タイトルマネジャーの処理[titlemanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "titlemanager.h"
#include "manager.h"
#include "math_T.h"
#include "sound.h"

// 規定値を設定
const D3DXVECTOR3 CTitleManager::Config::Menu::Bace = { 0.2f,0.65f,0.0f };
const D3DXVECTOR3 CTitleManager::Config::Logo::Apper = { 0.2f,0.0f,0.0f };
const D3DXVECTOR3 CTitleManager::Config::Logo::Dest = { 0.2f,0.27f,0.0f };
const D3DXVECTOR2 CTitleManager::Config::Logo::Size = { 0.17f,0.12f };
const D3DXVECTOR3 CTitleManager::Config::BG::Apper = { 0.5f,0.5f,0.0f };	// Misaki
const D3DXVECTOR2 CTitleManager::Config::BG::Size = { 0.5f,0.28f };			// Misaki

// シングルトンを宣言
CTitleManager* CTitleManager::m_Singleton = NULL;

//***************************************
// デストラクタ
//***************************************
CTitleManager::~CTitleManager()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CTitleManager::Init(void)
{
	// スクリーンのサイズ
	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);

	// 選んでいつメニュー
	m_SelectMenu = CTitleMenu::START;

	// メニューを一気に生成
	for (int nCount = 0; nCount < CTitleMenu::MAX; nCount++)
	{
		D3DXVECTOR3 menuPos = D3DXVECTOR3(CTitleManager::Config::Menu::Bace.x, CTitleManager::Config::Menu::Bace.y + CTitleManager::Config::Menu::OffSet * nCount, 0.0f);
		D3DXVECTOR3 screendMenuPos = D3DXVECTOR3(menuPos.x * screenSize.x, menuPos.y * screenSize.y, 0.0f);
		CTitleMenu* pTitleMenu = CTitleMenu::Create(screendMenuPos, (CTitleMenu::Menu)nCount);
		m_apTitleMenu.push_back(pTitleMenu);
	}

	// ロゴを生成
	D3DXVECTOR3 logoApper = D3DXVECTOR3(CTitleManager::Config::Logo::Apper.x * screenSize.x, CTitleManager::Config::Logo::Apper.y * screenSize.y, 0.0f);
	m_TitleLogo = CTitleLogo::Create(logoApper, VEC3_NULL, CTitleManager::Config::Logo::Size * screenSize.x, CTitleManager::Config::Logo::FilePath);

	// タイトル背景を生成
	D3DXVECTOR3 bgApper = D3DXVECTOR3(CTitleManager::Config::BG::Apper.x * screenSize.x, CTitleManager::Config::BG::Apper.y * screenSize.y, 0.0f);
	m_TitleBG = CTitleBG::Create(bgApper, VEC3_NULL, CTitleManager::Config::BG::Size * screenSize.x, CTitleManager::Config::BG::FilePath);

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CTitleManager::Uninit(void)
{
	// シングルトンの終了処理
	if (m_Singleton != NULL)
	{
		m_apTitleMenu.clear();
		m_Singleton = NULL;
	}

	// 自分自身の破棄
	Release();
}

//***************************************
// 更新処理
//***************************************
void CTitleManager::Update(void)
{
	// サウンドの取得
	CSound* pSound = CManager::GetSound();

	// メニューを選ぶ
	if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true || CManager::GetInputMouse()->GetMouseState().lZ < 0 ||
		CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_UP) == true)
	{
		m_SelectMenu = (CTitleMenu::Menu)Wrap(m_SelectMenu - 1, 0, (int)CTitleMenu::MAX - 1);

		// SE
		pSound->Play(CSound::LABEL_SELECT_SE);
	}
	else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true || CManager::GetInputMouse()->GetMouseState().lZ > 0 ||
		CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_DOWN) == true)
	{
		m_SelectMenu = (CTitleMenu::Menu)Wrap(m_SelectMenu + 1, 0, (int)CTitleMenu::MAX - 1);

		// SE
		pSound->Play(CSound::LABEL_SELECT_SE);
	}
}

//***************************************
// シングルトンを生成
//***************************************
CTitleManager* CTitleManager::CreateSingleton(void)
{
	// シングルトンが生成されていたら
	if (m_Singleton != NULL) return m_Singleton;
	// メモリ確保
	m_Singleton = new CTitleManager;
	// 初期化処理
	m_Singleton->Init();
	// 返す
	return m_Singleton;
}