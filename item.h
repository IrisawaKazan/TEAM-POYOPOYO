//****************************************************************
//
// �A�C�e���̏���
// Author:Takahashi Misaki
//
//****************************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

// �C���N���[�h
#include "object3D.h"

// �A�C�e���N���X
class CItem : public CObject3D
{
public:

	typedef enum
	{
		ITEM_NOEN = 0,	// �Ȃ�
		ITEM_BACK,		// ���ɐi�ގw��
		ITEM_FORWARD,	// �O�i����w��
		ITEM_FRONT,		// ��O�ɐi�ގw��
		ITEM_MAX,		// ���
	}ITEM;

	// �R���X�g���N�^�E�f�X�g���N�^
	CItem();
	~CItem();

	// �����o�֐�
	HRESULT Init(void);
	void Update(void);
	void Uninit(void);
	void Draw(void);
	bool CollisionItem(const D3DXVECTOR3 pos, const float fWidth, const float fDepth);		// �A�C�e���̓����蔻��

	// �ÓI�����o�֐�
	static CItem* Create(const ITEM type, const D3DXVECTOR3 pos, const float fWidth, const float fDepth);

private:

	// �����o�ϐ�
	ITEM m_type;		// �A�C�e���̎��
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_rot;	// ����
	float m_fWidth;		// ����
	float m_fDepth;		// ���s
};

#endif