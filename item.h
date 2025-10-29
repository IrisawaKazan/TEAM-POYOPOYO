//****************************************************************
//
// �A�C�e���̏���
// Author:Takahashi Misaki
//
//****************************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

// �C���N���[�h
#include "objectX.h"

// �A�C�e���N���X
class CItem : public CObjectX
{
public:

	typedef enum
	{
		ITEM_NOEN = 0,	// �Ȃ�
		ITEM_BACK,		// ���ɐi�ގw��
		ITEM_RIGHT,		// �E�ɐi�ގw��
		ITEM_FRONT,		// ��O�ɐi�ގw��
		ITEM_LEFT,		// ���ɐi�ގw��
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

	// �A�C�e���̓����蔻��
	bool CollisionItem(const D3DXVECTOR3 pos, const float fWidth, const float fDepth);

	// �ÓI�����o�֐�
	static CItem* Create(const ITEM type,	// ���
		const D3DXVECTOR3 pos,				// �ʒu
		const D3DXVECTOR3 rot,				// ����
		const float fWidth,					// ����
		const float fDepth,					// ���s
		const D3DXVECTOR3 scale,			// ����
		const std::string FileName);		// ���f���̃t�@�C����

private:

	// �����o�ϐ�
	ITEM m_type;			// �A�C�e���̎��
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	float m_fWidth;			// ����
	float m_fDepth;			// ���s
	D3DXVECTOR3 m_scale;	// �g�嗦
	int m_nModelIdx;		// �C���f�b�N�X
};

#endif