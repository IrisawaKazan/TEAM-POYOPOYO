//=================================================
//
//	modelmanager.h
// 
//	Author:�ߓc����
//
//=================================================

// ��d�C���N���[�h�h�~
#ifndef _MODELMANAGER_H_
#define _MODELMANAGER_H_

#include "main.h"

class CModelManager
{
public:

	typedef struct
	{
		LPD3DXMESH pMesh;		// ���b�V���̃|�C���^
		LPD3DXBUFFER pBuffMat;	// �}�e���A���̃|�C���^
		DWORD dwNumMat;			// �}�e���A����
		std::string sName;
		int* pTexture;		// �e�N�X�`���̃|�C���^

	}ModelInfo;

	~CModelManager();
	HRESULT Load(std::string sName);
	void Unload(void);
	int Register(std::string sName);
	ModelInfo GetAddress(int nIdx);
	static CModelManager* Instance(void);
private:
	CModelManager();
	std::vector<ModelInfo> m_vModel;					// �e�N�X�`���̃|�C���^
	D3DXVECTOR3 m_VtxMax;								// ���_�ő�l
	D3DXVECTOR3 m_VtxMin;								// ���_�ŏ��l
	D3DXVECTOR3 m_Size;									// �T�C�Y
	static std::unique_ptr<CModelManager> m_Instance;	// �V���O���g��
};
#endif