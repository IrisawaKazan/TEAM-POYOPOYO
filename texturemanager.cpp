//=================================================
//
//	TextureManager.cpp
// 
//	Author:近田尚也
//
//=================================================
#include "texturemanager.h"
#include "manager.h"

// 静的メンバ変数
std::unique_ptr<CTextureManager> CTextureManager::m_Instance = NULL;

//==============================================================
// コンストラクタ
//==============================================================
CTextureManager::CTextureManager()
{
	// ベクターのクリア
	m_vTex.clear();

	// 総数の初期化
	m_nNumAll = NULL;
}

//==============================================================
// デストラクタ
//==============================================================
CTextureManager::~CTextureManager()
{

}

//==============================================================
// 読み込み
//==============================================================
HRESULT CTextureManager::Load()
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// ローカルのテクスチャ配列
	const char* TexName[MAX_TEXTURE] =
	{
		//------------------------------------
		"data\\TEXTURE\\inseki.jpg",				//	0
		"data\\TEXTURE\\effect000.jpg",				//	1
		"data\\TEXTURE\\sky000.jpg",				//	2
		"data\\TEXTURE\\title.jpg",					//	3
		"data\\TEXTURE\\title5.png",				//	4
		"data\\TEXTURE\\title1.png",				//	5
		"data\\TEXTURE\\sea1.jpg",					//	6
		"data\\TEXTURE\\sea02s.jpg",				//	7
		"data\\TEXTURE\\number005.png",				//	8
		"data\\TEXTURE\\ue.jpg",					//	9
		//------------------------------------			
		"data\\TEXTURE\\ue1.jpg",					//	10
		"data\\TEXTURE\\ue2.jpg",					//	11
		"data\\TEXTURE\\ue3.jpg",					//	12
		"data\\TEXTURE\\kabe.jpg",					//	13
		"data\\TEXTURE\\kabe1.jpg",					//	14
		"data\\TEXTURE\\kabe2.jpg",					//	15
		"data\\TEXTURE\\kabe3.jpg",					//	16
		"data\\TEXTURE\\renga1.jpg",				//	17
		"data\\TEXTURE\\face3.jpg",					//	18
		"data\\TEXTURE\\kaminari.jpg",				//	19
		//------------------------------------			
		"data\\TEXTURE\\moyou.jpg",					//	20
		"data\\TEXTURE\\red1.png",					//	21
		"data\\TEXTURE\\koron.png",					//	22
		"data\\TEXTURE\\Tumbase.png",				//	23
		"data\\TEXTURE\\YellowGauge.png",			//	24
		"data\\TEXTURE\\check1.png",				//	25
		"data\\TEXTURE\\continue2.png",				//	26
		"data\\TEXTURE\\retry2.png",				//	27
		"data\\TEXTURE\\quit2.png",					//	28
		"data\\TEXTURE\\brack.png",					//	29
		//------------------------------------			
		"data\\TEXTURE\\MasicCircle.png",			//	30
		"data\\TEXTURE\\humanface.jpg",				//	31
		"data\\TEXTURE\\result.jpg",				//	32
		"data\\TEXTURE\\tutorial.jpg",
		"data\\TEXTURE\\tree.jpg",					//	
		//------------------------------------			
	};											  

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		// ローカルテクスチャに情報が入っていたら
		if (TexName[nCnt] != nullptr)
		{
			// ローカル変数
			TexInfo Info = {};

			// テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				TexName[nCnt],
				&Info.Tex);

			Info.sName = TexName[nCnt];

			m_vTex.push_back(Info);
		}
	}
	return S_OK;
}

//==============================================================
// 破棄
//==============================================================
void CTextureManager::Unload(void)
{
	for (auto Texs = m_vTex.begin(); Texs != m_vTex.end(); Texs++)	// 情報コンテナにアクセス
	{
		//テクスチャがNULLじゃなかったら
		if (Texs->Tex != nullptr)
		{
			//終了
			Texs->Tex->Release();
			Texs->Tex = nullptr;
		}

		if (Texs->sName.empty() != NULL)
		{
			Texs->sName.clear();
		}

	}

	// ベクターのクリア
	m_vTex.clear();
}

//==============================================================
// 追加
//==============================================================
int CTextureManager::Register(std::string sName)
{
	//デバイス取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < static_cast<int>(m_vTex.size()); nCnt++)
	{
		if (m_vTex[nCnt].sName.compare(sName) == 0)
		{
			return nCnt;
		}
	}

	// ローカル変数
	TexInfo Info = {};

	// テクスチャの読み込み
	HRESULT hr = D3DXCreateTextureFromFile(pDevice,
		sName.c_str(),
		&Info.Tex);

	// 読み込みに失敗したら
	if (FAILED(hr))
	{
		return -1;
	}

	Info.sName = sName;

	m_vTex.push_back(Info);

	int nIdx = m_vTex.size() - 1;
	return nIdx;
}

//==============================================================
// アドレスの取得
//==============================================================
LPDIRECT3DTEXTURE9 CTextureManager::GetAddress(int nIdx)
{
	if (static_cast<int>(m_vTex.size()) <= nIdx || nIdx < 0)return NULL;
	return m_vTex[nIdx].Tex;
}

//==============================================================
// インスタンスの生成
//==============================================================
CTextureManager* CTextureManager::Instance(void)
{
	if (m_Instance != NULL) return m_Instance.get();
	m_Instance.reset(new CTextureManager);
	return m_Instance.get();
}
