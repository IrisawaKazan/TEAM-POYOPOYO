//*************************************
//
//�@3D�|���S������[Object3D.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

// �C���N���[�h
#include "manager.h"

// �I�u�W�F�N�g�̔h���N���X���`[�RD�|���S��]
class CObject3D : public CObject
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObject3D(int Priority = 3);
	~CObject3D();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �Z�b�^�[
	void SetPosition(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetRotasion(D3DXVECTOR3 Rot) { m_Rot = Rot; };
	void SetRotasionDest(D3DXVECTOR3 Rot) { m_RotDest = Rot; };
	void SetSize(D3DXVECTOR2 Size) { m_fWidth = Size.x,m_fHeigth = Size.y; };
	void SetCol(D3DXCOLOR Col) { m_Col = Col; };
	void SetUV(D3DXVECTOR2 UVMax, D3DXVECTOR2 UVMin) { m_UVMax = UVMax;m_UVMin = UVMin; };
	void ResisterTexIndx(const int TexIndx) { m_nTexIndx = TexIndx; };

	// �Q�b�^�[
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR3 GetRot(void) { return m_Rot; };
	D3DXVECTOR2 GetUvMax(void) { return m_UVMax; };
	D3DXVECTOR2 GetUvMin(void) { return m_UVMin; };
	float GetWidth(void) { return m_fWidth; };
	float GetHeight(void) { return m_fHeigth; };

	// ����
	static CObject3D* Create(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot);
private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;				// �}�g���b�N�X
	D3DXVECTOR3 m_Pos;					// �ʒu
	D3DXVECTOR3 m_Rot;					// ����
	D3DXVECTOR3 m_RotDest;				// �ڕW�̌���
	D3DXVECTOR2 m_UVMax;				// �e�N�X�`�����W�̍ő�l
	D3DXVECTOR2 m_UVMin;				// �e�N�X�`�����W�̍ŏ��l
	D3DXCOLOR m_Col;					// �F
	float m_fWidth;						// ����
	float m_fHeigth;					// ����
	int m_nTexIndx;						// �e�N�X�`���ւ̃C���f�b�N�X
};
#endif // !