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
CNaviUI* CNaviUI::Create(const char* frameTexturePath, std::vector<const char*> objectTexturePaths, std::vector<const char*> arrowTexturePaths, std::vector<const char*> keyTexturePaths, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNaviUI* pNaviUI = new CNaviUI;
	if (pNaviUI == nullptr)
	{
		return nullptr;
	}

	pNaviUI->SetFrameTexturePath(frameTexturePath);
	pNaviUI->SetObjectTexturePaths(objectTexturePaths);
	pNaviUI->SetArrowTexturePaths(arrowTexturePaths);
	pNaviUI->SetKeyTexturePaths(keyTexturePaths);
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
	m_pFrames[0] = CObject2D::Create(m_pos, VEC3_NULL, m_size, GetPriority());
	m_pFrames[1] = CObject2D::Create(m_pos + D3DXVECTOR3(-m_size.x * SIDE_OBJECT_POS_MAGNIFICATION_X, m_size.y * SIDE_OBJECT_POS_MAGNIFICATION_Y, 0.0f), VEC3_NULL, m_size * MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION, GetPriority());
	m_pFrames[2] = CObject2D::Create(m_pos + D3DXVECTOR3(m_size.x * SIDE_OBJECT_POS_MAGNIFICATION_X, m_size.y * SIDE_OBJECT_POS_MAGNIFICATION_Y, 0.0f), VEC3_NULL, m_size * MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION, GetPriority());
	for (auto pFrame : m_pFrames)
	{
		pFrame->SetTexIndx(textureManager->Register(m_frameTexturePath));
		pFrame->SetAlphaTest(true);
	}

	m_pObjects[0] = CObject2D::Create(m_pos, VEC3_NULL, m_size * FARAME_TO_OBJECT_SIZE_MAGNIFICATION, GetPriority());
	m_pObjects[1] = CObject2D::Create(m_pos + D3DXVECTOR3(-m_size.x * SIDE_OBJECT_POS_MAGNIFICATION_X, m_size.y * SIDE_OBJECT_POS_MAGNIFICATION_Y, 0.0f), VEC3_NULL, m_size * FARAME_TO_OBJECT_SIZE_MAGNIFICATION * MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION, GetPriority());
	m_pObjects[2] = CObject2D::Create(m_pos + D3DXVECTOR3(m_size.x * SIDE_OBJECT_POS_MAGNIFICATION_X, m_size.y * SIDE_OBJECT_POS_MAGNIFICATION_Y, 0.0f), VEC3_NULL, m_size * FARAME_TO_OBJECT_SIZE_MAGNIFICATION * MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION, GetPriority());
	for (auto pObject : m_pObjects)
	{
		pObject->SetAlphaTest(true);
	}

	m_pKeys[0] = CObject2D::Create(m_pos + D3DXVECTOR3(-m_size.x * SIDE_OBJECT_POS_MAGNIFICATION_X, m_size.y * SIDE_OBJECT_POS_MAGNIFICATION_Y, 0.0f) + D3DXVECTOR3(0.0f, -m_size.y * KEY_HEIGHT_MAGNIFICATION, 0.0f), VEC3_NULL, m_size * MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION * KEY_SIZE_MAGNIFICATION, GetPriority());
	m_pKeys[1] = CObject2D::Create(m_pos + D3DXVECTOR3(m_size.x * SIDE_OBJECT_POS_MAGNIFICATION_X, m_size.y * SIDE_OBJECT_POS_MAGNIFICATION_Y, 0.0f) + D3DXVECTOR3(0.0f, -m_size.y * KEY_HEIGHT_MAGNIFICATION, 0.0f), VEC3_NULL, m_size * MAIN_OBJECT_TO_SIDE_OBJECT_SIZE_MAGNIFICATION * KEY_SIZE_MAGNIFICATION, GetPriority());
	for (auto pKey : m_pKeys)
	{
		pKey->SetAlphaTest(true);
	}

	SetObjectUI(); // オブジェクトUIのセット
	SetKeyUI();    // キーUIのセット
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviUI::Uninit(void)
{
	Release();
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
void CNaviUI::SetObjectUI()
{
	CTextureManager* textureManager = CTextureManager::Instance();
	CNavi* pNavi = CNavi::GetInstance();

	// モード
	bool isArrowMode = pNavi->GetArrowMode();

	// モードで使用するTextureを変える
	std::vector<const char*>& texturePaths = isArrowMode ? m_arrowTexturePaths : m_objectTexturePaths;

	// 今のオブジェクト
	unsigned int listID = isArrowMode ? static_cast<int>(pNavi->GetArrowType()) : static_cast<int>(pNavi->GetType());

	// 範囲
	int listMax = isArrowMode ? int(CNavi::ARROW::Max) : int(CNavi::TYPE::Max);

	if (!texturePaths.empty() && texturePaths.size() > listID)
	{// テクスチャがあればセット
		if (m_pObjects[0] != nullptr && texturePaths[listID] != nullptr)
		{
			m_pObjects[0]->SetTexIndx(textureManager->Register(texturePaths[listID]));
		}
	}

	// 前のオブジェクト
	unsigned int lastListID{ listID };
	while (true)
	{// 無効化オブジェクトをスキップ
		lastListID = Wrap(int(lastListID - 1), int(0), listMax - 1);
		if ((isArrowMode && pNavi->GetEnable(CNavi::ARROW(lastListID))) || (!isArrowMode && pNavi->GetEnable(CNavi::TYPE(lastListID))))break;
	}
	if (!texturePaths.empty() && texturePaths.size() > lastListID)
	{// テクスチャがあればセット
		if (m_pObjects[1] != nullptr && texturePaths[lastListID] != nullptr)
		{
			m_pObjects[1]->SetTexIndx(textureManager->Register(texturePaths[lastListID]));
		}
	}

	// 次のオブジェクト
	unsigned int nextListID{ listID };
	while (true)
	{// 無効化オブジェクトをスキップ
		nextListID = Wrap(int(nextListID + 1), int(0), listMax - 1);
		if ((isArrowMode && pNavi->GetEnable(CNavi::ARROW(nextListID))) || (!isArrowMode && pNavi->GetEnable(CNavi::TYPE(nextListID))))break;
	}
	if (!texturePaths.empty() && texturePaths.size() > nextListID)
	{// テクスチャがあればセット
		if (m_pObjects[2] != nullptr && texturePaths[nextListID] != nullptr)
		{
			m_pObjects[2]->SetTexIndx(textureManager->Register(texturePaths[nextListID]));
		}
	}
}

//--------------------------------
// キーUIのセット
//--------------------------------
void CNaviUI::SetKeyUI()
{
	if (m_pKeys.empty() || m_keyTexturePaths.empty())return; // キーのオブジェクトやテクスチャがない

	CTextureManager* textureManager = CTextureManager::Instance();

	for (size_t cnt = 0; cnt < m_pKeys.size(); ++cnt)
	{// キー配列の走査
		if (m_keyTexturePaths.size() > cnt)
		{
			if (m_pKeys[cnt] != nullptr && m_keyTexturePaths[cnt] != nullptr)
			{// テクスチャがあればセット
				m_pKeys[cnt]->SetTexIndx(textureManager->Register(m_keyTexturePaths[cnt]));
			}
		}
	}
}