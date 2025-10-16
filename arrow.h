//------------------------------------
//
// ‚â‚¶‚é‚µ [arrow.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object3D.h"

//--------------------------------
// ‚â‚¶‚é‚µ‚ÌƒNƒ‰ƒX
//--------------------------------
class CArrow : public CObject3D
{
public:
	CArrow() : CObject3D(4), m_chengeSize{} {};
	~CArrow() {};
	static CArrow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetChengeSize(float size) { m_chengeSize = size; }
	void ChengeAngle(D3DXVECTOR3* pos, D3DXVECTOR3* rot) const;

private:
	float m_chengeSize; // –îˆó‚Ì”ÍˆÍ
};