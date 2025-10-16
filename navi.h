//------------------------------------
//
// ナビゲーションマーカー [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "objectbillboard.h"

//--------------------------------
// ナビゲーションマーカーのクラス
//--------------------------------
class CNavi : public CObjectBillBoard
{
public:
	CNavi() : m_RayPos{ 0.0f,0.0f,0.0f }, m_RayDir{ 0.0f,0.0f,0.0f } {};
	~CNavi() {};
	static CNavi* Create(D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

private:
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);

	D3DXVECTOR3 m_RayPos; // レイの始点
	D3DXVECTOR3 m_RayDir; // レイの方向
};