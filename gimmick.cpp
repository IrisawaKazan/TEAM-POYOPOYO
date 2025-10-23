//=================================================
//
//	block.cpp
// 
// Author:chikada shouya
//
//=================================================

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
	m_VtxMax = { -10000.0f,-10000.0f,-10000.0f };
	m_VtxMin = { 10000.0f,10000.0f,10000.0f };
	m_size = VEC3_NULL;
	m_RBOffset = VEC3_NULL;
	sNamePath = {};
}

//***************************************
// デストラクタ
//***************************************
CGimmick::~CGimmick()
{

}

//***************************************
// ブロックの生成
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
//　ブロック初期化処理
//***************************************
HRESULT CGimmick::Init(void)
{
	// 初期化
	CBlock::Init();
	
	return S_OK;
}

//***************************************
//　ブロック終了処理
//***************************************
void CGimmick::Uninit(void)
{
	// 破棄
	CBlock::Uninit();
}

//***************************************
//　ブロック更新処理
//***************************************
void CGimmick::Update(void)
{
	// 更新
	CBlock::Update();

	//// ナビにレイキャストオブジェクトを登録 sato 仮
	//CModelManager* pModelTexManager = CModelManager::Instance();
	//CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));
	//CNavi::GetInstance()->RegisterRayCastObject(modelinfo.pMesh, GetWorldMtx());
}

//***************************************
//　ブロック描画処理
//***************************************
void CGimmick::Draw(void)
{
	// 描画
	CBlock::Draw();
}