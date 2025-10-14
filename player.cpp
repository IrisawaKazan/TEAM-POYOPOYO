//************************************************************
//
//　プレイヤーの処理[player.cpp]
//　Author:chikada shouya
//
//************************************************************
#include "player.h"

// コンストラクタ
CPlayer::CPlayer()
{

}

// デストラクタ
CPlayer::~CPlayer()
{

}

// 初期化
HRESULT CPlayer::Init(void)
{
	CModelCharacter::Init("data\\Model\\MOTION\\player.txt",5.0f);
	return S_OK;
}

// 破棄
void CPlayer::Uninit(void)
{
	CModelCharacter::Uninit();
}

// 更新
void CPlayer::Update(void)
{
	CModelCharacter::Update();
}

// 描画
void CPlayer::Draw(void)
{
	CModelCharacter::Draw();
}

// 生成
CPlayer* CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer* pPlayer = nullptr;
	pPlayer = new CPlayer;

	if (pPlayer != nullptr)
	{
		pPlayer->SetPos(pos);
		pPlayer->SetRot(rot);
		pPlayer->Init();
		return pPlayer;
	}
	else
	{
		return nullptr;
	}

}
