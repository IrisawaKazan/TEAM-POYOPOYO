//*************************************
//
//�@�r���{�[�h����[ObjectBilBoard.h]
//�@Author:Hatori Kensaku
//
//*************************************

// ��d�C���N���[�h�h�~
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

// �C���N���[�h
#include "manager.h"

// �I�u�W�F�N�g�̔h���N���X���`[�r���{�[�h]
class CObjectBillBoard : public CObject
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObjectBillBoard(const int Priority = 3);
	~CObjectBillBoard();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �Z�b�^�[
	void SetPosition(D3DXVECTOR3 Pos) { m_Pos = Pos; };
	void SetSize(D3DXVECTOR2 Size) { m_Size = Size; };
	void SetCol(D3DXCOLOR Col) { m_Col = Col; };
	void SetTexIndx(int Indx) { m_nTexIndx = Indx; };

	// �Q�b�^�[
	D3DXVECTOR3 GetPos(void) { return m_Pos; };
	D3DXVECTOR2 GetSize(void) { return m_Size; };
	D3DXCOLOR GetCol(void) { return m_Col; };

	// ����
	static CObjectBillBoard* Create(D3DXVECTOR3 Pos, D3DXVECTOR2 Size);
private:
	// �����o�ϐ�
	LPDIRECT3DVERTEXBUFFER9 m_pVertex;	// ���_�o�b�t�@
	D3DXMATRIX m_mtxWorld;				// �}�g���b�N�X
	D3DXVECTOR3 m_Pos;					// �ʒu
	D3DXVECTOR2 m_Size;					// �傫��
	D3DXCOLOR m_Col;					// �F
	int m_nTexIndx;						// �e�N�X�`���ւ̃C���f�b�N�X
};
#endif // !_OBJECTBILLBOARD_H_