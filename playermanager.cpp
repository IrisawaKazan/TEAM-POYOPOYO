//************************************************************
//
//　プレイヤーの処理[playermanager.cpp]
//　Author:chikada shouya
//
//************************************************************

// インクルード
#include "playermanager.h"
#include "manager.h"

// 静的メンバ変数の定義
const D3DXVECTOR3 CPlayerManager::SPAWN_POS = D3DXVECTOR3(-1900.0f, 0.0f, 0.0f);              // NPCの出現位置 sato Add
const D3DXVECTOR3 CPlayerManager::SPAWN_ROT = D3DXVECTOR3(0.0f, D3DXToRadian(-90.0f), 0.0f); // NPCの出現回転 sato Add

//************************************************************
// コンストラクタ
//************************************************************
CPlayerManager::CPlayerManager()
{

}

//************************************************************
// デストラクタ
//************************************************************
CPlayerManager::~CPlayerManager()
{

}

//************************************************************
// 初期化
//************************************************************
HRESULT CPlayerManager::Init(void)
{
	return S_OK;
}

//************************************************************
// 破棄
//************************************************************
void CPlayerManager::Uninit(void)
{

}

//************************************************************
// 更新
//************************************************************
void CPlayerManager::Update(void)
{
	bool bPause = CManager::isPause();

	// ポーズ中だったら
	if (bPause != true)
	{
		// タイマーを進める
		m_Timer++;

		// スポーンさせる時間なら
		if (m_Timer > SPAWN_TIME)
		{
			// タイマーリセット
			m_Timer = 0;

			// 最大数以下なら
			if (m_pPlayer.size() < CAPACITY)
			{
				// ローカル変数
				CPlayer* Info = {};

				// 生成
				Info = CPlayer::Create(SPAWN_POS, SPAWN_ROT);

				// 情報を前から追加していく
				m_pPlayer.push_back(Info);
			}
		}
	}

	// プレイヤーの配列にアクセス
	for (auto Players = m_pPlayer.begin(); Players != m_pPlayer.end(); Players++)
	{
		// 落ちていたら
		if ((*Players)->GetPos().y < Fall)
		{
			// 破棄
			(*Players)->Uninit();
			Players = m_pPlayer.erase(Players);
			if (Players == m_pPlayer.end()) break;
		}
	}
}

//************************************************************
// 描画
//************************************************************
void CPlayerManager::Draw(void)
{

}