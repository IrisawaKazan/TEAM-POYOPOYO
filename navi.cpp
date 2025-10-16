//--------------------------------
//
// ナビゲーションマーカー [navi.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "navi.h"
#include "manager.h"
#include "renderer.h"
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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ビューポートの取得
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);

	// 射影行列の取得
	D3DXMATRIX mtxProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProj);

	// ビューマトリックスの取得
	D3DXMATRIX mtxView;
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	// 始点と終点
	D3DXVECTOR3 vNear, vFar;

	// ニアクリップ面上の3D座標を計算
	D3DXVECTOR3 screenNear(mousePos.x, mousePos.y, 0.0f);
	D3DXVec3Unproject(
		&vNear,
		&screenNear,
		&viewPort,
		&mtxProj,
		&mtxView,
		NULL
	);

	// ファークリップ面上の3D座標を計算
	D3DXVECTOR3 screenFar(mousePos.x, mousePos.y, 1.0f);
	D3DXVec3Unproject(
		&vFar,
		&screenFar,
		&viewPort,
		&mtxProj,
		&mtxView,
		NULL
	);

	// レイの始点と方向を決定
	D3DXVECTOR3 rayOrigin = vNear;                   // レイの始点はニアクリップ面上の点
	D3DXVECTOR3 rayDirection = vFar - vNear;         // ニアからファーへ向かうベクトル
	D3DXVec3Normalize(&rayDirection, &rayDirection); // 方向ベクトルなので正規化する
}