//======================================================
// 
// 影の処理
// Author:Takahashi Misaki
// 
//======================================================

#include "shadow.h"
#include "manager.h"

//********************************************
// コンストラクタ
//********************************************
CShadow::CShadow(int nPriority) :CObject3D(nPriority)
{

}

//********************************************
// デストラクタ
//********************************************
CShadow::~CShadow()
{

}

//********************************************
// 初期化処理
//********************************************
HRESULT CShadow::Init(void)
{
	// オブジェクト3Dの初期化処理
	CObject3D::Init();

	return S_OK;
}

//********************************************
// 終了処理
//********************************************
void CShadow::Uninit(void)
{
	// オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//********************************************
// 更新処理
//********************************************
void CShadow::Update(void)
{
	// オブジェクト3Dの更新処理
	CObject3D::Update();

}

//********************************************
// 描画処理
//********************************************
void CShadow::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, NULL);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// 減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// オブジェクト3Dの描画処理
	CObject3D::Draw();

	// 設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// アルファテストを無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

}

//********************************************
// クリエイト処理
//********************************************
CShadow* CShadow::Create(D3DXVECTOR3 pos, D3DXCOLOR col, float fWidth, float fHeight, const char* FilePath)
{
	CShadow* pShadow;

	// 影を生成
	pShadow = new CShadow;

	// 引数を代入する
	pShadow->SetPosition(pos);							// 位置
	pShadow->SetCol(col);								// 色
	pShadow->SetSize(D3DXVECTOR2(fWidth, fHeight));		// サイズ
	pShadow->SetFilePath(FilePath);						// テクスチャ名

	// 初期化処理
	pShadow->Init();

	return pShadow;

}