//==============================================================
//
// [timer.h]
// Author: Irisawa Kazan
//
//==============================================================
#ifndef _TIMER_H_ // ���̃}�N����`������ĂȂ�������
#define _TIMER_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

#include"main.h"
#include"object.h"

// �O���錾
class CNumber;

// �}�N����`
#define MAX_TIMER (2)

// �^�C�}�[�N���X
class CTimer : public CObject
{
public:
	CTimer(int nPriority = 7);
	~CTimer();

	static CTimer* Create(D3DXVECTOR3 pos);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);

	void SetTime(int nTime);
	void SubNs(int nValue);
	void SubMin(int nValue);

	int GetNs(void);
	int GetMin(void);
	static int GetTime(void) { return m_nTimer; }

private:
	static CNumber* m_pNumber1[MAX_TIMER];	// �i���o�[�̃|�C���^(�b)
	static CNumber* m_pNumber2[MAX_TIMER];	// �i���o�[�̃|�C���^(��)
	static CNumber* m_pNumber3;				// �i���o�[�̃|�C���^(:)
	D3DXVECTOR3 m_pos;						// �ʒu
	int m_nTime;							// �l
	int m_nNs;								// �b
	int m_nMin;								// ��
	static int m_nTimer;					// �^�C�}�[�̎擾
};

#endif