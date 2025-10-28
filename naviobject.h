//------------------------------------
//
// ナビゲーションオブジェクト [naviobject.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "navi.h"     // ナビゲーション
#include "object3D.h" // 3Dオブジェクト

//--------------------------------
// ナビゲーションオブジェクトのクラス
//--------------------------------
class CNaviObject : public CObject3D
{
public:
	CNaviObject() : CObject3D(4), m_type{}, m_length{}, m_chengeLength{}, m_idx{ m_nextIdx }, m_biasIdx{} { m_nextIdx++; };
	~CNaviObject() override {};

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetType(CNavi::TYPE type) { m_type = type; }
	void SetLength(float length) { m_length = length; }
	void SetChengeLength(float length) { m_chengeLength = length; }
	void SetBiasIdx(size_t biasIdx) { m_biasIdx = biasIdx; }
	CNavi::TYPE ActivateTrigger(const D3DXVECTOR3& pos, D3DXVECTOR3* rot, size_t* pIdx) const;
	bool ReleaseTrigger(const D3DXVECTOR3& pos, float length) const;

	CNavi::TYPE GetType() const { return m_type; }
	float GetChengeLength() const { return m_chengeLength; }
	size_t GetIdx() const { return m_idx; }
	size_t GetBiasIdx() const { return m_biasIdx; }

	void RequestRelease() { Release(); }

protected:
	static constexpr float CHENGE_LENGTH_MAGNIFICATION = 0.5f; // 反応する範囲の倍率

	virtual void Activate(D3DXVECTOR3* rot) const = 0;

private:
	static size_t m_nextIdx; // 次のインデックス (Trigger渡し用)

	CNavi::TYPE m_type;   // ナビゲーションオブジェクトのタイプ
	float m_length;       // 見た目の範囲
	float m_chengeLength; // 反応する範囲
	size_t m_idx;         // インデックス (Trigger渡し用)
	size_t m_biasIdx;     // インデックス (Zバイアス用)
};