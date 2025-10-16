//--------------------------------
//
// ナビゲーションマーカー [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "manager.h"
#include "input.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CNavi* CNavi::Create(void)
{
	CNavi* pNavi = new CNavi;
	if (pNavi == NULL)
	{
		return NULL;
	}
	if (FAILED(pNavi->Init()))
	{
		delete pNavi;
		return NULL;
	}
	return pNavi;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNavi::Init(void)
{
	CObjectBillBoard::Init();
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNavi::Uninit(void)
{
	CObjectBillBoard::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNavi::Update(void)
{
	CreateRay(CManager::GetInputMouse()->GetPos());
	CObjectBillBoard::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CNavi::Draw(void)
{
	CObjectBillBoard::Draw();
}

//--------------------------------
// レイを作成
//--------------------------------
void CNavi::CreateRay(D3DXVECTOR2 mousePos)
{

}