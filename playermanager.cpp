//************************************************************
//
//　プレイヤーの処理[playermanager.cpp]
//　Author:chikada shouya
//
//************************************************************
#include "playermanager.h"

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
	// ローカル変数
	CPlayer* Info = {};

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		Info = CPlayer::Create(D3DXVECTOR3(50.0f * nCnt,0.0f,0.0f), VEC3_NULL);

		// 情報を前から追加していく
		m_pPlayer.push_back(Info);
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

}

// 描画
void CPlayerManager::Draw(void)
{

}