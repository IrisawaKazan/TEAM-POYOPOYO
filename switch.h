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
//É}ÉNÉç
#define HALF (0.5f)					// îºï™ÇÃíl

class CSwitch :public CBlock
{
public:
	CSwitch(int nPriority = 3);
	~CSwitch();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CSwitch* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
};
#endif