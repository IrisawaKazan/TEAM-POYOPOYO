//=================================================
//
//	switch.cpp
//
// Author:chikada shouya
//
//=================================================

//***************************************
// インクルードファイル宣言
//***************************************
#include "switch.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"
#include "playermanager.h"

//***************************************
// コンストラクタ
//***************************************
CSwitch::CSwitch(int nPriority) :CBlock(nPriority)
{
}

//***************************************
// デストラクタ
//***************************************
CSwitch::~CSwitch()
{
}

//***************************************
// スイッチ生成
//***************************************
CSwitch* CSwitch::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	// インスタンスを宣言
	CSwitch* pSwitch = nullptr;

	// メモリを確保
	pSwitch = new CSwitch;

	// 確保できたら
	if (pSwitch != nullptr)
	{
		// オブジェクト設定
		pSwitch->SetFilepath(sName);
		pSwitch->SetPosition(pos);
		pSwitch->SetRotasion(rot);
		pSwitch->SetScale(Scale);
		pSwitch->Init();

		// 返す
		return pSwitch;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
//　スイッチ初期化処理
//***************************************
HRESULT CSwitch::Init(void)
{
	// 初期化
	CBlock::Init();

	// 初期位置を記憶
	m_ApperPos = GetPosition();

	return S_OK;
}

//***************************************
//　スイッチ終了処理
//***************************************
void CSwitch::Uninit(void)
{
	// 破棄
	CBlock::Uninit();
}

//***************************************
//　スイッチ更新処理
//***************************************
void CSwitch::Update(void)
{
	// 今の位置を取得
	D3DXVECTOR3 CurrentPos = GetPosition();

	m_IsFinishOld = m_IsFinish;
	m_IsFinish = false;

	// 押されていたら
	if (m_IsPressed == true)
	{
		// 沈み切っていなかったら
		if (m_ApperPos.y + Config::Depth < CurrentPos.y - Config::PressSpeed)
		{
			// 沈める
			CurrentPos.y -= Config::PressSpeed;
		}
		else
		{
			m_IsFinish = true;
		}
	}
	else
	{
		// 上がりきっていなかったら
		if (m_ApperPos.y > CurrentPos.y + Config::ReturnSpeed)
		{
			// あげる
			CurrentPos.y += Config::ReturnSpeed;
		}
	}

	if (m_IsFinish == true && m_IsFinishOld == false)
	{
		// サウンドの取得
		CSound* pSound = CManager::GetSound();

		// SE
		pSound->Play(CSound::LABEL_SWITCH_SE);
	}

	// 位置を設定
	SetPosition(CurrentPos);

	// 更新
	CBlock::Update();
}

//***************************************
//　スイッチ描画処理
//***************************************
void CSwitch::Draw(void)
{
	// 描画
	CBlock::Draw();
}

//***************************************
// ドア
//***************************************
//***************************************
// コンストラクタ
//***************************************
CDoor::CDoor(int nPriority) : CBlock(nPriority)
{
}

//***************************************
// デストラクタ
//***************************************
CDoor::~CDoor()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CDoor::Init(void)
{
	// 初期化
	CBlock::Init();

	// 初期位置を保存
	m_ApperPos = GetPosition();

	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CDoor::Uninit(void)
{
	// 破棄
	CBlock::Uninit();
}

//***************************************
// 更新処理
//***************************************
void CDoor::Update(void)
{
	// 位置を取得
	D3DXVECTOR3 Pos = GetPosition();

	// 起動していたら
	if (m_IsFunc)
	{
		// ドアが上がりきっていなかったら
		if (m_ApperPos.y + Config::Limit > Pos.y + Config::UpSpeed)
		{
			// あげる
			Pos.y += Config::UpSpeed;
		}
	}
	// 起動していなかったら
	else
	{
		// ドアが下がりきっていなかったら
		if (m_ApperPos.y < Pos.y - Config::DownSpeed)
		{
			// 下げる
			Pos.y -= Config::DownSpeed;
		}
	}

	// 位置を設定
	SetPosition(Pos);

	// 更新
	CBlock::Update();
}

//***************************************
// 描画処理
//***************************************
void CDoor::Draw(void)
{
	// 描画
	CBlock::Draw();
}

//***************************************
// 生成
//***************************************
CDoor* CDoor::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	// インスタンスを宣言
	CDoor* pDoor = nullptr;

	// メモリ確保
	pDoor = new CDoor;

	// 確保できたら
	if (pDoor != nullptr)
	{
		// オブジェクト設定
		pDoor->SetFilepath(sName);
		pDoor->SetPosition(pos);
		pDoor->SetRotasion(rot);
		pDoor->SetScale(Scale);
		pDoor->Init();

		// メモリ確保
		return pDoor;
	}
	else
	{
		return nullptr;
	}
}