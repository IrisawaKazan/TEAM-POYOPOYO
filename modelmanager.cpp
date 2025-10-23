//==============================================================
//
//	modelmanager.cpp
//	Author:chikada shouya
//
//==============================================================
#include "modelmanager.h"
#include "manager.h"
#include <assert.h>

// 静的メンバ変数
std::unique_ptr<CModelManager> CModelManager::m_Instance = NULL;

//==============================================================
// コンストラクタ
//==============================================================
CModelManager::CModelManager()
{
	// ベクターのクリア
	m_vModel.clear();

	// 総数の初期化
	//m_nNumAll = NULL;
}

//==============================================================
// デストラクタ
//==============================================================
CModelManager::~CModelManager()
{

}

//==============================================================
// 読み込み
//==============================================================
HRESULT CModelManager::Load(std::string sName)
{
	return S_OK;
}

//==============================================================
// モデルの法線再設定
//==============================================================
void CModelManager::ReCalcNormalize(const int Indx)
{
	// 法線のスムース化
	const float Epsilon = 1e-6f;
	std::vector<DWORD> adjacency(m_vModel[Indx].pMesh->GetNumFaces() * 3);
	m_vModel[Indx].pMesh->GenerateAdjacency(Epsilon, adjacency.data());

	HRESULT hr = D3DXComputeNormals(m_vModel[Indx].pMesh, adjacency.data());

	if (FAILED(hr))
	{
		// 失敗したら
		assert(0 && "モデルのスムース化に失敗しました");
	}
}

//==============================================================
// 破棄
//==============================================================
void CModelManager::Unload(void)
{
	for (auto Models = m_vModel.begin(); Models != m_vModel.end(); Models++)	// 情報コンテナにアクセス
	{
		// テクスチャの破棄
		if (Models->pTexture != nullptr)
		{
			delete[] Models->pTexture;

			Models->pTexture = nullptr;
		}

		// バッファの破棄
		if (Models->pBuffMat != nullptr)
		{
			//終了
			Models->pBuffMat->Release();
			Models->pBuffMat = nullptr;
		}

		// メッシュの破棄
		if (Models->pMesh != nullptr)
		{
			//終了
			Models->pMesh->Release();
			Models->pMesh = nullptr;
		}

		
	}

	// ベクターのクリア
	m_vModel.clear();
}

//==============================================================
// 追加
//==============================================================
int CModelManager::Register(std::string sName)
{
	// 不正な文字列なら
	if (sName.size() <= 0) return -1;

	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < static_cast<int>(m_vModel.size()); nCnt++)
	{
		// 名前のものがあったら
		if (m_vModel[nCnt].sName.compare(sName) == 0)
		{
			return nCnt;
		}
	}

	// ローカル変数
	ModelInfo Info = {};

	//Xファイルの読み込み(全パーツ)2
	HRESULT hr = D3DXLoadMeshFromX(sName.c_str(),
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&Info.pBuffMat,
		NULL,
		&Info.dwNumMat,
		&Info.pMesh);

	D3DXMATERIAL* pMat;	//マテリアルへのポインタ

	// テクスチャの分の配列の確保
	Info.pTexture = new int[Info.dwNumMat];

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)Info.pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)Info.dwNumMat; nCntMat++)
	{

		if (pMat[nCntMat].pTextureFilename != nullptr)
		{//テクスチャファイルが存在する

			// テクスチャポインタ取得
			CTextureManager* pTexture = CTextureManager::Instance();
			Info.pTexture[nCntMat] = pTexture->Register(pMat[nCntMat].pTextureFilename);
		}
		else
		{
			Info.pTexture[nCntMat] = -1;
		}
	}


	// 読み込みに失敗したら
	if (FAILED(hr))
	{
		return -1;
	}

	// 名前の保存
	Info.sName = sName;

	// 情報を前から追加していく
	m_vModel.push_back(Info);

	// インデックスに最後の情報の数を追加
	int nIdx = m_vModel.size() - 1;
	return nIdx;
}

//==============================================================
// アドレスを取得
//==============================================================
CModelManager::ModelInfo CModelManager::GetAddress(int nIdx)
{
	if (static_cast<int>(m_vModel.size()) <= nIdx || nIdx < 0)return {};
	return m_vModel[nIdx];
}

//==============================================================
// インスタンスの生成
//==============================================================
CModelManager* CModelManager::Instance(void)
{
	if (m_Instance != NULL) return m_Instance.get();
	m_Instance.reset(new CModelManager);
	return m_Instance.get();
}

