////==============================================================
////
//// [number.h]
//// Author: Irisawa Kazan
////
////==============================================================
//#include"number.h"
//#include"manager.h"
//#include"renderer.h"
//
//// 静的メンバ変数宣言
//
//
////----------------------------------------
//// コンストラクタ
////----------------------------------------
//CNumber::CNumber()
//{
//	m_pTexture = nullptr;
//	m_pVtxBuff = nullptr;
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_faUV = 0.0f;
//	m_fzUV = 0.0f;
//	m_fSizeX = 0.0f;
//	m_fSizeY = 0.0f;
//}
//
////----------------------------------------
//// デストラクタ
////----------------------------------------
//CNumber::~CNumber()
//{
//
//}
//
////----------------------------------------
//// 初期化処理
////----------------------------------------
//HRESULT CNumber::Init(D3DXVECTOR3 pos, float sizeX, float sizeY)
//{
//	m_pos = pos;
//	m_faUV = 0.1f;
//	m_fzUV = 1.0f;
//
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
//
//	// 頂点バッファの生成
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&m_pVtxBuff,
//		NULL);
//
//	VERTEX_2D* pVtx; // 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// 頂点座標の設定
//	pVtx[0].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y - sizeY, m_pos.z);
//	pVtx[1].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y - sizeY, m_pos.z);
//	pVtx[2].pos = D3DXVECTOR3(m_pos.x - sizeX, m_pos.y + sizeY, m_pos.z);
//	pVtx[3].pos = D3DXVECTOR3(m_pos.x + sizeX, m_pos.y + sizeY, m_pos.z);
//
//	// rhwの設定
//	pVtx[0].rhw = 1.0f;
//	pVtx[1].rhw = 1.0f;
//	pVtx[2].rhw = 1.0f;
//	pVtx[3].rhw = 1.0f;
//
//	// 頂点カラーの設定
//	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
//
//	// テクスチャ座標の設定
//	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(m_faUV, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.0f, m_fzUV);
//	pVtx[3].tex = D3DXVECTOR2(m_faUV, m_fzUV);
//
//	// 頂点バッファをアンロックする
//	m_pVtxBuff->Unlock();
//
//	return S_OK;
//}
//
////----------------------------------------
//// 終了処理
////----------------------------------------
//void CNumber::Uninit(void)
//{
//	// テクスチャの破棄
//	if (m_pTexture != nullptr)
//	{
//		m_pTexture->Release();
//		m_pTexture = nullptr;
//	}
//
//	// 頂点バッファの破棄
//	if (m_pVtxBuff != nullptr)
//	{
//		m_pVtxBuff->Release();
//		m_pVtxBuff = nullptr;
//	}
//}
//
////----------------------------------------
//// 描画処理
////----------------------------------------
//void CNumber::Draw(void)
//{
//	// デバイスの取得
//	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
//
//	// 頂点バッファをデータストリームに設定
//	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
//
//	// 頂点フォーマットの設定
//	pDevice->SetFVF(FVF_VERTEX_2D);
//	
//	// テクスチャの設定
//	pDevice->SetTexture(0, m_pTexture);
//	
//	// ポリゴンの描画
//	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
//}
//
////----------------------------------------
//// 位置の設定処理
////----------------------------------------
//void CNumber::SetPosition(D3DXVECTOR3 pos)
//{
//	m_pos = pos;
//}
//
////----------------------------------------
//// テクスチャの設定処理
////----------------------------------------
//void CNumber::SetUV(float aUV, float zUV)
//{
//	m_faUV = aUV;
//	m_fzUV = zUV;
//}
//
////----------------------------------------
//// サイズの設定処理
////----------------------------------------
//void CNumber::SetSize(float sizeX, float sizeY)
//{
//	m_fSizeX = sizeX;
//	m_fSizeY = sizeY;
//}
//
////----------------------------------------
//// 桁数の設定処理
////----------------------------------------
//void CNumber::SetDigit(int digit)
//{
//	float tex = digit * 0.1f;
//
//	VERTEX_2D* pVtx; // 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// テクスチャ座標の設定
//	pVtx[0].tex = D3DXVECTOR2(tex, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(m_faUV + tex, 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(tex, m_fzUV);
//	pVtx[3].tex = D3DXVECTOR2(m_faUV + tex, m_fzUV);
//
//	// 頂点バッファをアンロックする
//	m_pVtxBuff->Unlock();
//}
//
////----------------------------------------
//// タイマー加算処理
////----------------------------------------
//void CNumber::Timer(int timer)
//{
//	VERTEX_2D* pVtx; // 頂点情報へのポインタ
//
//	// 頂点バッファをロックし,頂点情報へのポインタを取得
//	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
//
//	// テクスチャ座標の設定
//	pVtx[0].tex = D3DXVECTOR2(0.1f * timer, 0.0f);
//	pVtx[1].tex = D3DXVECTOR2(0.1f + (0.1f * timer), 0.0f);
//	pVtx[2].tex = D3DXVECTOR2(0.1f * timer, 1.0f);
//	pVtx[3].tex = D3DXVECTOR2(0.1f + (0.1f * timer), 1.0f);
//
//	// 頂点バッファをアンロックする
//	m_pVtxBuff->Unlock();
//}
//
////----------------------------------------
//// テクスチャの割り当て
////----------------------------------------
//void CNumber::BindTexture(const LPDIRECT3DTEXTURE9 pTexture)
//{
//	m_pTexture = pTexture;
//}