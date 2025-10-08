//=================================================
//
//	TextureManager.h
// 
//	Author:近田尚也
//
//=================================================

// 二重インクルード防止
#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "main.h"

#define MAX_TEXTURE (128)

class CTextureManager
{
public:

	typedef struct
	{
		LPDIRECT3DTEXTURE9 Tex;
		std::string sName;
	}TexInfo;

	~CTextureManager();
	HRESULT Load(void);
	void Unload(void);
	int Register(std::string sName);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	static CTextureManager* Instance(void);
private:
	CTextureManager();
	std::vector<TexInfo> m_vTex;						// テクスチャのポインタ
	int m_nNumAll;										// 総数
	static std::unique_ptr<CTextureManager> m_Instance;	// シングルトン
};
#endif