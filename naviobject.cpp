//--------------------------------
//
// ナビゲーションオブジェクト [naviobject.cpp]
// Author : Fuma Sato
//
//--------------------------------
#include "naviobject.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------
//
// ナビゲーションマーカーのクラス
// 
//--------------------------------

size_t CNaviObject::m_nextIdx = 0; // 次のインデックス (Trigger渡し用)

//--------------------------------
// 初期化処理
//--------------------------------
HRESULT CNaviObject::Init(void)
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
	return S_OK;
}

//--------------------------------
// 終了処理
//--------------------------------
void CNaviObject::Uninit(void)
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
void CNaviObject::Update(void)
{

}

//--------------------------------
// 描画処理
//--------------------------------
void CNaviObject::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストを有効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	// Depth Bias 設定 sato
	float depthBias = -0.000001f;                                  //Zバッファをカメラ方向にオフセットする値
	depthBias *= m_biasIdx;                                        // バイアスID分だけオフセット
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&depthBias); //Zバイアス設定

	// 計算用マトリックス
	D3DXMATRIX mtxWorld, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// アングルの適応
	const D3DXVECTOR3 up = VEC_UP;
	D3DXMatrixRotationAxis(&mtxWorld, &up, m_angle);

	// 向きを反転
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &m_mtxRot);

	// 位置を反映
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

	// Depth Bias 設定を解除 sato
	float resetBias = 0.0f;
	pDevice->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&resetBias);

	// アルファテストを無効に戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------
// 触れたら起動する
//--------------------------------
CNavi::TYPE CNaviObject::ActivateTrigger(const D3DXVECTOR3& pos, float* pAngle, size_t* pIdx) const
{
	D3DXVECTOR3 space = pos - m_pos;
	float hight(space.y);
	space.y = 0.0f;
	float length = D3DXVec3Length(&space);
	if (length <= m_chengeLength && std::abs(hight) <= CHENGE_HEIGHT)
	{
		Activate(pAngle);
		if (pIdx != nullptr)
		{
			*pIdx = m_idx;
		}
		return m_type;
	}
	else
	{
		return CNavi::TYPE::None;
	}
}

//--------------------------------
// 触れたら消す
//--------------------------------
bool CNaviObject::ReleaseTrigger(const D3DXVECTOR3& pos, float length) const
{
	D3DXVECTOR3 space = pos - m_pos;
	float hight(space.y);
	space.y = 0.0f;
	float spaseLength = D3DXVec3Length(&space);
	return (spaseLength < (m_length + length) && std::abs(hight) <= CHENGE_HEIGHT);
}