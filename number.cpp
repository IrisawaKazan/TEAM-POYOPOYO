//****************************************************************
//
// [number.h]
// Author: Irisawa Kazan
//
//****************************************************************
#include "number.h"
#include "manager.h"
#include "texturemanager.h"

//****************************************************************
// コンストラクタ
//****************************************************************
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nIdx = NULL;
	m_nColTime = NULL;
	m_bChange = false;

}

//****************************************************************
// デストラクタ
//****************************************************************
CNumber::~CNumber()
{

}

//****************************************************************
// 生成
//****************************************************************
CNumber* CNumber::Create(D3DXVECTOR3 pos)
{
	return NULL;
}

//****************************************************************
// 初期化
//****************************************************************
HRESULT CNumber::Init(float fX1, float fX2, float fY1, float fY2, int nCnt, int nCnt2, float fNum1, float fNum2, float fNum3, int nNum, int nAdd, const char* FileName, float fx)
{
	m_nIdx = CTextureManager::Instance()->Register(FileName);
	m_nColTime = 0;

	//デバイス取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * nNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff, NULL);

	VERTEX_2D* pVtx;

	// ロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(fX1 + nCnt * fNum1, fY1 + nCnt2 * fNum3, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(fX2 + nCnt * fNum1 + fNum2, fY1 + nCnt2 * fNum3, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(fX1 + nCnt * fNum1, fY2 + nCnt2 * fNum3, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(fX2 + nCnt * fNum1 + fNum2, fY2 + nCnt2 * fNum3, 0.0f);

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fx, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fx, 1.0f);

	pVtx += nAdd;

	// アンロック
	m_pVtxBuff->Unlock();

	return S_OK;
}

//****************************************************************
// 終了
//****************************************************************
void CNumber::Uninit(void)
{
	// バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//****************************************************************
// 更新
//****************************************************************
void CNumber::Update(void)
{
	m_pos += m_move;
}

//****************************************************************
// 描画
//****************************************************************
void CNumber::Draw(void)
{

	//デバイス取得
	LPDIRECT3DDEVICE9 pD3DDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pD3DDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pD3DDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pD3DDevice->SetTexture(0, CTextureManager::Instance()->GetAddress(m_nIdx));
	//ポリゴンの描画
	pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,//プリミティブの種類
		0, 2); //頂点情報構造体のサイズ

}

//****************************************************************
//数字によってテクスチャ座標による設定
//****************************************************************
void CNumber::SetNumber(int nNumber, int nAdd)
{
	VERTEX_2D* pVtx = nullptr;
	
	// バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//ここで計算する
	float fTex = 0.1f * nNumber;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + fTex, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + fTex, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + fTex, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + fTex, 1.0f);

	pVtx += nAdd;

	// バッファをアンロック
	m_pVtxBuff->Unlock();
}

//****************************************************************
// カラーアニメーション
//****************************************************************
void CNumber::ColAnim(void)
{
	VERTEX_2D* pVtx = nullptr;


	if (m_nColTime >= 30)
	{
		m_bChange = true;
	}
	else if(m_nColTime < 0)
	{
		m_bChange = false;
	}

	// バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (m_bChange == true)
	{
		m_nColTime--;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.3f, 1.0f, 0.3f, 1.0f);
	}
	else
	{
		m_nColTime++;

		// 頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	// バッファをアンロック
	m_pVtxBuff->Unlock();
}