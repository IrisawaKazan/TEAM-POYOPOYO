//****************************************************************
//
// �K�w�\�����f������[model.h]
// Author Kensaku Hatori
//
//****************************************************************
// ��d�C���N���[�h�h�~
#ifndef _MODEL_H_
#define _MODEL_H_
// �C���N���[�h
#include "main.h"
// ���f���N���X���`
class CModel
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CModel();
	~CModel();
	// ������
	HRESULT Init(const char* ModelFileName);
	void Update(void);
	void Uninit(void);
	void Draw(void);
	void Draw(D3DXCOLOR Col);
	// �Z�b�^�[
	void SetParent(CModel* pParent) { m_ParentModel = pParent; };
	void SetPos(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetOffSet(D3DXVECTOR3 OffSet) { m_OffSet = OffSet; };
	void SetRot(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetMtxRot(D3DXMATRIX mtxrot) { m_mtxRot = mtxrot; };
	void SetIndx(const int Indx) { m_nIndx = Indx; };
	// �Q�b�^�[
	CModel* GetParentModel(void) { return m_ParentModel; };
	D3DXVECTOR3 GetOffSet(void) { return m_OffSet; };
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3 GetRot(void) { return m_Rot; };
	D3DXMATRIX GetMatrix(void) { return m_mtxWorld; };
	int GetIndx(void) { return m_nIndx; };
	static CModel* Create(const char* ModelFileName);
private:
	// ���f�����̍\����
	LPD3DXMESH m_pMesh;						// ���b�V���ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;				// �}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;						// �}�e���A���̐�
	int m_nTexNum;							// ���f���̃e�N�X�`���̑���
	int* m_nTexIndx;						// ���f���̃e�N�X�`���̃C���f�b�N�X
	D3DXVECTOR3 m_OffSet;					// �I�t�Z�b�g
	D3DXVECTOR3 m_Pos;						// �ʒu
	D3DXVECTOR3 m_Rot;						// ����
	D3DXMATRIX m_mtxWorld;					// �}�g���b�N�X
	D3DXMATRIX m_mtxRot;					// ��]�s��
	CModel* m_ParentModel;					// �e���f��
	int m_nIndx;							// �K�w�\���̃C���f�b�N�X
};
#endif // !
