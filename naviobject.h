//------------------------------------
//
// �i�r�Q�[�V�����I�u�W�F�N�g [naviobject.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "navi.h"   // �i�r�Q�[�V����
#include "object.h" // �I�u�W�F�N�g

//--------------------------------
// �i�r�Q�[�V�����I�u�W�F�N�g�̃N���X
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
	static constexpr float CHENGE_LENGTH_MAGNIFICATION = 0.5f; // ��������͈͂̔{��
	static constexpr float CHENGE_HEIGHT = 20.0f;              // �������鍂���̍�

	virtual void Activate(float* angle) const = 0;

private:
	void SetTriggerObject();
	void SetReleaseObject();

	LPDIRECT3DVERTEXBUFFER9 m_pVertex; // ���_
	int m_texIdx;                      // �e�N�X�`���ԍ�
	D3DXVECTOR3 m_pos;                 // ���W
	D3DXMATRIX m_mtxRot;               // ��]�s��
	float m_angle;                     // �A���O��(Y������]�s��K���O�ɉ�]������)
	D3DXVECTOR2 m_size;                // �T�C�Y

	static size_t m_nextIdx; // ���̃C���f�b�N�X (Trigger�n���p)

	CNavi::TYPE m_type;   // �i�r�Q�[�V�����I�u�W�F�N�g�̃^�C�v
	float m_length;       // �����ڂ͈̔�
	float m_chengeLength; // ��������͈�
	size_t m_idx;         // �C���f�b�N�X (Trigger�n���p)
	size_t m_biasIdx;     // �C���f�b�N�X (Z�o�C�A�X�p)

	std::unique_ptr<btCylinderShape> m_triggerShape;    // �����蔻��̌`(�A�N�e�B�u)
	std::unique_ptr<btCylinderShape> m_releaseShape;    // �����蔻��̌`(�A�N�e�B�u)
	std::unique_ptr<btCollisionObject> m_triggerObject; // �����蔻��(�A�N�e�B�u)
	std::unique_ptr<btCollisionObject> m_releaseObject; // �����蔻��(�㏑��)
};