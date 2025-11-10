//------------------------------------
//
// ジャンプマーカー [jump.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "naviobject.h"

//--------------------------------
// やじるしのクラス
//--------------------------------
class CJump : public CNaviObject
{
public:
	CJump() = default;
	~CJump() = default;
	static CJump* Create(D3DXVECTOR3 pos, D3DXMATRIX rotMtx, float angle, const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Activate(D3DXVECTOR3* outPos, float* outAngle) const override;

private:
};