//*************************************
//
//�@2D�|���S������[Object2D.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

// �C���N���[�h�h�~
#include "Object.h"

// �I�u�W�F�N�g�N���X���p������2D�|���S���p�̃N���X��錾
class CObject2D :public CObject
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObject2D(int Priority = 3);
	~CObject2D();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CObject2D* Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot);
	static CObject2D* Create(const D3DXVECTOR3 Pos, const D3DXVECTOR3 Rot,const D3DXVECTOR2 Size);

	// �Z�b�^�[
	void SetPosition(const D3DXVECTOR3 Pos);
	void SetRotasion(const D3DXVECTOR3 Rot);
	void SetSize(const D3DXVECTOR2 Size);
	void SetUv(const D3DXVECTOR2 Uv, const float SizeU, const float SizeV);
	void SetCol(const D3DXCOLOR Col);
	void SetTexIndx(const int Indx) { m_nTexIndx = Indx; };

	// �Q�b�^�[
	D3DXVECTOR3 GetPosition(void) {return m_Pos; };
	D3DXVECTOR3 GetRotasion(void) { return m_Rot; };
	D3DXVECTOR2 GetSize(void) { return D3DXVECTOR2(m_fWidth, m_fHeigth); };
	int GetTexIndx(void) { return m_nTexIndx; };
private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// ���_�o�b�t�@
	D3DXVECTOR3 m_Pos;					// �ʒu
	D3DXVECTOR3 m_Rot;					// ����
	float m_fAngle;						// ���S�_����l���܂ł̊p�x
	float m_fLength;					// ����
	float m_fWidth;						// ����
	float m_fHeigth;					// ����
	int m_nTexIndx;						// �e�N�X�`���ւ̃C���f�b�N�X
};
#endif // !_OBJECT2D_H_