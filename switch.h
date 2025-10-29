//=================================================
//
//	switch.h
//
// Author:chikada shouya
//
//=================================================
#ifndef _SWITCH_H_
#define _SWITCH_H_
#include "main.h"
#include "block.h"
//�}�N��
#define HALF (0.5f)					// �����̒l

class CSwitch :public CBlock
{
public:
	struct Config {
		static constexpr float Depth = -10.0f;		// �ǂꂾ�����ނ�(�ʒu)
		static constexpr float PressSpeed = 0.05f; // ������鑬�x
		static constexpr float ReturnSpeed = 0.1f;// �߂鑬�x
	};

	CSwitch(int nPriority = 3);
	~CSwitch();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool IsFinished(void) { return m_IsFinished; }
	static CSwitch* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
	bool m_IsPressed;
	bool m_IsFinished;
};
#endif