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
	// ���̌���
	enum class ARROW_DIRECTION : unsigned char
	{
		Left,
		Front,
		Back,
		Max
	};

	CNavi() : CObject3D(4), m_RayPos{ 0.0f,0.0f,0.0f }, m_RayDir{ 0.0f,0.0f,0.0f }, m_clickPos{ 0.0f,0.0f,0.0f }, m_apArrow{}, m_direction{} {};
	~CNavi() {};
	static CNavi* Create(const char* filePath, D3DXVECTOR2 size);

	HRESULT Init(void) override;
	void Uninit(void) override;
	void Update(void) override;
	void Draw(void) override;

	D3DXVECTOR3 GetClickPos(void) const { return m_clickPos; }

	std::vector<const CArrow*> GetArrow(void) const { return m_apArrow; }
	ARROW_DIRECTION GetDirection(void) const { return m_direction; }

private:
	void CreateRay(D3DXVECTOR2 mousePos);
	D3DXVECTOR3 PlaneIntersect(float fHeight);

	static constexpr float HEIGHT = 0.05f; // �n�ʂ̍���

	D3DXVECTOR3 m_RayPos; // ���C�̎n�_
	D3DXVECTOR3 m_RayDir; // ���C�̕���

	D3DXVECTOR3 m_clickPos; // �N���b�N�����ʒu

	std::vector<const CArrow*> m_apArrow; // ���̔z��
	ARROW_DIRECTION m_direction;          // ���̌���
};