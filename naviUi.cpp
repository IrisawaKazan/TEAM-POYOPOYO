//--------------------------------
//
// ナビゲーションUI [naviUI.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviUi.h"
#include "object2D.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "navi.h"
#include "math_T.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CNaviUI* CNaviUI::Create(const char* frameTexturePath, std::vector<const char*> objectTexturePaths, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNaviUI* pNaviUI = new CNaviUI;
	if (pNaviUI == nullptr)
	{
		return nullptr;
	}

	pNaviUI->SetFrameTexturePath(frameTexturePath);
	pNaviUI->SetObjectTexturePaths(objectTexturePaths);
	pNaviUI->SetPos(pos);
	pNaviUI->SetSize(size);

	// 初期化
	if (FAILED(pNaviUI->Init()))
	{
		delete pNaviUI;
		return nullptr;
	}
	return pNaviUI;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviUI::Init(void)
{
	// フレームのセット
	CTextureManager* textureManager = CTextureManager::Instance();
	m_pFrame = CObject2D::Create(m_pos, VEC3_NULL, m_size);
	m_pFrame->SetTexIndx(textureManager->Register(m_frameTexturePath));
	m_pFrame->SetAlphaTest(true);

	m_pObjects[0] = CObject2D::Create(m_pos, VEC3_NULL, m_size * 0.7f);
	m_pObjects[1] = CObject2D::Create(m_pos + D3DXVECTOR3(-m_size.x * 0.7f, m_size.y * 0.7f, 0.0f), VEC3_NULL, m_size * 0.2f);
	m_pObjects[2] = CObject2D::Create(m_pos + D3DXVECTOR3(m_size.x * 0.7f, m_size.y * 0.7f, 0.0f), VEC3_NULL, m_size * 0.2f);
	for (auto pObject : m_pObjects)
	{
		pObject->SetAlphaTest(true);
	}

	SetObjectUI(); // オブジェクトUIのセット

	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviUI::Uninit(void)
{

}

//--------------------------------
// 更新処理
//--------------------------------
void CNaviUI::Update(void)
{
	SetObjectUI(); // オブジェクトUIのセット
}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviUI::Draw(void)
{

}

//--------------------------------
// オブジェクトUIのセット
//--------------------------------
void CNaviUI::SetObjectUI(void)
{
	CTextureManager* textureManager = CTextureManager::Instance();

	// 今のオブジェクト
	unsigned int listID = static_cast<int>(CNavi::GetInstance()->GetList());
	if (!m_objectTexturePaths.empty() && m_objectTexturePaths.size() > listID)
	{// テクスチャがあればセット
		if (m_pObjects[0] != nullptr)
		{
			m_pObjects[0]->SetTexIndx(textureManager->Register(m_objectTexturePaths[listID]));
		}
	}

	// 前のオブジェクト
	unsigned int lastListID = Wrap(int(listID - 1), int(0), int(CNavi::LIST::Max) - 1);
	if (!m_objectTexturePaths.empty() && m_objectTexturePaths.size() > lastListID)
	{// テクスチャがあればセット
		if (m_pObjects[1] != nullptr)
		{
			m_pObjects[1]->SetTexIndx(textureManager->Register(m_objectTexturePaths[lastListID]));
		}
	}

	// 次のオブジェクト
	unsigned int nextListID = Wrap(int(listID + 1), int(0), int(CNavi::LIST::Max) - 1);
	if (!m_objectTexturePaths.empty() && m_objectTexturePaths.size() > nextListID)
	{// テクスチャがあればセット
		if (m_pObjects[2] != nullptr)
		{
			m_pObjects[2]->SetTexIndx(textureManager->Register(m_objectTexturePaths[nextListID]));
		}
	}
}