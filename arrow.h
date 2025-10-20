//------------------------------------
//
// やじるし [arrow.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object3D.h"

//--------------------------------
// やじるしのクラス
//--------------------------------
class CArrow : public CObject3D
{
public:
	CArrow() : CObject3D(4), m_chengeLength{}, m_idx{} {};
	~CArrow() {};
	static CArrow* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* filePath, D3DXVECTOR2 size, size_t idx);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetChengeLength(float length) { m_chengeLength = length; }
	void SetIdx(size_t idx) { m_idx = idx; }
	void ChengeAngle(const D3DXVECTOR3& pos, D3DXVECTOR3* rot) const;
	bool ReleaseHit(const D3DXVECTOR3& pos, float length) const;

	float GetChengeLength() const { return m_chengeLength; }
	size_t GetIdx() const { return m_idx; }

	void RequestRelease() { Release(); }

private:
	float m_chengeLength; // 矢印の範囲
	size_t m_idx;         // 矢印インデックス (Zバイアス用)
};