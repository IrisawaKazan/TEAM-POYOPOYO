//=================================================
//
//	switch.cpp
//
// Author:chikada shouya
//
//=================================================
// 
//***************************************
// インクルードファイル宣言
//***************************************
#include "switch.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"

//***************************************
// コンストラクタ
//***************************************
CSwitch::CSwitch(int nPriority) :CBlock(nPriority)
{
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	sNamePath = {};
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
	CSwitch* pSwitch = nullptr;
	pSwitch = new CSwitch;
	if (pSwitch != nullptr)
	{
		// オブジェクト設定
		pSwitch->SetFilepath(sName);
		pSwitch->SetPosition(pos);
		pSwitch->SetRotasion(rot);
		pSwitch->SetScale(Scale);
		pSwitch->Init();
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