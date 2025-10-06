//****************************************************************
//
// ���C�g�̏���[light.h]
// Author Kensaku Hatori
//
//****************************************************************

// ��d�C���N���[�h�h�~
#ifndef _LIGHT_H_
#define _LIGHT_H_

// �C���N���[�h
#include "main.h"

// ���C�g�N���X���`
class CLight
{
public:
	struct Config {
		static constexpr int NumLIght = 3;
		static const D3DXVECTOR3 DefoultDir;
		static const D3DXCOLOR Evening;
	};
	// �R���X�g���N�^�E�f�X�g���N�^
	CLight();
	~CLight();

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// �Z�b�^�[
	int SetLight(D3DLIGHT9 Setter);
	void DeleteLight(const int Indx);
	void ResetCol(void);
private:
	// �����o�ϐ�
	D3DLIGHT9 m_Light[Config::NumLIght];	// ���C�g��ݒ肷��p
	int m_nCntLight;						// ���C�g�̃J�E���^
};
#endif // !_LIGHT_H_