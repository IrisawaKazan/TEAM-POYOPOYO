//****************************************************************
//
// ポーズマネージャーの処理[pausemanager.cpp]
// Author Kensaku Hatori
//
//****************************************************************

// インクルード
#include "pausemanager.h"
#include "manager.h"
#include "math_T.h"
#include "fade.h"
#include "game.h"
#include "tutorialBoard.h"

// 位置 sato Add
const D3DXVECTOR3 CPauseManager::Config::Bace = { 0.5f,0.4f,0.0f };

// シングルトンを宣言
CPauseManager* CPauseManager::m_Singleton = NULL;

//***************************************
// デストラクタ
//***************************************
CPauseManager::~CPauseManager()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CPauseManager::Init(void)
{
	m_SelectMenu = CPause::CONTINUE;
	D3DXVECTOR2 screenSize{};
	CManager::GetRenderer()->GetBackBufferSize(&screenSize);
	for (int nCount = 0; nCount < CPause::MAX; nCount++)
	{
		CPause* pPause = CPause::Create(D3DXVECTOR3(screenSize.x * Config::Bace.x, screenSize.y * (Config::Bace.y + Config::OffSet * nCount), 0.0f), (CPause::Menu)nCount);
		m_apPauseMenu.push_back(pPause);
	}
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CPauseManager::Uninit(void)
{
	if (m_Singleton != NULL)
	{
		m_apPauseMenu.clear();
		delete m_Singleton;
		m_Singleton = NULL;
	}
}

//***************************************
// 更新処理
//***************************************
void CPauseManager::Update(void)
{
	// サウンドの取得
	CSound* pSound = CManager::GetSound();

	if (CManager::GetCamera()->IsAnim() == false && CFade::GetFadeSingle()->GetFade() == CFade::FADE_NONE && CGame::GetTutorialBoard()->GetProgress() == false)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_P) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_START) == true)
		{
			// SE
			pSound->Play(CSound::LABEL_PAUSE_SE);

			CManager::ChangePause();
			m_SelectMenu = CPause::CONTINUE;
		}
	}

	if (CManager::isPause() == true)
	{
		if (CManager::GetInputKeyboard()->GetTrigger(DIK_W) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_UP) == true)
		{
			// SE
			pSound->Play(CSound::LABEL_SELECT_SE);

			m_SelectMenu = (CPause::Menu)Wrap(m_SelectMenu - 1, 0, (int)CPause::MAX - 1);
		}
		else if (CManager::GetInputKeyboard()->GetTrigger(DIK_S) == true || CManager::GetInputJoypad()->GetTrigger(CInputJoypad::JOYKEY_DOWN) == true)
		{
			// SE
			pSound->Play(CSound::LABEL_SELECT_SE);

			m_Singleton->m_SelectMenu = (CPause::Menu)Wrap(m_SelectMenu + 1, 0, (int)CPause::MAX - 1);
		}
		for (auto iter = m_apPauseMenu.begin(); iter != m_apPauseMenu.end(); iter++)
		{
			(*iter)->Update();
		}
	}
}

//***************************************
// シングルトンを生成
//***************************************
CPauseManager* CPauseManager::CreateSingleton(void)
{
	if (m_Singleton != NULL) return m_Singleton;
	m_Singleton = new CPauseManager;
	m_Singleton->Init();
	return m_Singleton;
}