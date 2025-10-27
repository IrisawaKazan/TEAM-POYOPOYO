//------------------------------------
//
// ナビゲーションUI [naviUi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object2D.h"

class CArrow;
//--------------------------------
// ナビゲーションUIのクラス
//--------------------------------
class CNaviUI : public CObject2D
{
public:
	CNaviUI() : CObject2D(7) {};
	~CNaviUI() {};
	static CNaviUI* Create(const char* filePath, D3DXVECTOR3 pos, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
};