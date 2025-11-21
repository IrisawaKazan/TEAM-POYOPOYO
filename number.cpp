//****************************************************************
//
// [number.h]
// Author: Irisawa Kazan
//
//****************************************************************
#include "number.h"
#include "manager.h"
#include "texturemanager.h"
#include "math_T.h"

//****************************************************************
// 静的メンバ変数
//****************************************************************
bool CNumber::m_bEasing = NULL;

//****************************************************************
// コンストラクタ
//****************************************************************
CNumber::CNumber()
{
	m_pVtxBuff = NULL;
	m_nIdx = NULL;
	m_nColTime = NULL;
	m_bChange = false;
	m_Type = TYPE_NONE;
	m_nAnimCounter = NULL;
	m_MaxFrame = NULL;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_pos[nCnt] = VEC3_NULL;
		m_fX[nCnt] = NULL;
		m_fY[nCnt] = NULL;
	}

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
CNumber* CNumber::Create()
{
	return NULL;
}

//****************************************************************
// 初期化
//****************************************************************
HRESULT CNumber::Init(float fX1, float fX2, float fY1, float fY2, int nCnt, int nCnt2, float fNum1, float fNum2, float fNum3, int nNum, int nAdd, const char* FileName, float fx,TYPE type)
{
	m_nIdx = CTextureManager::Instance()->Register(FileName);
	m_nColTime = 0;
	m_MaxFrame = 60;
	m_nAnimCounter = NULL;
	m_Type = type;
	m_bEasing = false;

	m_fX[0] = fX1 + nCnt * fNum1;
	m_fX[1] = fX2 + nCnt * fNum1 + fNum2;

	m_fY[0] = fY1 + nCnt2 * fNum3;
	m_fY[1] = fY2 + nCnt2 * fNum3;

	m_Dest = { m_fX[0] * 1.0f,SCREEN_HEIGHT * 0.5f,0.0f };	// 目標位置
	m_Apper[0] = { m_fX[0] * 8.425f,SCREEN_HEIGHT * 0.5f,0.0f };	// 初期位置
	m_Apper[1] = { m_fX[1] * 8.425f,SCREEN_HEIGHT * 0.5f,0.0f };	// 初期位置

	if (m_Type != TYPE_NONE)
	{
		fX1 = 1280.0f;
		fX2 = 1280.0f;
		fY1 = 360.0f;
		fY2 = 360.0f;
	}

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * nNum,
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
	if (m_nAnimCounter < m_MaxFrame)
	{
		// カウントを進める
		m_nAnimCounter++;

		D3DXVECTOR3 Diff = m_Dest - m_Apper[0];
		D3DXVECTOR3 Diff1 = m_Dest - m_Apper[1];

		float fEasing = EaseOutSine((float)m_nAnimCounter / (float)m_MaxFrame);

		// 初期位置に加算する
		m_pos[0] = m_Apper[0] + (Diff * fEasing);
		m_pos[1] = m_Apper[1] + (Diff1 * fEasing);
		SetPos();
	}
	else
	{
		m_bEasing = true;
	}
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

void CNumber::SetPos(void)
{
	if (m_pVtxBuff == NULL) return;

	D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	VERTEX_2D* pVtx = NULL;

	// 幅を変更してポリゴンの中心点からの角度を再設定
	float fAngle = atan2f((SCREEN_WIDTH * 0.02f),(SCREEN_HEIGHT * 0.02f));
	// 大きさを再設定
	float fLength = sqrtf((SCREEN_WIDTH * 0.02f * SCREEN_HEIGHT * 0.02f) + (SCREEN_WIDTH * 0.02f * SCREEN_HEIGHT * 0.02f));

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		//	//頂点座標の更新
		pVtx[0].pos.x = m_pos[0].x + sinf(rot.z - (D3DX_PI - fAngle)) * fLength;//pos.y - 25.0f;
		pVtx[0].pos.y = m_fY[0] + cosf(rot.z - (D3DX_PI - fAngle)) * fLength;//pos.x - 150.0f;
		pVtx[0].pos.z = 0.0f;//0.0f;
		pVtx[1].pos.x = m_pos[1].x + sinf(rot.z + (D3DX_PI - fAngle)) * fLength;//pos.x + 150.0f;
		pVtx[1].pos.y = m_fY[0] + cosf(rot.z + (D3DX_PI - fAngle)) * fLength;//pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;//0.0f;
		pVtx[2].pos.x = m_pos[0].x + sinf(rot.z + (0.0f - fAngle)) * fLength;//pos.x - 150.0f;
		pVtx[2].pos.y = m_fY[1] + cosf(rot.z + (0.0f - fAngle)) * fLength;//pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;//0.0f;
		pVtx[3].pos.x = m_pos[1].x + sinf(rot.z + (0.0f + fAngle)) * fLength;//pos.x + 150.0f;
		pVtx[3].pos.y = m_fY[1]+ cosf(rot.z + (0.0f + fAngle)) * fLength;//pos.y + 25.0f;
		pVtx[3].pos.z = 0.0f;
	}
	// バッファをアンロック
	m_pVtxBuff->Unlock();
}