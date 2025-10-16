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
CNavi* CNavi::Create(const char* filePath, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNavi* pNavi = new CNavi;
	if (pNavi == nullptr)
	{
		return nullptr;
	}

	pNavi->SetFilePath(filePath);
	pNavi->SetSize(size);

	// 初期化
	if (FAILED(pNavi->Init()))
	{
		delete pNavi;
		return nullptr;
	}
	return pNavi;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNavi::Init(void)
{
	// 親クラスの初期化
	CObject3D::Init();
	
	// レイを作成
	CreateRay(CManager::GetInputMouse()->GetPos());

	// 初期位置を設定
	SetPosition(PlaneIntersect(HEIGHT));
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNavi::Uninit(void)
{
	CObject3D::Uninit();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNavi::Update(void)
{
	// レイを作成
	CreateRay(CManager::GetInputMouse()->GetPos());

	// 位置を更新
	SetPosition(PlaneIntersect(HEIGHT));

	// 親クラスの更新
	CObject3D::Update();
}

//--------------------------------
// 描画処理
//--------------------------------
void CNavi::Draw(void)
{
	CObject3D::Draw();
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
		nullptr
	);

	// ファークリップ面上の3D座標を計算
	D3DXVECTOR3 screenFar(mousePos.x, mousePos.y, 1.0f);
	D3DXVec3Unproject(
		&vFar,
		&screenFar,
		&viewPort,
		&mtxProj,
		&mtxView,
		nullptr
	);

	// レイの始点と方向を決定
	m_RayPos = vNear;                        // レイの始点はニアクリップ面上の点
	m_RayDir = vFar - vNear;                 // ニアからファーへ向かうベクトル
	D3DXVec3Normalize(&m_RayDir, &m_RayDir); // 方向ベクトルなので正規化する
}

//--------------------------------
// 平面との交点を求める
//--------------------------------
D3DXVECTOR3 CNavi::PlaneIntersect(float fHeight)
{
	// y=fHeight の平面
	D3DXPLANE plane(0.0f, 1.0f, 0.0f, -fHeight);

	// 交点
	D3DXVECTOR3 intersectionPoint;

	// rayの終点
	D3DXVECTOR3 rayEnd = m_RayPos + m_RayDir * 10000.0f;

	// レイと平面の交点を計算
	D3DXPlaneIntersectLine(
		&intersectionPoint, // 結果の交点
		&plane,             // 平面
		&m_RayPos,          // レイの始点
		&rayEnd             // レイの終点
	);

	if (m_RayDir.y < 0)
	{// レイが下向きのときのみ交点を有効とする
		return intersectionPoint;
	}

	// 交点が無効なときは非常に低い位置を返す
	return D3DXVECTOR3(0.0f, -1000.0f, 0.0f);
}