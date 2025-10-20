//=================================================
//
//	block.cpp
// 
// Author:近田 尚也
//
//=================================================

//****************************
// インクルードファイル宣言
//****************************
#include "block.h"
#include "manager.h"
#include "game.h"
#include "model.h"

// コンストラクタ
CBlock::CBlock(int nPriority):CObjectX(nPriority)
{
	m_type = TYPE_0;
	m_pos = VEC3_NULL;
	m_rot = VEC3_NULL;
	m_VtxMax = VEC3_NULL;
	m_VtxMin = VEC3_NULL;
	m_size = VEC3_NULL;
	sNamePath = {};
}

// デストラクタ
CBlock::~CBlock()
{

}

//=========================
// ブロックの生成
//=========================
CBlock* CBlock::Create(std::string sName,D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CBlock* pBlock = nullptr;
	pBlock = new CBlock;

	if (pBlock != nullptr)
	{
		// オブジェクト設定
		pBlock->SetFilePath(sName);
		pBlock->sNamePath = sName;
		pBlock->SetPosition(pos);
		pBlock->SetRotasion(D3DXToRadian(rot));
		pBlock->Init();
		return pBlock;
	}
	else
	{
		return nullptr;
	}
}

//=========================
//　ブロック初期化処理
//=========================
HRESULT CBlock::Init(void)
{
	CObjectX::Init();

	CModelManager* pModelTexManager = CModelManager::Instance();
	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(sNamePath));

	LPD3DXMESH pMesh = modelinfo.pMesh;

	// ローカル変数宣言-----------------------------
	int nNumVtx;		// 頂点数
	DWORD sizeFVF;		// 頂点フォーマットのサイズ
	BYTE* pVtxBuff;		// 頂点バッファのポインタ
	//----------------------------------------------

	// 頂点数の取得
	nNumVtx = pMesh->GetNumVertices();

	// 頂点のサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());

	// 頂点バッファをロック
	pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	// アンロック
	pMesh->UnlockVertexBuffer();

	return S_OK;
}
//=========================
//　ブロック終了処理
//=========================
void CBlock::Uninit(void)
{
	// 破棄
	CObjectX::Uninit();
}
//=========================
//　ブロック更新処理
//=========================
void CBlock::Update(void)
{
	// 更新
	CObjectX::Update();
}
//=========================
//　ブロック描画処理
//=========================
void CBlock::Draw(void)
{
	// 描画
	CObjectX::Draw();
}