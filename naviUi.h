//------------------------------------
//
// �i�r�Q�[�V����UI [naviUi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once
#include <array>
#include "object.h"

class CObject2D;
//--------------------------------
// �i�r�Q�[�V����UI�̃N���X
//--------------------------------
class CNaviUI : public CObject
{
public:
	CNaviUI() : CObject(7), m_pObjects{}, m_objectTexturePaths{}, m_pos{}, m_size{} {};
	~CNaviUI() {};
	static CNaviUI* Create(const char* frameTexturePath, std::vector<const char*> objectTexturePaths, D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetFrameTexturePath(const char* frameTexturePath) { m_frameTexturePath = frameTexturePath; }
	void SetObjectTexturePaths(std::vector<const char*> objectTexturePaths) { m_objectTexturePaths = objectTexturePaths; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

private:
	void SetObjectUI(void);

	CObject2D* m_pFrame;                           // UI�t���[��
	std::array<CObject2D*, 3u> m_pObjects;         // 2D�I�u�W�F�N�g�z��
	const char* m_frameTexturePath;                // �t���[���̃e�N�X�`��
	std::vector<const char*> m_objectTexturePaths; // �i�r�Q�[�V�����I�u�W�F�N�g�̃e�N�X�`���z��
	D3DXVECTOR3 m_pos;                             // ��ʒu
	D3DXVECTOR2 m_size;                            // ��T�C�Y

};