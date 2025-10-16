//------------------------------------
//
// �i�r�Q�[�V�����}�[�J�[ [navi.h]
// Author : Fuma Sato
//
//------------------------------------
#pragma once

#include "object3D.h"

class CArrow;
//--------------------------------
// �i�r�Q�[�V�����}�[�J�[�̃N���X
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

	// ������(�e�X�g����)
	std::vector<const CArrow*> GetArrow(void) const { return m_apArrow; }

private:
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);

	static constexpr float HEIGHT = 0.05f; // �n�ʂ̍���

	D3DXVECTOR3 m_RayPos; // ���C�̎n�_
	D3DXVECTOR3 m_RayDir; // ���C�̕���

	D3DXVECTOR3 m_clickPos; // �N���b�N�����ʒu

	// ������(�e�X�g����)
	std::vector<const CArrow*> m_apArrow; // ���̔z��
	float m_arrowAngle;                   // ���̊p�x
};