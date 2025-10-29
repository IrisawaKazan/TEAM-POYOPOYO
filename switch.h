//=================================================
//
//	switch.h
//
// Author:chikada shouya
//
//=================================================
#ifndef _SWITCH_H_
#define _SWITCH_H_
#include "main.h"
#include "block.h"
//�}�N��
#define HALF (0.5f)					// �����̒l

class CSwitch :public CBlock
{
public:
	struct Config {
		static constexpr float Depth = -8.5f;		// �ǂꂾ�����ނ�(�ʒu)
		static constexpr float PressSpeed = 0.05f; // ������鑬�x
		static constexpr float ReturnSpeed = 0.1f;// �߂鑬�x
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CSwitch(int nPriority = 3);
	~CSwitch();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void CollisionPlayer(void);

	// �Q�b�^�[
	bool IsPress(void) { return m_IsPressed; }

	// �ÓI�����o�֐�
	// ����
	static CSwitch* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
	bool m_IsPressed;		// ������Ă��邩�ǂ���
	D3DXVECTOR3 m_ApperPos;	// �����ʒus
};

// �N���X���`
class CDoor :public CBlock
{
public:
	struct Config {
		static constexpr float Limit = 100.0f;		// �ǂꂾ�����ނ�(�ʒu)
		static constexpr float UpSpeed = 0.5f; // ������鑬�x
		static constexpr float DownSpeed = 0.1f;// �߂鑬�x
	};

	// �R���X�g���N�^�E�f�X�g���N�^
	CDoor(int nPriority = 3);
	~CDoor();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �N��
	void Begin(void) { m_IsFunc = true; }

	// �V���b�g�_�E��
	void End(void) { m_IsFunc = false; }

	// �ÓI�����o�֐�
	// ����
	static CDoor* Create(std::string sName, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 Scale = { 1.0f,1.0f,1.0f });
private:
	bool m_IsFunc;			// �N�������ǂ���
	D3DXVECTOR3 m_ApperPos;	// �����ʒu
};
#endif