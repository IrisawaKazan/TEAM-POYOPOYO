//*************************************
//
//�@���f������[ObjectX.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

// �C���N���[�h
#include "main.h"
#include "object.h"

// �I�u�W�F�N�g�̔h���N���X���`[���f��]
class CObjectX : public CObject
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObjectX(const int Priority = 3);
	~CObjectX();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �Q�b�^�[
	float GetAlpha(void) { return m_fAlpha; };
	D3DXVECTOR3 GetPosition(void) { return m_Pos; };
	D3DXVECTOR3 GetRotasion(void) { return m_Rot; };
	D3DXVECTOR3* GetpPosition(void) { return &m_Pos; };
	D3DXVECTOR3* GetpRotasion(void) { return &m_Rot; };
	D3DXMATRIX* GetMtxParent(void) { return m_pMtxParent; };
	D3DXMATRIX GetMtxRot(void) { return m_mtxRot; };

	// �Z�b�^�[
	void SetPosition(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetRotasion(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetScale(D3DXVECTOR3 Scale) { m_Scale = Scale; };
	void SetAlpha(const float Alpha) { m_fAlpha = Alpha; };
	void SetRotMtx(D3DXMATRIX mtxRot) { m_mtxRot = mtxRot; };
	void SetParentMtx(D3DXMATRIX* mtxParent) { m_pMtxParent = mtxParent; };

	// ����
	static CObjectX* Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, std::string Path);
private:
	D3DXVECTOR3 m_Pos;			// �ʒu
	D3DXVECTOR3 m_Rot;			// ����
	D3DXVECTOR3 m_Scale;		// �傫��
	D3DXMATRIX* m_pMtxParent;	// �e�̃}�g���b�N�X
	D3DXMATRIX m_mtxWorld;		// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxRot;		// ��]�s��
	std::string m_FilePath;		// ���f���ւ̃p�X
	float m_fAlpha;				// �����x
};
#endif // !_OBJECTX_H_