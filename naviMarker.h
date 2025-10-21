//------------------------------------
//
// ナビゲーションマーカー [naviMarker.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object3D.h"

//--------------------------------
// ナビゲーションマーカーのクラス
//--------------------------------
class CNaviMarker : public CObject3D
{
public:
	CNaviMarker() : CObject3D(4), m_nBiasID{} {};
	~CNaviMarker() {};
	static CNaviMarker* Create(const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetBiasID(const size_t biasID) { m_nBiasID = biasID; }

private:
	size_t m_nBiasID; // バイアスID
};