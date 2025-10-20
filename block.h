//=================================================
//
//	block.h
// 
// Author:�ߓc ����
//
//=================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "objectx.h"
#include "object.h"

//�}�N��
#define HALF (0.5f)					// �����̒l

class CBlock :public CObjectX
{
public:
	//***************************
	// �u���b�N�̎�ނ̗񋓌^�錾
	//***************************
	typedef enum
	{
		TYPE_0 = 0,	//
		TYPE_1,		//
		TYPE_2,		//
		TYPE_3,		//
		TYPE_4,		//
		TYPE_MAX,
	}TYPE;

	CBlock(int nPriority = 7);
	~CBlock();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBlock* Create(std::string sName,D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	bool Collision(D3DXVECTOR3* pos, D3DXVECTOR3* posOld, D3DXVECTOR3* move, D3DXVECTOR3* size);
	//bool Collision(void);

private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	TYPE m_type;			// �u���b�N�̃^�C�v
	D3DXVECTOR3 m_size;		// �T�C�Y
	D3DXVECTOR3 m_VtxMax;	// ���_�ő�l
	D3DXVECTOR3 m_VtxMin;	// ���_�ŏ��l
	std::string sNamePath;	// �t�@�C���p�X
	int m_nIdx;
};

#endif