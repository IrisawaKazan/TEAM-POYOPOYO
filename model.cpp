//****************************************************************
//
// 階層構造モデル処理[model.cpp]
// Author Kensaku Hatori
//
//****************************************************************
// インクルード
#include "model.h"
#include "manager.h"
#include "texturemanager.h"
#include "light.h"
//***************************************
// コンストラクタ
//***************************************
CModel::CModel()
{
	m_ParentModel = NULL;
	m_dwNumMat = NULL;
	m_nTexIndx = NULL;
	m_nTexNum = NULL;
	m_pBuffMat = NULL;
	m_pMesh = NULL;
	m_Pos = VEC3_NULL;
	m_OffSet = VEC3_NULL;
	m_Rot = VEC3_NULL;
}
//***************************************
// デストラクタ
//***************************************
CModel::~CModel()
{
}
//***************************************
// 初期化処理
//***************************************
HRESULT CModel::Init(const char* ModelFileName)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	D3DXMatrixIdentity(&m_mtxRot);
	m_mtxRot._11 = 1.0f;
	m_mtxRot._22 = 1.0f;
	m_mtxRot._33 = 1.0f;
	// Xファイルの読み込み
	D3DXLoadMeshFromX(ModelFileName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh
	);
	// マテリアルへのポインタ
	D3DXMATERIAL* pMat;
	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	int TexCount = 0;
	m_nTexIndx = new int[(int)m_dwNumMat];
	memset(m_nTexIndx, -1, sizeof(m_nTexIndx));
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_nTexIndx[nCntMat] != NULL)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				TexCount++;
				CTextureManager* pTexture = CTextureManager::Instance();
				m_nTexIndx[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
			}
			else
			{
				m_nTexIndx[nCntMat] = -1;
			}
		}
	}
	return S_OK;
}
//***************************************
// 更新処理
//***************************************
void CModel::Update(void)
{
}
//***************************************
// 終了処理
//***************************************
void CModel::Uninit(void)
{
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_nTexIndx != NULL)
	{
		delete m_nTexIndx;
		m_nTexIndx = NULL;
	}
	delete this;
}
//***************************************
// 描画処理
//***************************************
void CModel::Draw(void)
{
	CRenderer* pRenderer;
	pRenderer = CManager::GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	CTextureManager* pTexManager = CTextureManager::Instance();
	D3DXMATRIX mtxRot, mtxTrans, mtxParent;		// 計算用マトリックス
	D3DXMATERIAL* pMat;			// マテリアルへのポインタ

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_ParentModel != NULL)
	{
		mtxParent = m_ParentModel->GetMatrix();
	}
	else
	{
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出した「パーツのマトリックス」と「親のマトリックス」をかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// マテリアルデータへのポインタ
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		pDevice->SetTexture(0, pTexManager->GetAddress(m_nTexIndx[nCntMat]));

		// モデル(パーツ)の描画
		m_pMesh->DrawSubset(nCntMat);
	}

}

//***************************************
// 生成
//***************************************
CModel* CModel::Create(const char* ModelFileName)
{
	CModel* pModel = NULL;
	if (pModel == NULL)
	{
		pModel = new CModel;
		pModel->Init(ModelFileName);
		return pModel;
	}
	return NULL;
}