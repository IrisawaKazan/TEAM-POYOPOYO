//=================================================
//
//	gimmick.cpp
//
// Author:chikada shouya
//
//=================================================
// 
//***************************************
// インクルードファイル宣言
//***************************************
#include "gimmick.h"
#include "manager.h"
#include "game.h"
#include "model.h"
#include "navi.h"

//***************************************
// コンストラクタ
//***************************************
CGimmick::CGimmick(int nPriority) :CBlock(nPriority)
{
	m_type = TYPE_NONE;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
}

//***************************************
// デストラクタ
//***************************************
CGimmick::~CGimmick()
{
}

//***************************************
// ギミックの生成
//***************************************
CGimmick* CGimmick::Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale)
{
	CGimmick* pGimmick = nullptr;
	pGimmick = new CGimmick;
	if (pGimmick != nullptr)
	{
		// オブジェクト設定
		pGimmick->SetFilepath(sName);
		pGimmick->SetPosition(pos);
		pGimmick->SetRotasion(rot);
		pGimmick->SetScale(Scale);
		pGimmick->Init();
		return pGimmick;
	}
	else
	{
		return nullptr;
	}
}

//***************************************
//　ギミック初期化処理
//***************************************
HRESULT CGimmick::Init(void)
{
	// 初期化
	CBlock::Init();

	return S_OK;
}

//***************************************
//　ギミック終了処理
//***************************************
void CGimmick::Uninit(void)
{
	// 破棄
	CBlock::Uninit();
}

//***************************************
//　ギミック更新処理
//***************************************
void CGimmick::Update(void)
{
	// 更新
	CBlock::Update();

	if (m_type == TYPE_SWITCH)
	{

	}
}

//***************************************
//　ギミック描画処理
//***************************************
void CGimmick::Draw(void)
{
	// 描画
	CBlock::Draw();
}