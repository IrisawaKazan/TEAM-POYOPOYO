//--------------------------------
//
// ナビゲーションマーカー [naviMarker.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviMarker.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "arrow.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

//--------------------------------
// 生成
//--------------------------------
CNaviMarker* CNaviMarker::Create(const char* filePath, D3DXVECTOR2 size)
{
	// インスタンスの生成
	CNaviMarker* pNaviMarker = new CNaviMarker;
	if (pNaviMarker == nullptr)
	{
		return nullptr;
	}

	pNaviMarker->SetTexIdx(CTextureManager::Instance()->Register(filePath));
	pNaviMarker->SetSize(size);

	// 初期化
	if (FAILED(pNaviMarker->Init()))
	{
		delete pNaviMarker;
		return nullptr;
	}
	return pNaviMarker;
}

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviMarker::Init(void)
{
	// デバイスを取得
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// 頂点バッファをロック
	VERTEX_3D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = D3DXVECTOR3(-m_size.x, 0.0f, m_size.y);
		pVtx[1].pos = D3DXVECTOR3(m_size.x, 0.0f, m_size.y);
		pVtx[2].pos = D3DXVECTOR3(-m_size.x, 0.0f, -m_size.y);
		pVtx[3].pos = D3DXVECTOR3(m_size.x, 0.0f, -m_size.y);

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVertex->Unlock();

	m_pos = { 0,0,0 };
	D3DXMatrixIdentity(&m_mtxRot);
	m_nBiasID = 0;
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviMarker::Uninit(void)
{
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// メモリの開放
	Release();
}

//--------------------------------
// 更新処理
//--------------------------------
void CNaviMarker::Update(void)
{

}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviMarker::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f;                                  //Zバッファをカメラ方向にオフセットする値
	depthBias *= m_nBiasID;                                        // バイアスID分だけオフセット
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

	//計算用マトリックス
	D3DXMATRIX mtxWorld, mtxTrans;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	//向きを反転
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxWorld);

	// 頂点バッファをデバイスからデータストリームに設定
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_3D));

	// テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::Instance()->GetAddress(m_texIdx));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// Depth Bias 設定を解除
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}