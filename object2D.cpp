//*************************************
//
//　2Dポリゴン処理[Object2D.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "Object2D.h"
#include "main.h"
#include "manager.h"
#include "renderer.h"

//*********************************************
// コンストラクタ
//*********************************************
CObject2D::CObject2D(int Priority) : CObject(Priority)
{
	// メンバ変数をNULにする
	m_pVertex = NULL;
	m_fAngle = NULL;
	m_fLength = NULL;
	m_fWidth = NULL;
	m_fHeigth = NULL;
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_nTexIndx = -1;
	m_isAlphaText = false;
}

//*********************************************
// デストラクタ
//*********************************************
CObject2D::~CObject2D()
{

}

//*********************************************
// 初期化処理
//*********************************************
HRESULT CObject2D::Init()
{
	// デバイスを取得
	CRenderer *pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVertex,
		NULL);

	// 頂点バッファをロック
	VERTEX_2D* pVtx = NULL;
	m_pVertex->Lock(0, 0, (void**)&pVtx, 0);

	if (pVtx != NULL)
	{
		pVtx[0].pos = VEC3_NULL;
		pVtx[1].pos = VEC3_NULL;
		pVtx[2].pos = VEC3_NULL;
		pVtx[3].pos = VEC3_NULL;

		pVtx[0].col = WHITE;
		pVtx[1].col = WHITE;
		pVtx[2].col = WHITE;
		pVtx[3].col = WHITE;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	// 頂点バッファをアンロック
	m_pVertex->Unlock();

	return S_OK;
}

//*********************************************
// 終了処理
//*********************************************
void CObject2D::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVertex != NULL)
	{
		m_pVertex->Release();
		m_pVertex = NULL;
	}
	// メモリの解放
	Release();
}

//*********************************************
// 更新処理
//*********************************************
void CObject2D::Update()
{
	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		//頂点座標の更新
		pVtx[0].pos.x = m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.y = m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[0].pos.z = 0.0f;
		pVtx[1].pos.x = m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.y = m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;
		pVtx[1].pos.z = 0.0f;
		pVtx[2].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;
		pVtx[2].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;
		pVtx[2].pos.z = 0.0f;
		pVtx[3].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;
		pVtx[3].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;
		pVtx[3].pos.z = 0.0f;

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
	}

	m_pVertex->Unlock();
}

//*********************************************
// ポリゴンの位置を設定
//*********************************************
void CObject2D::SetPosition(D3DXVECTOR3 Pos)
{
	m_Pos = Pos;

	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		//頂点座標の更新
		pVtx[0].pos.x = m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[0].pos.y = m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[0].pos.z = 0.0f;//0.0f;
		pVtx[1].pos.x = m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[1].pos.y = m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;//0.0f;
		pVtx[2].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[2].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;//0.0f;
		pVtx[3].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[3].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[3].pos.z = 0.0f;
	}
}

//*********************************************
// ポリゴンの向きを設定
//*********************************************
void CObject2D::SetRotasion(D3DXVECTOR3 Rot)
{
	m_Rot = Rot;

	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		//頂点座標の更新
		pVtx[0].pos.x = m_Pos.x + sinf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[0].pos.y = m_Pos.y + cosf(m_Rot.z - (D3DX_PI - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[0].pos.z = 0.0f;//0.0f;
		pVtx[1].pos.x = m_Pos.x + sinf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[1].pos.y = m_Pos.y + cosf(m_Rot.z + (D3DX_PI - m_fAngle)) * m_fLength;//pos.y - 25.0f;
		pVtx[1].pos.z = 0.0f;//0.0f;
		pVtx[2].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.x - 150.0f;
		pVtx[2].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f - m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[2].pos.z = 0.0f;//0.0f;
		pVtx[3].pos.x = m_Pos.x + sinf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.x + 150.0f;
		pVtx[3].pos.y = m_Pos.y + cosf(m_Rot.z + (0.0f + m_fAngle)) * m_fLength;//pos.y + 25.0f;
		pVtx[3].pos.z = 0.0f;
	}
}

//*********************************************
// ポリゴンの大きさを設定
//*********************************************
void CObject2D::SetSize(const D3DXVECTOR2 Size)
{
	m_fWidth = Size.x;
	m_fHeigth = Size.y;

	// 幅を変更してポリゴンの中心点からの角度を再設定
	m_fAngle = atan2f(Size.x, Size.y);
	// 大きさを再設定
	m_fLength = sqrtf((Size.x * Size.x) + (Size.y * Size.y));
}

//*********************************************
// ポリゴンのUV座標を設定
//*********************************************
void CObject2D::SetUv(const D3DXVECTOR2 Uv, const float SizeU, const float SizeV)
{
	// UvがカウントでSizeがテクスチャ座標の大きさ

	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		pVtx[0].tex = D3DXVECTOR2(SizeU * Uv.x, SizeV * Uv.y);
		pVtx[1].tex = D3DXVECTOR2(SizeU + (Uv.x * SizeU), SizeV * Uv.y);
		pVtx[2].tex = D3DXVECTOR2(SizeU * Uv.x, SizeV + (Uv.y * SizeV));
		pVtx[3].tex = D3DXVECTOR2(SizeU + (Uv.x * SizeU), SizeV + (Uv.y * SizeV));
	}

	m_pVertex->Unlock();
}

//*********************************************
// ポリゴンの色を設定
//*********************************************
void CObject2D::SetCol(const D3DXCOLOR Col)
{
	VERTEX_2D* pVtx = NULL;

	if (m_pVertex != NULL)
	{
		m_pVertex->Lock(0, 0, (void**)&pVtx, 0);
	}

	if (pVtx != NULL)
	{
		pVtx[0].col = Col;
		pVtx[1].col = Col;
		pVtx[2].col = Col;
		pVtx[3].col = Col;
	}

	m_pVertex->Unlock();
}

//*********************************************
// 描画処理
//*********************************************
void CObject2D::Draw()
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();

	if (m_isAlphaText)
	{
		// アルファテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	}

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVertex, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetTexture(0, CTextureManager::Instance()->GetAddress(m_nTexIndx));
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	if (m_isAlphaText)
	{
		// アルファテストを無効に戻す
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	}
}

//*********************************************
// 2Dポリゴンを作る
//*********************************************
CObject2D* CObject2D::Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, int priority)
{
	CObject2D* pObject2D;
	pObject2D = new CObject2D(priority);
	pObject2D->Init();
	pObject2D->SetPosition(Pos);
	pObject2D->SetRotasion(Rot);
	pObject2D->SetSize(D3DXVECTOR2(100.0f, 100.0f));
	pObject2D->SetCol(WHITE);
	return pObject2D;
}

//*********************************************
// 2Dポリゴンを作るテクスチャ指定
//*********************************************
CObject2D* CObject2D::Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot, const D3DXVECTOR2 Size, int priority)
{
	CObject2D* pObject2D;
	pObject2D = new CObject2D(priority);
	pObject2D->Init();
	pObject2D->SetPosition(Pos);
	pObject2D->SetRotasion(Rot);
	pObject2D->SetSize(Size);
	pObject2D->SetCol(WHITE);
	return pObject2D;
}