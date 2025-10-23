//=================================================
//
//	gimmick.h
//
// Author:chikada shouya
//
//=================================================
#ifndef _GIMMICK_H_
#define _GIMMICK_H_
#include "main.h"
#include "block.h"
//�}�N��
#define HALF (0.5f)					// �����̒l

class CGimmick :public CBlock
{
public:
	//***************************
	// �M�~�b�N�̎�ނ̗񋓌^�錾
	//***************************
	typedef enum
	{
		TYPE_NONE = 0,	//
		TYPE_SWITCH,	//
		TYPE_2,			//
		TYPE_3,			//
		TYPE_4,			//
		TYPE_MAX,
	}TYPE;
	CGimmick(int nPriority = 7);
	~CGimmick();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CGimmick* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����
	TYPE m_type;			// �u���b�N�̃^�C�v
};
#endif