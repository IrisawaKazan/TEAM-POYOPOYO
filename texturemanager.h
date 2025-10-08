//=================================================
//
//	TextureManager.h
// 
//	Author:�ߓc����
//
//=================================================

// ��d�C���N���[�h�h�~
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
	std::vector<TexInfo> m_vTex;						// �e�N�X�`���̃|�C���^
	int m_nNumAll;										// ����
	static std::unique_ptr<CTextureManager> m_Instance;	// �V���O���g��
};
#endif