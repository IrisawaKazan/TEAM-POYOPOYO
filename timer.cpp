////==============================================================
////
//// [timer.cpp]
//// Author: Irisawa Kazan
////
////==============================================================
//#include"timer.h"
//#include"number.h"
//#include"renderer.h"
//#include"manager.h"
//
//// 静的メンバ変数宣言
//LPDIRECT3DTEXTURE9 CTimer::m_pTexture = nullptr;
//CNumber* CTimer::m_apNumber[MAX_TIMER] = {};
//
////----------------------------------------
//// コンストラクタ
////----------------------------------------
//CTimer::CTimer(int nPriority) : CObject(nPriority)
//{
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_fxSize = 0.0f;
//	m_fySize = 0.0f;
//	m_nTimer = NULL;
//	m_nContTimer = NULL;
//}
//
////----------------------------------------
//// デストラクタ
////----------------------------------------
//CTimer::~CTimer()
//{
//
//}
//
////----------------------------------------
//// タイマーの生成処理
////----------------------------------------
//CTimer* CTimer::Create(D3DXVECTOR3 pos, float xsize, float ysize)
//{
//	CTimer* pTimer = nullptr;
//
//	if (pTimer == nullptr)
//	{
//		pTimer = new CTimer;
//	}
//
//	pTimer->m_pos = pos;
//	pTimer->m_fxSize = xsize;
//	pTimer->m_fySize = ysize;
//
//	pTimer->Init();
//
//	return pTimer;
//}
//
////----------------------------------------
//// タイマーの初期化処理
////----------------------------------------
//HRESULT CTimer::Init(void)
//{
//	m_nContTimer = 0; // 初期タイム
//
//	float fTexPos = m_fySize / MAX_TIMER;
//
//	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
//	{
//		m_apNumber[nCnt] = new CNumber;
//		m_apNumber[nCnt]->SetSize(fTexPos, m_fxSize);
//		m_apNumber[nCnt]->SetPosition(m_pos);
//		m_apNumber[nCnt]->Init(D3DXVECTOR3(m_pos.x + (fTexPos * 2.0f * nCnt), m_pos.y, 0.0f), fTexPos, m_fxSize);
//
//		m_apNumber[nCnt]->BindTexture(m_pTexture);
//	}
//
//	// タイプの設定
//	SetType(TYPE_TIMER);
//
//	return S_OK;
//}
//
////----------------------------------------
//// タイマーの終了処理
////----------------------------------------
//void CTimer::Uninit(void)
//{
//	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
//	{
//		if (m_apNumber[nCnt] != nullptr)
//		{
//			m_apNumber[nCnt]->Uninit();
//
//			delete m_apNumber[nCnt];
//			m_apNumber[nCnt] = nullptr;
//		}
//	}
//
//	CObject::Release();
//}
//
////----------------------------------------
//// タイマーの更新処理
////----------------------------------------
//void CTimer::Update(void)
//{
//	AddTimer();
//}
//
////----------------------------------------
//// タイマーの描画処理
////----------------------------------------
//void CTimer::Draw(void)
//{
//	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
//	{
//		if (m_apNumber[nCnt] != nullptr)
//		{
//			m_apNumber[nCnt]->Draw();
//		}
//	}
//}
//
////----------------------------------------
//// タイマーの位置の設定処理
////----------------------------------------
//void CTimer::SetPosition(D3DXVECTOR3 pos)
//{
//	m_pos = pos;
//}
//
////----------------------------------------
//// タイマーの位置の取得処理
////----------------------------------------
//D3DXVECTOR3 CTimer::GetPos(void)
//{
//	return m_pos;
//}
//
////----------------------------------------
//// タイマーの加算処理
////----------------------------------------
//void CTimer::AddTimer(void)
//{
//	int texU[MAX_TIMER] = { NULL };
//
//	m_nTimer++;
//
//	if (m_nTimer >= 60)
//	{
//		//m_nContTimer--; // カウントダウン
//		m_nContTimer++; // カウントアップ
//		m_nTimer = 0;
//	}
//
//	texU[0] = m_nContTimer % 1000 / 100;
//	texU[1] = m_nContTimer % 100 / 10;
//	texU[2] = m_nContTimer % 10 / 1;
//
//	for (int nCnt = 0; nCnt < MAX_TIMER; nCnt++)
//	{
//		m_apNumber[nCnt]->Timer(texU[nCnt]);
//	}
//}