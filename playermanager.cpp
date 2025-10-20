//************************************************************
//
//　プレイヤーの処理[playermanager.cpp]
//　Author:chikada shouya
//
//************************************************************

// インクルード
#include "playermanager.h"

// 静的メンバ変数の定義
const D3DXVECTOR3 CPlayerManager::SPAWN_POS = D3DXVECTOR3(-100.0f, 0.0f, 0.0f); // NPCの出現位置 sato Add

// コンストラクタ
CPlayerManager::CPlayerManager()
{
	
}

// デストラクタ
CPlayerManager::~CPlayerManager()
{

}

// 初期化
HRESULT CPlayerManager::Init(void)
{
	m_pPlayer.resize(CAPACITY);
	m_bUses.resize(CAPACITY);

	for (int nCnt = 0; nCnt < CAPACITY; nCnt++)
	{
		// 情報を前から追加していく
		m_pPlayer[nCnt] = CPlayer::Create({10000.0f,0.0f,0.0f}, VEC3_NULL);
		m_bUses[nCnt] = false;
	}

	return S_OK;
}

// 破棄
void CPlayerManager::Uninit(void)
{

}

// 更新
void CPlayerManager::Update(void)
{
	// タイマーを進める
	m_Timer++;

	// スポーンさせる時間なら
	if (m_Timer > SPAWN_TIME)
	{
		// タイマーリセット
		m_Timer = 0;

		bool IsCapacity = false;

		for (int nCnt = 0; nCnt < m_pPlayer.size(); nCnt++)
		{
			if (m_bUses[nCnt] == false)
			{
				IsCapacity = true;
				m_bUses[nCnt] = true;
				m_pPlayer[nCnt]->SetPos(SPAWN_POS);
				break;
			}
		}
		if (IsCapacity == false)
		{
			// ローカル変数
			CPlayer* Info = {};

			// 生成
			Info = CPlayer::Create(SPAWN_POS, VEC3_NULL);

			// 情報を前から追加していく
			m_pPlayer.push_back(Info);
		}
	}
}

// 描画
void CPlayerManager::Draw(void)
{

}