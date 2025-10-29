//------------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [naviMarker.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object.h"

//--------------------------------
// �i�r�Q�[�V�����}�[�J�[�̃N���X
//--------------------------------
class CNaviMarker : public CObject
{
public:
	CNaviMarker() : CObject(4), m_nBiasID{}, m_pVertex{}, m_texIdx{}, m_pos{}, m_mtxRot{}, m_size{} {};
	~CNaviMarker() {};
	static CNaviMarker* Create(const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetBiasID(const size_t biasID) { m_nBiasID = biasID; }
	void SetTexIdx(int texIdx) { m_texIdx = texIdx; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotMtx(const D3DXMATRIX& mtxRot) { m_mtxRot = mtxRot; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }

	D3DXVECTOR2 GetSize(void) { return m_size; }
	D3DXMATRIX GetRotMtx(void) { return m_mtxRot; }

private:
	size_t m_nBiasID;                  // �o�C�A�XID
	LPDIRECT3DVERTEXBUFFER9 m_pVertex; // ���_
	int m_texIdx;                      // �e�N�X�`���ԍ�
	D3DXVECTOR3 m_pos;                 // ���W
	D3DXMATRIX m_mtxRot;               // ��]�s��
	D3DXVECTOR2 m_size;                // �T�C�Y
};