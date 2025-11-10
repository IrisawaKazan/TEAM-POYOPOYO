//------------------------------------
//
// クライムマーカー [climb.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "naviobject.h"

//--------------------------------
// やじるしのクラス
//--------------------------------
class CClimb : public CNaviObject
{
public:
	CClimb() = default;
	~CClimb() = default;
	static CClimb* Create(D3DXVECTOR3 pos, D3DXMATRIX rotMtx, float angle, const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Activate(D3DXVECTOR3* outPos, float* outAngle) const override;

private:
};