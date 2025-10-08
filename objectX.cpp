//*************************************
//
//　モデル処理[ObjectX.cpp]
//　Author:Hatori Kensaku
//
//*************************************

// インクルード
#include "objectX.h"
#include "manager.h"

//***************************************
// コンストラクタ
//***************************************
CObjectX::CObjectX(const int Priority) : CObject(Priority)
{
	m_Pos = VEC3_NULL;
	m_Rot = VEC3_NULL;
	m_Scale = VEC3_NULL;
	m_pMtxParent = NULL;
	m_fAlpha = 1.0f;
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);
}

//***************************************
// デストラクタ
//***************************************
CObjectX::~CObjectX()
{
}

//***************************************
// 初期化処理
//***************************************
HRESULT CObjectX::Init(void)
{
	m_Scale = { 1.0f,1.0f,1.0f };
	return S_OK;
}

//***************************************
// 終了処理
//***************************************
void CObjectX::Uninit(void)
{
	Release();
}

//***************************************
// 更新処理
//***************************************
void CObjectX::Update(void)
{
}

//***************************************
// 描画処理
//***************************************
void CObjectX::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTextureManager* pTexManager = CTextureManager::Instance();
	CModelManager* pModelTexManager = CModelManager::Instance();

	D3DXMATRIX mtxRot, mtxTrans,mtxScale;	// 計算用マトリックス
	D3DMATERIAL9 matDef;					// 現在のマテリアルの保存用
	D3DXMATERIAL* pMat;						// マテリアルへのポインタ

	CModelManager::ModelInfo modelinfo = pModelTexManager->GetAddress(pModelTexManager->Register(m_FilePath));

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_pMtxParent != NULL)
	{
		m_mtxWorld = *m_pMtxParent * m_mtxWorld;
	}

	if (m_mtxRot != NULL)
	{
		m_mtxWorld = m_mtxRot * m_mtxWorld;
	}

	// 拡大率を反映
	D3DXMatrixScaling(&mtxScale, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 現在のマテリアルの取得
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)modelinfo.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)modelinfo.dwNumMat; nCntMat++)
	{
		D3DXMATERIAL pCol = pMat[nCntMat];
		pCol.MatD3D.Diffuse.a = m_fAlpha;
		// マテリアルの設定
		pDevice->SetMaterial(&pCol.MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexManager->GetAddress(modelinfo.pTexture[nCntMat]));

		// モデル(パーツ)の描画
		modelinfo.pMesh->DrawSubset(nCntMat);
	}
	pDevice->SetMaterial(&matDef);
}

//***************************************
// 生成
//***************************************
CObjectX* CObjectX::Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path)
{
	CObjectX* pObjectX = NULL;
	// メモリ確保
	pObjectX = new CObjectX;

	if (pObjectX != nullptr)
	{
		pObjectX->SetPosition(Pos);
		pObjectX->SetRotasion(Rot);
		pObjectX->m_FilePath = Path;
		pObjectX->Init();
		return pObjectX;
	}
	else
	{
		return nullptr;
	}
}