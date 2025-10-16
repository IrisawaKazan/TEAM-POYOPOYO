//------------------------------------
//
// ナビゲーションマーカー [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object3D.h"

class CArrow;
//--------------------------------
// ナビゲーションマーカーのクラス
//--------------------------------
class CNavi : public CObject3D
{
public:
	CNavi() : CObject3D(4), m_RayPos{ 0.0f,0.0f,0.0f }, m_RayDir{ 0.0f,0.0f,0.0f }, m_clickPos{ 0.0f,0.0f,0.0f }, m_apArrow{}, m_arrowAngle{} {};
	~CNavi() {};
	static CNavi* Create(const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	D3DXVECTOR3 GetClickPos(void) const { return m_clickPos; }

	// 仮実装(テスト実装)
	std::vector<const CArrow*> GetArrow(void) const { return m_apArrow; }

private:
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);

	static constexpr float HEIGHT = 0.05f; // 地面の高さ

	D3DXVECTOR3 m_RayPos; // レイの始点
	D3DXVECTOR3 m_RayDir; // レイの方向

	D3DXVECTOR3 m_clickPos; // クリックした位置

	// 仮実装(テスト実装)
	std::vector<const CArrow*> m_apArrow; // 矢印の配列
	float m_arrowAngle;                   // 矢印の角度
};