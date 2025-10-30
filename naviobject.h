//------------------------------------
//
// ナビゲーションオブジェクト [naviobject.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "navi.h"   // ナビゲーション
#include "object.h" // オブジェクト

//--------------------------------
// ナビゲーションオブジェクトのクラス
//--------------------------------
class CNaviObject : public CObject
{
public:
	CNaviObject() : CObject(4), m_pVertex{}, m_texIdx{}, m_pos{}, m_mtxRot{}, m_angle{}, m_size{}, m_type{}, m_length{}, m_chengeLength{}, m_idx{ m_nextIdx }, m_biasIdx{}, m_triggerShape{}, m_releaseShape{}, m_triggerObject{}, m_releaseObject{} { m_nextIdx++; };
	~CNaviObject() override {};

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	void SetTexIdx(int texIdx) { m_texIdx = texIdx; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotMtx(D3DXMATRIX rotMtx) { m_mtxRot = rotMtx; }
	void SetAngle(float angle) { m_angle = angle; }
	void SetSize(D3DXVECTOR2 size) { m_size = size; }
	void SetType(CNavi::TYPE type) { m_type = type; }
	void SetLength(float length) { m_length = length; }
	void SetChengeLength(float length) { m_chengeLength = length; }
	void SetBiasIdx(size_t biasIdx) { m_biasIdx = biasIdx; }
	CNavi::TYPE ActivateTrigger(const btCollisionObject* const& collisionObject, float* pAngle, size_t* pIdx) const;
	bool ReleaseTrigger(const btCollisionObject* const& collisionObject) const;

	D3DXVECTOR3 GetPos(void) { return m_pos; }
	float GetAngle(void) const { return m_angle; }
	CNavi::TYPE GetType() const { return m_type; }
	float GetChengeLength() const { return m_chengeLength; }
	size_t GetIdx() const { return m_idx; }
	size_t GetBiasIdx() const { return m_biasIdx; }
	btCollisionObject* GetreleaseCollObject() { return m_releaseObject.get(); }

	void RequestRelease() { Release(); }

protected:
	static constexpr float CHENGE_LENGTH_MAGNIFICATION = 0.5f; // 反応する範囲の倍率
	static constexpr float CHENGE_HEIGHT = 20.0f;              // 反応する高さの差

	virtual void Activate(float* angle) const = 0;

private:
	void SetTriggerObject();
	void SetReleaseObject();

	LPDIRECT3DVERTEXBUFFER9 m_pVertex; // 頂点
	int m_texIdx;                      // テクスチャ番号
	D3DXVECTOR3 m_pos;                 // 座標
	D3DXMATRIX m_mtxRot;               // 回転行列
	float m_angle;                     // アングル(Y軸を回転行列適応前に回転させる)
	D3DXVECTOR2 m_size;                // サイズ

	static size_t m_nextIdx; // 次のインデックス (Trigger渡し用)

	CNavi::TYPE m_type;   // ナビゲーションオブジェクトのタイプ
	float m_length;       // 見た目の範囲
	float m_chengeLength; // 反応する範囲
	size_t m_idx;         // インデックス (Trigger渡し用)
	size_t m_biasIdx;     // インデックス (Zバイアス用)

	std::unique_ptr<btCylinderShape> m_triggerShape;    // 当たり判定の形(アクティブ)
	std::unique_ptr<btCylinderShape> m_releaseShape;    // 当たり判定の形(アクティブ)
	std::unique_ptr<btCollisionObject> m_triggerObject; // 当たり判定(アクティブ)
	std::unique_ptr<btCollisionObject> m_releaseObject; // 当たり判定(上書き)
};