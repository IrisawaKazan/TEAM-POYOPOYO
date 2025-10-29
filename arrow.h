//------------------------------------
//
// ‚â‚¶‚é‚µ [arrow.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "naviobject.h"

//--------------------------------
// ‚â‚¶‚é‚µ‚ÌƒNƒ‰ƒX
//--------------------------------
class CArrow : public CNaviObject
{
public:
	CArrow() = default;
	~CArrow() = default;
	static CArrow* Create(D3DXVECTOR3 pos, D3DXMATRIX rotMtx, float angle, const char* filePath, D3DXVECTOR2 size, size_t idx);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void Activate(float* angle) const override;

private:
};