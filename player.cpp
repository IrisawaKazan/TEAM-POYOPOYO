//************************************************************
//
//　プレイヤーの処理[player.cpp]
//　Author:chikada shouya
//
//************************************************************
#include "player.h"
#include "math_T.h"

// 静的メンバ変数の定義

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

	// 移動処理 (プロト用) sato Add
	CModelCharacter::SetRot(D3DXVECTOR3(0.0f, TEST_MOVE_ANGLE, 0.0f));
	CModelCharacter::SetRotDest(D3DXVECTOR3(0.0f, TEST_MOVE_ANGLE, 0.0f));
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
	TestMove(); // プロト用移動処理 sato Add
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

//*******************************
// プロト用移動処理 sato Add
//*******************************
void CPlayer::TestMove(void)
{
	// 移動処理 (プロト用) sato Add
	D3DXVECTOR3 rot = CModelCharacter::GetRot();
	D3DXVECTOR3 pos = CModelCharacter::GetPos();
	CModelCharacter::SetMove(D3DXVECTOR3(-sinf(rot.y) * TEST_MOVE_SPEED, 0.0f, -cosf(rot.y) * TEST_MOVE_SPEED + (0.0f - pos.z) * 0.01f));
	D3DXVECTOR3 posOld = CModelCharacter::GetPosOld();
	if (std::abs(pos.x) > TEST_MOVE_LIMIT && std::abs(posOld.x) <= TEST_MOVE_LIMIT)
	{// 移動制限を超えたら
		rot.y += D3DX_PI; // 角度を反転
		Normalize(rot.y, &rot.y);
		CModelCharacter::SetRotDest(rot);
	}
}