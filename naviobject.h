//------------------------------------
//
// �i�r�Q�[�V�����I�u�W�F�N�g [naviobject.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "navi.h"     // �i�r�Q�[�V����
#include "object3D.h" // 3D�I�u�W�F�N�g

//--------------------------------
// �i�r�Q�[�V�����I�u�W�F�N�g�̃N���X
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
	static constexpr float CHENGE_LENGTH_MAGNIFICATION = 0.5f; // ��������͈͂̔{��

	virtual void Activate(D3DXVECTOR3* rot) const = 0;

private:
	static size_t m_nextIdx; // ���̃C���f�b�N�X (Trigger�n���p)

	CNavi::TYPE m_type;   // �i�r�Q�[�V�����I�u�W�F�N�g�̃^�C�v
	float m_length;       // �����ڂ͈̔�
	float m_chengeLength; // ��������͈�
	size_t m_idx;         // �C���f�b�N�X (Trigger�n���p)
	size_t m_biasIdx;     // �C���f�b�N�X (Z�o�C�A�X�p)
};